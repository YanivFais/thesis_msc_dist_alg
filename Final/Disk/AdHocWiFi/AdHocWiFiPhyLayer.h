/*********************************************************
 * File:        AdHocWiFiPhyLayer.h
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: physical layer implementation
 **************************************************************************/

#ifndef AD_HOC_WI_FI_PHYLAYER_H_
#define AD_HOC_WI_FI_PHYLAYER_H_

#include <PhyLayer.h>
#include "AdHocWiFiPathLossModel.h"

/**
 * Phy layer of each node
 */
class AdHocWiFiPhyLayer: public PhyLayer {
protected:

	/**
	 * @brief OMNeT++ initialization function.
	 * Read simple parameters.
	 * Read and parse xml file for decider and analogue models
	 * configuration.
	 */
	virtual void initialize(int stage);

	virtual void handleMessage(cMessage* msg);

	virtual AnalogueModel* getAnalogueModelFromName(std::string name, ParameterMap& params);

	virtual Decider* getDeciderFromName(std::string name, ParameterMap& params);
};

#endif
