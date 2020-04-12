/*********************************************************
 * File:        RtpData.cpp
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: real time protocol handler
 **************************************************************************/
/*
* Program reads ahead by READAHEAD packets to compensate for reordering.
* Currently does not correct SR/RR absolute (NTP) timestamps,
* but should. Receiver reports are fairly meaningless.
*
* Original file taken from RTP Tools:(c) 1994-1998 Henning Schulzrinne (Columbia University); all rights reserved
* Modified by Yaniv Fais <yaniv.fais@gmail.com>
*/

extern "C" {
  #include <sys/time.h>    /* gettimeofday() */
}
#include <RtpData.h>
#include <exception>
#include <iostream>
#include <stdexcept>
using namespace std;
#ifdef __unix__
#include <sys/socket.h>
#include <arpa/inet.h>
#else
#include <winsock.h>  /* struct sockaddr */
#endif

extern "C" {
#include <limits.h>
#include <time.h>
#include <stdio.h>       /* stderr, printf() */
#include <string.h>
#include <stdlib.h>      /* perror() */
#include <unistd.h>      /* write() */

#include "rtp.h"         /* RTP headers */


#include "ansi.h"

}
#include <fstream>
using namespace std;

RtpData::RtpData(const string& name,bool buffersLoadP)
	: first(-1),verbose(0),wallclock(0),begin(0),end(UINT_MAX),totalSize(0),buffersLoad(buffersLoadP)
{


      if (!(in = fopen(name.c_str(), "rb"))) {
    	perror("While attempting to open:");
        fprintf(stderr,"Can't open RTP Dump file %s\n",name.c_str());
        throw std::runtime_error ( " Can't open RTP Dump file" ) ;
      }

	  static struct sockaddr_in sin;

	  /* read header of input file */
	  if (RD_header(in, &sin, 0) < 0) {
	    fprintf(stderr, "Invalid header\n");
	    exit(1);
	  }


	  /* initialize event queue */
	  first = -1;

	  while (playHandler(0)) {
	  };

	  fclose(in);
}


RtpData::~RtpData()
{
	for (vector<RD_buffer_t*>::iterator it = buffers.begin(); it != buffers.end(); it++)
		delete *it;
}

double RtpData::tdbl(struct timeval *a)
{
  return a->tv_sec + a->tv_usec/1e6;
} /* tdbl */


/*
* Transmit RTP/RTCP packet on output socket and mark as read.
*/
void RtpData::addBuffer(RD_buffer_t * buffer)
{
  if (buffer->p.hdr.length) {
	  // size in bits
	  sizes.push_back(buffer->p.hdr.length*8);
	  if (first==-1)
		  first = buffer->p.hdr.offset;
	   times.push_back(buffer->p.hdr.offset-first);
	  totalSize += buffer->p.hdr.length*8;
	  if (buffersLoad)
		  buffers.push_back(buffer);
  }
}


/*
* Timer handler: read next record from file and insert into timer
* handler.
*/
int RtpData::playHandler(int client)
{
  struct timeval now;           /* current time */
  struct rt_ts {
    struct timeval rt;          /* real-time */
    unsigned long ts;           /* timestamp */
  };
  rtp_hdr_t *r;
  int b = (int)client;  /* buffer to be played now */

  gettimeofday(&now, 0);

  RD_buffer_t * buffer = new RD_buffer_t();

  /* Get next packet; try again if we haven't reached the begin time. */
  do {
    if (RD_read(in, buffer) == 0) return 0;
  } while (buffer->p.hdr.offset < (unsigned)begin);

  if (verbose > 0 && b >= 0) {
    printf("! %1.3f %s(%3d;%3d) t=%6lu",
      tdbl(&now), buffer->p.hdr.plen ? "RTP " : "RTCP",
      buffer->p.hdr.length, buffer->p.hdr.plen,
      (unsigned long)buffer->p.hdr.offset);

    if (buffer->p.hdr.plen) {
      r = (rtp_hdr_t *)buffer->p.data;
      printf(" pt=%u ssrc=%8lx %cts=%9lu seq=%5u",
        (unsigned int)r->pt,
        (unsigned long)ntohl(r->ssrc), r->m ? '*' : ' ',
        (unsigned long)ntohl(r->ts), ntohs(r->seq));
    }
    printf("\n");
  }

  addBuffer(buffer);

  return 1;

}

bool RtpData::setPacket(RD_buffer_t ** buffer,unsigned & ID)
{
	(*buffer) = buffers[ID++];
	if (ID==times.size())
		return true;
	return false;
}

