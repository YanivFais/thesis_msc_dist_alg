/*********************************************************
 * File:        RtpData.h
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: real time protocol handler
 **************************************************************************/

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

/**
 * RTP data message handler
 */
class RtpData {
public:

	/**
	 * construct from file, if load real data buffers or only headers
	 */
  RtpData(const string& name,bool buffersLoadP=true);

  ~RtpData();

  /*
   * "play" handler for streaming data
   */
  int playHandler(int client);


  /**
   * @param i ID of packet
   * @return size of packet number i
   */
  int getSizes(int i) {
	  return sizes[i%sizes.size()];
  }

  /**
   * @return number of packets
   */
  int getSizes() { return sizes.size(); }

  /**
   * @param i ID of packet
   * @return time of transmit of packet number i in mili sec
   */
  int getTimes(int i) {
	  return times[i%times.size()];
  }

  /**
   * @return number of packets
   */
  int getTimes() { return times.size(); }

  /**
   * @return total size of video data
   */
  int getTotalSize() { return totalSize; }

  /**
   * set a packet with buffer
   * @param buffer data handler of video packet
   * @param ID id of packet
   * @return true if last
   */
  bool setPacket(RD_buffer_t ** buffer,unsigned & ID);

private:

  /**
   * add buffer of packet for transmittion
   */
  void addBuffer(RD_buffer_t * buffer);

  /**
   * @param a time value
   * @return time as double from time value
   */
  double tdbl(struct timeval *a);


  FILE *in;               /* input file */
  int first;         /* time offset of first packet */
  int verbose;        /* be chatty about packets sent */
  int wallclock;      /* use wallclock time rather than timestamps */
  int begin;      /* time of first packet to send */
  int end; /* when to stop sending */

  /**
   * buffers of video packets
   */
  vector<RD_buffer_t*> buffers;

  /**
   * sizes of packets
   */
  vector<int> sizes;

  /**
   * transmition time of packets
   */
  vector<int> times;

  /**
   * total size of video
   */
  unsigned long totalSize;

  /**
   * whether to load actul video buffer or only header
   */
  bool buffersLoad;
};

#endif // RTP_DATA_H



