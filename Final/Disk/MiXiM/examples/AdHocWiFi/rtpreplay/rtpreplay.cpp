/*
* Recreate packet stream recorded with rtpdump -f dump and timestamps file created by AdHocWiFi Omnet++/MiXiM based simulator
* Usage:
* -v         verbose
* -T         use absolute time rather than RTP timestamps
* -f         file to read
* -t         times file result of simulation
* -b         begin time
* -e         end time
* -s         local binding port
* destination/port[/ttl]
*
* Program reads ahead by READAHEAD packets to compensate for reordering.
* Currently does not correct SR/RR absolute (NTP) timestamps,
* but should. Receiver reports are fairly meaningless.
*
* (c) 1994-1998 Henning Schulzrinne (Columbia University); all rights reserved
* MiXiM version: Yaniv Fais : yaniv.fais@gmail.com
*/

extern "C" {
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>    /* gettimeofday() */
#include <sys/socket.h>  /* struct sockaddr */
#include <netinet/in.h>
#include <arpa/inet.h>   /* inet_ntoa() */
#include <netdb.h>       /* gethostbyname() */
#include <time.h>
#include <stdio.h>       /* stderr, printf() */
#include <string.h>
#include <stdlib.h>      /* perror() */
#include <unistd.h>      /* write() */
#include "sysdep.h"
#if HAVE_SEARCH_H
#include <search.h>      /* hash table */
#else
#include "hsearch.h"
#endif
#include "notify.h"      /* notify_start(), ... */
#include "rtp.h"         /* RTP headers */
//#include "types.h"
//#include "rtpdump.h"     /* RD_packet_t */
#include "multimer.h"    /* timer_set() */
#include "ansi.h"
}

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "RtpData.h"
using namespace std;
#define READAHEAD 16 /* must be power of 2 */

static int verbose = 0;        /* be chatty about packets sent */
static u_int32 begin = 0;      /* time of first packet to send */
static u_int32 end = UINT_MAX; /* when to stop sending */
static int sock[2];            /* output sockets */

/**
* File with times and video rates which is a result of simulation
*/
class TimesFile {
public:
	TimesFile() {};

	void read(const string& fName,const string& base) {
		ifstream f(fName.c_str());
		if (!f.is_open()) {
			cerr << "Can't open " << fName << endl;
			exit(1);
		}
		unsigned i = 0;
		times.reserve(3000);
		long double first = -1;
		while (!f.eof()) {
			string line;
			getline(f,line);
			if (line.find("#")==0) {
				if (line.find("0_")==1)
					continue; // just ignore this
				string::size_type p = line.find('\r');
				if (p!=string::npos) {
					line.erase(p,1);
                                }
				string name = base + line.substr(1) + ".rtp";
				cout << "Reading file " << name << endl;
				if (dumps.find(name)==dumps.end()) {
					RtpData *dump = new RtpData(name);
					dumps[name] = dump;
				}
				if (switches.empty())
					switches.push_back(make_pair(i,name));
				switches.push_back(make_pair(i,name));
				continue;
			}
			int id;
			long double d;
			sscanf(line.c_str(),"%d@%Lf",&id,&d);
			if (first<0) {
				first = d;
				d = 0;
			}
			else d -= first;
			d*=1000;
			//cout << "Id=" << id << " Type=" << type << " time=" << d << endl;

			if (times.size()<=(unsigned)id)
				times.resize(times.capacity()*2);
			times[id] = d;
			i++;
		}
		f.close();
	}

	double videoDelay(unsigned int id,double offset) { return (id<times.size()) ? times[id] : offset; }
	int videoPackets() { return times.size(); }

	bool setPacket(RD_buffer_t** buffer,unsigned& switchID,unsigned cnt,unsigned& ID,unsigned long& offset) {
		pair<unsigned,string> Switch = switches[switchID];
		if (Switch.first==ID)
			Switch = switches[++switchID];
		RtpData * dump = dumps[Switch.second];
		offset = times[cnt]-first;
		return dump->setPacket(buffer,ID);
	}

private:
	double first;
	vector<double> times;
	map<string,RtpData *> dumps;
	vector<pair<unsigned,string> > switches;
};

TimesFile timesData;

static void usage(char *argv0)
{
  fprintf(stderr,
"Usage: %s [-v] [-T] [-f RTP files prefix ] [-t times file] [-b begin time] [-e end time] \
[-s localport] destination/port[/ttl]\n",
  argv0);
  exit(1);
} /* usage */


static double tdbl(struct timeval *a)
{
  return a->tv_sec + a->tv_usec/1e6;
} /* tdbl */


/*
* Transmit RTP/RTCP packet on output socket and mark as read.
*/
static void play_transmit(RD_buffer_t& buffer)
{
  if (buffer.p.hdr.length) {
    if (send(sock[buffer.p.hdr.plen == 0],
        buffer.p.data, buffer.p.hdr.length, 0) < 0) {
      perror("write");
    }
    //buffer.p.hdr.length = 0;
  }
} /* play_transmit */

unsigned int videoID = 0 , switchID = 0,cnt=0;

