/*********************************************************
 * File:        AdHocWiFiMac.h
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: media access control layer for AdHocWiFi
 **************************************************************************/

#ifndef ADHOCWIFIMAC_H_
#define ADHOCWIFIMAC_H_

#include "Mac80211g.h"
#include "ScheduleItemCollection.h"
class AdHocWiFiApplLayer;

/**
 * Implementation of MAC with statistics which derives from 802.11g MAC
 * Adds various specific features such as statistics collection
 */
class AdHocWiFiMac: public Mac80211g {
public:

	AdHocWiFiMac();

	virtual ~AdHocWiFiMac();

	/** @brief initialize model **/
	virtual void initialize(int stage);


	static long long bitrates[9];

	/**
	 * @return actual bitrate for PER=1 and given MCS and packet length for 802.11g
	 * @param mcs MCS of transmission
	 * @param packetLength length of packet payload
	 */
	static unsigned int getActualBitrate(int mcs,int packetLength);

	/**
	 * @return data bitrate for current message
	 */
	double getCurrMsgDataBitrate() { return currMsgDataBitrate; }

	/**
	 * delay a message send based on MCS rate
	 * @param rate MCS rate of conflict msg
	 */
	void delayMsg(int rate);

	/**
	 * adjust rate (MCS) according to SNR on edge
	 */
	static int getAdjustedRate(ScheduleItemSPtr task);

protected:

	/** @brief send data frame */
	virtual void sendDATAframe(Mac80211Pkt*);

	/**
	 * handle message from application/network
	 */
	virtual void handleUpperMsg(cMessage *msg);

	/**
	 * handle message from Phy
	 */
	virtual void handleLowerMsg(cMessage *msg);

	/**
	 * handle lower control message
	 */
	virtual void handleLowerControl(cMessage* msg);

	/**
	 * begin a new MAC cycle (RTS/CTS/DATA/ACK)
	 */
	virtual void beginNewCycle();


private:

	/**
	 * application on-top of this MAC
	 */
	AdHocWiFiApplLayer * appl;


	/**
	 * map between a mac address to an application address
	 */
	static map<int,int> mac2applAddr;

	/**
	 * <source,destination> to SNR map
	 */
	typedef map<pair<int,int> , double > EdgeToSnrMap;
	static EdgeToSnrMap snrs;

	/**
	 * adjust MCS according to SNR
	 */
	static bool adjustMCS;

};

#endif /* ADHOCWIFIMAC_H_ */
