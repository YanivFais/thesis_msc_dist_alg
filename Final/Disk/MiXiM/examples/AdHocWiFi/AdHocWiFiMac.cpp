/*********************************************************
 * File:        AdHocWiFiMac.cpp
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: media access control layer for AdHocWiFi
 **************************************************************************/

#include "AdHocWiFiMac.h"
#include "AdHocWiFiApplLayer.h"
#include "NetwControlInfo.h"
#include <SimpleAddress.h>
#include "MacToPhyControlInfo.h"
#include <PhyToMacControlInfo.h>
#include <Decider80211.h>
#include <DeciderResult80211.h>

Define_Module(AdHocWiFiMac);

map<int,int> AdHocWiFiMac::mac2applAddr;
AdHocWiFiMac::EdgeToSnrMap AdHocWiFiMac::snrs;
bool AdHocWiFiMac::adjustMCS = false;

AdHocWiFiMac::AdHocWiFiMac() : Mac80211g() {

}

AdHocWiFiMac::~AdHocWiFiMac() {
}

void AdHocWiFiMac::initialize(int stage)
{
	if (hasPar("per2snr_filename"))
      per2snr_file_name = par("per2snr_filename").stdstringValue();

	if (hasPar("adjustMCS"))
		adjustMCS = par("adjustMCS").boolValue();
	else adjustMCS = false;

	Mac80211g::initialize(stage);

	if (stage==1) {
		cModule * nic = getParentModule();
		cModule * node = nic->getParentModule();
		cModule * app = node->getSubmodule("appl");
		appl = check_and_cast<AdHocWiFiApplLayer*>(app);
		mac2applAddr[myMacAddr] = appl->getNodeID();
		static bool first = true;
		if (first) {
			cOutVector txp("TX_POWER_mW");
			txp.recordWithTimestamp((simtime_t)1,txPower);
			first = false;
		}
	}

}

// for 802.11g - MCS - rate can use 6,9,12,18,24,36,48,54
long long AdHocWiFiMac::bitrates[9] = {-1,6000000,9000000,12000000,
		18000000,24000000,36000000,
		48000000,54000000 };

// for 802.11g - MCS - actual rate (based on calculation for PER=0) per different MCS
// calculated for default parameters used for 802.11g only
unsigned int AdHocWiFiMac::getActualBitrate(int mcs,int packetLength)
{
  int bitrate = bitrates[mcs];
  return (packetLength/((656+packetLength)/(bitrate/1E6)+305))*1E6;
}


void AdHocWiFiMac::handleUpperMsg(cMessage *msg)
{
  Mac80211g::handleUpperMsg(msg);
}

void AdHocWiFiMac::beginNewCycle()
{

  if (timeout->isScheduled())
    cancelEvent(timeout);

  if (!nav->isScheduled() && !fromUpperLayer.empty()) {
      const char * fn = fromUpperLayer.front()->getFullName();
      
      if (fn[0]!='w') {
    	  AdHocWiFiMsg msg(fn);
    	  if (msg.stream>=0) { // valid, set the sending rate
    		  currMsgDataBitrate = bitrates[msg.rate];
    		  defaultBitrate = currMsgDataBitrate;
    	  }
      }
  }
  Mac80211g::beginNewCycle();

  //  DEBUG_MSG(appl->getNodeID() << " beggining new cycle " << " state=" << state << " upper msg#=" << fromUpperLayer.size() << " @" << simTime() << endl);
  if (state==IDLE && fromUpperLayer.empty()) {
    //int radioState = phy->getRadioState();
    // if in rx and message for me and waiting for ack and got ack then done with message
    // signal for another message
    //if (radioState == Radio::RX) {
      appl->txOver();
      //}
  }

}

/**
 *  Send a DATA frame. Called by HandleEndSifsTimer() or
 *  handleEndContentionTimer() - copied from Mac80211 for changing data bitrate
 */
