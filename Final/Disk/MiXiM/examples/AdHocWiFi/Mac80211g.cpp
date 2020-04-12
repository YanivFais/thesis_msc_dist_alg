/*********************************************************
 * File:        Mac80211g.cpp
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: media access control 802.11g extension
 **************************************************************************/

#include <stdio.h>

#include "Mac80211g.h"

PER2SNRTable * Mac80211g::PER2SNRTableObj = NULL;
string Mac80211g::per2snr_file_name = "per2snr.txt";

Mac80211g::Mac80211g() {
}

Mac80211g::~Mac80211g() {
}

void Mac80211g::initialize(int stage)
{

	if (PER2SNRTableObj==NULL) {
		PER2SNRTableObj = new PER2SNRTable(per2snr_file_name.c_str());
	}

  Mac80211::initialize(stage);

  // override base 802.11b bitrate
  bitrate = hasPar("bitrate") ? par("bitrate").doubleValue() : BITRATES_80211[0];
  
  defaultBitrate = bitrate;
  
  EV << " MAC Address: " << myMacAddr
     << " rtsCtsThreshold: " << rtsCtsThreshold
     << " bitrate: " << bitrate
       << " autoBitrate: " << autoBitrate
     << endl;
  
  currMsgDataBitrate = defaultBitrate;
}

/** @brief computes the duration of a transmission over the physical channel, given a certain bitrate */
/** Note: base class is hacked to be virtual !!! **/
simtime_t Mac80211g::packetDuration(double bits, double br)
{
  if (currMsgDataBitrate!=0)
    br = currMsgDataBitrate;
  static const double PHY_HEADER_TIME = 28e-6;//26us
  double duration  = ceil((16+bits+6)/(br/1e6))*1e-6 + PHY_HEADER_TIME;
  EV <<" duration="<<duration*1e6<<"us("<<bits<<"bits "<<br/1e6<<"Mbps)"<<endl;
  return duration;
}
