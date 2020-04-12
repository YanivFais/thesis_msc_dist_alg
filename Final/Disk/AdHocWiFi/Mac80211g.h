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
	 * current message data bitrate
	 */
	double currMsgDataBitrate;
};

#endif /* MAC80211G_H_ */