/*
* Timer handler: read next record from file and insert into timer
* handler.
*/
static Notify_value play_handler(Notify_client client)
{
  timeval now;           /* current time */
  timeval next;          /* next packet generation time */
  gettimeofday(&now, 0);

  static struct timeval start = now;  /* generation time of first played back p. */
  struct rt_ts {
    struct timeval rt;          /* real-time */
    unsigned long ts;           /* timestamp */
  };
  rtp_hdr_t *r;

  unsigned long offset;
  RD_buffer_t * buffer;
  bool done = timesData.setPacket(&buffer,switchID,cnt++,videoID,offset);

  /* playback scheduled packet */
  play_transmit(*buffer);

  /* If we are done, skip rest. */
  if (done || (end == 0)) return NOTIFY_DONE;

  if (verbose > 0) {
    printf("! %1.3f %s(%3d;%3d) t=%6lu",
      tdbl(&now), buffer->p.hdr.plen ? "RTP " : "RTCP",
      buffer->p.hdr.length, buffer->p.hdr.plen,
      (unsigned long)/*buffer->p.hdr.*/offset);

    if (buffer->p.hdr.plen) {
      r = (rtp_hdr_t *)buffer->p.data;
      printf(" pt=%u ssrc=%8lx %cts=%9lu seq=%5u",
        (unsigned int)r->pt,
        (unsigned long)ntohl(r->ssrc), r->m ? '*' : ' ',
        (unsigned long)ntohl(r->ts), ntohs(r->seq));
    }
    printf("\n");
  }

  next = start;
  next.tv_usec += offset*1000;

  while (next.tv_usec >= 1000000) {
    next.tv_usec -= 1000000;
    next.tv_sec  += 1;
  }

  //cout << now.tv_sec << ":" << now.tv_usec << "-" << next.tv_sec <<":" << next.tv_usec << "=" << offset << endl;
  timer_set(&next, play_handler, (Notify_client)0, 0);
  return NOTIFY_DONE;
} /* play_handler */


extern "C" char *optarg;
extern "C" int optind;
extern "C" int hpt(char *h, struct sockaddr *sa, unsigned char *ttl);

int main(int argc, char *argv[])
{
  unsigned char ttl = 1;
  static struct sockaddr_in sin;
  static struct sockaddr_in from;
  int sourceport = 0;  /* source port */
  int on = 1;          /* flag */
  int i;
  int c;
  char * times = NULL;
  string baseName = "wildlife";
  /* For NT, we need to start the socket; dummy function otherwise */
  startupSocket();

  /* parse command line arguments */
  while ((c = getopt(argc, argv, "b:e:f:Ts:vh:t:n")) != EOF) {
    switch(c) {
    case 'b':
      begin = atof(optarg) * 1000;
      break;
    case 'e':
      end = atof(optarg) * 1000;
      break;
    case 't':
    	times = strdup(optarg);
      break;
    case 'f':
    	baseName = optarg;
      break;
    case 's':  /* locked source port */
      sourceport = atoi(optarg);
      break;
    case 'v':
      verbose = 1;
      break;
    case '?':
    case 'h':
      usage(argv[0]);
      break;
    }
  }

  if (times==NULL) {
    usage(argv[0]);
    exit(1);
  }


  if (optind < argc) {
    if (hpt(argv[optind], (sockaddr *)&sin, &ttl) < 0) {
      usage(argv[0]);
      exit(1);
    }
    if (sin.sin_addr.s_addr == (unsigned)-1) {
      fprintf(stderr, "%s: Invalid host. %s\n", argv[0], argv[optind]);
      usage(argv[0]);
      exit(1);
    }
    if (sin.sin_addr.s_addr == INADDR_ANY) {
      struct hostent *host;
      struct in_addr *local;
      if ((host = gethostbyname("localhost")) == NULL) {
        perror("gethostbyname()");
        exit(1);
      }
      local = (struct in_addr *)host->h_addr_list[0];
      sin.sin_addr = *local;
    }
  }

  timesData.read(times,baseName);

  /* create/connect sockets if they don't exist already */
  if (!sock[0]) {
    for (i = 0; i < 2; i++) {
      sock[i] = socket(PF_INET, SOCK_DGRAM, 0);
      if (sock[i] < 0) {
        perror("socket");
        exit(1);
      }
      sin.sin_port = htons(ntohs(sin.sin_port) + i);

      if (sourceport) {
        memset((char *)(&from), 0, sizeof(struct sockaddr_in));
        from.sin_family      = PF_INET;
        from.sin_addr.s_addr = INADDR_ANY;
        from.sin_port        = htons(sourceport + i);

        if (setsockopt(sock[i], SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
          perror("SO_REUSEADDR");
          exit(1);
        }

  #ifdef SO_REUSEPORT
        if (setsockopt(sock[i], SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) < 0) {
          perror("SO_REUSEPORT");
          exit(1);
        }
  #endif

        if (bind(sock[i], (struct sockaddr *)&from, sizeof(from)) < 0) {
          perror("bind");
          exit(1);
        }
      }

      if (connect(sock[i], (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("connect");
        exit(1);
      }

      if (IN_CLASSD(ntohl(sin.sin_addr.s_addr)) &&
          (setsockopt(sock[i], IPPROTO_IP, IP_MULTICAST_TTL, &ttl,
                   sizeof(ttl)) < 0)) {
        perror("IP_MULTICAST_TTL");
        exit(1);
      }
    }
  }

#if defined(WIN32)
  /*
   * We have to set the socket array when we use 'select' in NT,
   * otherwise the 'select' function in NT will consider all the
   * three fd_sets are NULL and return an error.  Error code
   * WSAEINVAL means The timeout value is not valid, or all three
   * descriptor parameters were NULL but the timeout value is valid.
   * After setting Writefds, the program runs ok.
   */
//  notify_set_socket(sock[i], 1);
  /*
   * Modified by Wenyu and Akira 12/27/01
   * setting Writefds was causing
   *   1)consuming CPU 100% (behave polling)
   *   2)slow
   *   3)large jitter
   * therefore, we changed it to set dummy fd to Readfds.
   */
  notify_set_socket(winfd_dummy, 0);
#endif


  hcreate(100); /* create hash table for SSRC entries */
  play_handler(0);
  notify_start();
  hdestroy();

  return 0;
} /* main */
