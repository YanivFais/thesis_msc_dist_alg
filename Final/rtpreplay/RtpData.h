#ifndef RTP_DATA_H
#define RTP_DATA_H

#include <string>
#include <vector>

#include <stdio.h>

using namespace std;

extern "C" {
#include <sys/types.h>
#include "types.h"
#include "rtpdump.h"
}

class RtpData {
public:

  RtpData(const string& name,bool buffersLoadP=true);

  ~RtpData();

  int playHandler(int client);


  int getSizes(int i) {
	  return sizes[i%sizes.size()];
  }

  int getSizes() { return sizes.size(); }

  int getTimes(int i) {
	  return times[i%times.size()];
  }

  int getTimes() { return times.size(); }

  int getTotalSize() { return totalSize; }

  bool setPacket(RD_buffer_t ** buffer,unsigned & ID);

private:

  void addBuffer(RD_buffer_t * buffer);

  double tdbl(struct timeval *a);

  FILE *in;               /* input file */
  int first;         /* time offset of first packet */
  int verbose;        /* be chatty about packets sent */
  int wallclock;      /* use wallclock time rather than timestamps */
  int begin;      /* time of first packet to send */
  int end; /* when to stop sending */

  vector<RD_buffer_t*> buffers;

  vector<int> sizes;

  vector<int> times;

  unsigned long totalSize;

  bool buffersLoad;
};

#endif // RTP_DATA_H