void AdHocWiFiMac::sendDATAframe(Mac80211Pkt *af)
{

    Mac80211Pkt *frame = static_cast<Mac80211Pkt *>(fromUpperLayer.front()->dup());
    double br;

    if(af) {
    	PhyToMacControlInfo* tmp = static_cast<PhyToMacControlInfo*>(af->removeControlInfo());

        br = currMsgDataBitrate; //static_cast<const DeciderResult80211*>(tmp->getDeciderResult())->getBitrate();
        delete tmp;
    }
    else {
       br  = retrieveBitrate(frame->getDestAddr());
       if(shortRetryCounter) frame->setRetry(true);
    }
    simtime_t duration = packetDuration(frame->getBitLength(), br);
    Signal* signal = createSignal(simTime(), duration, txPower, br);
    MacToPhyControlInfo *pco = new MacToPhyControlInfo(signal);
    // build a copy of the frame in front of the queue'
    frame->setControlInfo(pco);
    frame->setSrcAddr(myMacAddr);
    frame->setKind(DATA);
    frame->setDuration(SIFS + packetDuration(LENGTH_ACK, br));

    // schedule time out
    if (timeout->isScheduled())
    	cancelEvent(timeout);
    scheduleAt(simTime() + timeOut(DATA, br), timeout);
    EV << "sending DATA  to " << frame->getDestAddr() << " with bitrate " << br << endl;
    // send DATA frame
    sendDown(frame);

    // update state and display
    setState(WFACK);
}

int AdHocWiFiMac::getAdjustedRate(ScheduleItemSPtr task)
{
	if (adjustMCS) {
	  pair<int,int> edge = make_pair(task->GetSender(),task->GetReciever());
	  EdgeToSnrMap::const_iterator sit = snrs.find(edge);
	  int rate=1;
	  if (sit!=snrs.end()) {
			vector<PER_FOR_MCS *> * per4mcsVec = PER2SNRTableObj->GetMCSAndPER(FWMath::mW2dBm(sit->second));
			for (vector<PER_FOR_MCS *>::const_iterator it=per4mcsVec->begin(); it != per4mcsVec->end(); it++) {
				if ((*it)->PER<0.05)
					rate = (*it)->MCS_INDEX;
				//cout << " PER:" << (*it)->PER << " MCS:" << (*it)->MCS_INDEX << endl;
				delete *it;
			}
			delete per4mcsVec;
		    //cout << "SNR:" << sit->second << " " << task->GetMCS() << ":" << rate << endl;
		    return rate;
	  }
	}
	return task->GetMCS();
}

void AdHocWiFiMac::handleLowerMsg(cMessage *msg)
{
  Mac80211Pkt *af = dynamic_cast<Mac80211Pkt *>(msg);

  if (adjustMCS) {
	  if (af->getDestAddr()==myMacAddr) { // messsage is for me!
			const DeciderResult80211* result =
			  static_cast<const DeciderResult80211*>(
								 static_cast<PhyToMacControlInfo *>(
												af->getControlInfo())->getDeciderResult());
			pair<int,int> edge = make_pair(mac2applAddr[af->getSrcAddr()],mac2applAddr[af->getDestAddr()]);
			snrs[edge] = result->getSnr();
	  }
  }

  if ((msg->getKind()!=ACK) && (state==WFACK) && (AdHocWiFiApplLayer::oneSlot)) {
	  msg->setKind(ACK); // special mode for one-slot to  avoid errors
  }
  if ((msg->getKind()==RTS) && (af->getDestAddr()==myMacAddr)) {
    // make sure sending CTS in the rate in which recieved from transmitter
    const DeciderResult80211* result = 
      static_cast<const DeciderResult80211*>(
					     static_cast<PhyToMacControlInfo *>(
										af->getControlInfo())->getDeciderResult());
    
    defaultBitrate = currMsgDataBitrate = result->getBitrate();
  }
  Mac80211g::handleLowerMsg(msg);
}

void AdHocWiFiMac::handleLowerControl(cMessage *msg)
{
 	//if (msg->getKind()== Decider80211::COLLISION || msg->getKind()==Decider80211::BITERROR)
	/*Mac80211Pkt *af = dynamic_cast<Mac80211Pkt *>(msg);
	int forme=0;
	if (af)
	  forme =  af->getDestAddr()==myMacAddr;*/
  Mac80211g::handleLowerControl(msg);
}


void AdHocWiFiMac::delayMsg(int rate)
{
	Enter_Method("delay-Message");
	if (nav->isScheduled())
		cancelEvent(nav);
	double packetTime = getActualBitrate(rate,appl->getMessageSizeBits());
	packetTime = appl->getMessageSizeBits()/packetTime - 0.0002F;
	scheduleAt(simTime()+packetTime, nav);
    setState(QUIET);
}

