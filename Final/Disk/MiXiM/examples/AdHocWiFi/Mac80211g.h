/*********************************************************
 * File:        Mac80211g.h
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: media access control 802.11g extension
 **************************************************************************/
#ifndef MAC80211G_H_
#define MAC80211G_H_

#include "Mac80211.h"
#include "PER2SNRTable.h"
#include <string>
using namespace std;

/**
 * MAC 802.11g extension to MiXiM
 */
class Mac80211g: public Mac80211 {
public:
	Mac80211g();
	virtual ~Mac80211g();

	/** @brief initialize model according to stage */
	virtual void initialize(int);

    /** @brief computes the duration of a transmission over the physical channel, given a certain bitrate */
    /** Added by Yaniv Fais - make virtual**/ virtual
    simtime_t packetDuration(double bits, double br);

protected:

    /**
     * name of file name for per2snr communication data
     */
    static string per2snr_file_name;

    /**
     * PER 2 SNR table
     */
    static PER2SNRTable * PER2SNRTableObj;

	/**
         * handle end of contention perid
         */
	virtual void handleEndContentionTimer() { Mac80211::handleEndContentionTimer(); }

	/**
	 * current message data bitrate
	 */
	double currMsgDataBitrate;
};

#endif /* MAC80211G_H_ */
