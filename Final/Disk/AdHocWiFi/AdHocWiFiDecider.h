/*********************************************************
 * File:        AdHocWiFiDecider.h
 *
 * Author:      Yaniv Fais
 *
 *
 ***************************************************************************
 * description: decider whether a package transmission is succesful or not
 **************************************************************************/

#ifndef AD_HOC_WI_FI_DECIDER_H_
#define AD_HOC_WI_FI_DECIDER_H_

#include <string>
#include <cmath>
#include <map>
#include <list>
#include <vector>
using namespace std;

#include <Decider.h>
#include <Decider80211.h>

/**
 * @brief A simple Decider implementation which uses Threshold decider but also collects statistics
 * of SINR
 *
 * @ingroup Decider
 */
class AdHocWiFiDecider : public Decider80211 {
public:

	enum DeciderMode { Default, Sorin , MinPayload };
protected:

	// 802.11g bitrates
	static long long bitrates[9];

	// a,b,g op mode
	char phyOpMode,channelModel;

	DeciderMode deciderMode;

	/*
	 * threashold of BER
	 */
	double payloadBerThreshold;

	/**
	 * @brief Checks a mapping against a specific threshold (element-wise).
	 *
	 * @return	true	, if every entry of the mapping is above threshold
	 * 			false	, otherwise
	 *
	 *
	 */
	virtual DeciderResult* checkIfSignalOk(Mapping* snrMap, AirFrame* frame);

	// same as checkIfSignalOk in base class but work only base on threshold (no additional packet error modeling)
	DeciderResult* checkIfSignalOkBase(Mapping* snrMap, AirFrame* frame);

	/*
	 * calculate duration of airframe
	 */
	double calculateDuration(AirFrame *airframe,double bitrate);

	/**
	 * check if packet ok according to Sorin methods
	 */
	bool packetOkSorin(double sinrMin, int lengthMPDU, double bitrate);

	double dB2fraction(double dB);

	double countBelowThBer(ConstMapping& m, const Argument& min, const Argument& max,double th,double payloadBitrate);

	/**
	 * external packet error rate
	 */
	double external_PER;

public:

	/**
	 * @brief Initializes the Decider with a pointer to its PhyLayer and
	 * specific values for threshold and sensitivity
	 */
	AdHocWiFiDecider(DeciderToPhyInterface* phy,
			 double threshold,
			 double sensitivity,
			 double centerFrequency,
			 int myIndex = -1,
			 bool debug = false,
			 char phyOpModeP='g',
			 char channelModelP='r',
			 AdHocWiFiDecider::DeciderMode deciderModeP=Default,
			 double payloadBerThresholdP=0.01,
			 double external_PERP=0):
	  Decider80211(phy, threshold,sensitivity, centerFrequency,myIndex, debug),
	  phyOpMode(phyOpModeP),channelModel(channelModelP),
	  deciderMode(deciderModeP),payloadBerThreshold(payloadBerThresholdP),
	  external_PER(external_PERP)
	{}

	virtual ~AdHocWiFiDecider() {};

};

#endif /*AD_HOC_WI_FI_DECIDER_H_*/
