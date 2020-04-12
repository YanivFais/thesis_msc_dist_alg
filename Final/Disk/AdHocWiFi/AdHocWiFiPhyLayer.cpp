/*********************************************************
 * File:        AdHocWiFiPhyLayer.cpp
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: physical layer implementation
 **************************************************************************/

#include "AdHocWiFiPhyLayer.h"
#include "AdHocWiFiDecider.h"
#include "AdHocWiFiApplLayer.h"
#include "AdHocWiFiMac.h"

Define_Module(AdHocWiFiPhyLayer);


void AdHocWiFiPhyLayer::initialize(int stage)
{
  if (stage==1) {
    string config = cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName();
    if ((config!= "Main") && (config!="MultiCommFlowSched") && (config!="ShortestPathSched")) { // if no scheduler then lower  sensitivity
      sensitivity = ((double)par("sensitivity"))-15;
      sensitivity = FWMath::dBm2mW(sensitivity);    
    }
  }
  PhyLayer::initialize(stage);
}

AnalogueModel* AdHocWiFiPhyLayer::getAnalogueModelFromName(std::string name, ParameterMap& params)
{

	if(name == "AdHocWiFiPathLossModel")
	{

		double alpha = (double) par("pathloss_alpha");
		double beta = (double) par("pathloss_beta");
		double c = (double) par("pathloss_c");
		double htr = (double) par("pathloss_htr");
		double hrc = (double) par("pathloss_hrc");
		static bool once = false;
		if (!once) {
			cOutVector alphaVec("PathLossAlpha");
			alphaVec.recordWithTimestamp((simtime_t)1,alpha);
			cOutVector betaVec("PathLossBeta");
			betaVec.recordWithTimestamp((simtime_t)1,beta);
			cOutVector noiseVec("noise_dBm");
			noiseVec.recordWithTimestamp((simtime_t)1,par("thermalNoise").doubleValue());
			once = true;
		}

		AdHocWiFiPathLossModel::Model model = AdHocWiFiPathLossModel::ELBIT;
		if (hasPar("pathloss_model"))
        	model = (string(par("pathloss_model").stringValue())==string("AlphaBeta")) ? AdHocWiFiPathLossModel::ALPHA_BETA : AdHocWiFiPathLossModel::ELBIT;
		return new AdHocWiFiPathLossModel(&move, alpha , beta,c,htr,hrc,model);
	}

	return PhyLayer::getAnalogueModelFromName(name, params);
}

Decider* AdHocWiFiPhyLayer::getDeciderFromName(std::string name, ParameterMap& params) {

	if(name == "AdHocWiFiDecider"){
		double threshold = params["threshold"];
//		double centerFreq = params["centerFrequency"];
		double centerFreq;
		if(hasPar("centerFrequency"))
			centerFreq = par("centerFrequency");
		else {
			cerr << "No centerFrequency defined for AdHocWiFiPhyLayer" << endl;
			endSimulation();
		}
		double payloadBerThreshold = params["payloadBerThreshold"];
		const char * p = params["phyOpMode"];
		const char * c = params["channelModel"];
		const char * m = params["deciderMode"];
		double external_PER = (double) par("external_PER");
		string phyOpModeStr = p;
		string channelModelStr = c;
		string deciderModeStr = m;
		AdHocWiFiDecider::DeciderMode deciderMode = AdHocWiFiDecider::Default;
		if (deciderModeStr=="Mixim")
			deciderMode = AdHocWiFiDecider::Default;
		else if (deciderModeStr=="Sorin")
			deciderMode = AdHocWiFiDecider::Sorin;
		else if (deciderModeStr=="MinPayload")
			deciderMode = AdHocWiFiDecider::MinPayload;
		else EV << "Decider mode not recognized " << deciderMode << ", using default Mixim" << endl;
		char phyOpMode = (phyOpModeStr=="g" ? 'g' : 'b');
		char channelModel = (channelModelStr=="r" ? 'r' : 'a');
		return new AdHocWiFiDecider(this, threshold, sensitivity, centerFreq, findHost()->getIndex(),
				coreDebug,phyOpMode,channelModel,deciderMode,payloadBerThreshold,external_PER);
	}
	return PhyLayer::getDeciderFromName(name,params);
}

void AdHocWiFiPhyLayer::handleMessage(cMessage* msg)
{
	BasePhyLayer::handleMessage(msg);
}

