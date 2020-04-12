/*********************************************************
 * File:        AdHocWiFiApplLayer.cpp
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: send messages with random delay and size
 * halts every interval to allow other side to move
 **************************************************************************/


#include "AdHocWiFiApplLayer.h"
#include "NetwControlInfo.h"
#include <SimpleAddress.h>
#include <AdHocWiFiDecider.h>
#include <AdHocWiFiMac.h>
#include <BaseMacLayer.h>
#include "VideoStreamAppl.h"
#include "AdHocWiFiConnectionManager.h"
#include "cconfiguration.h"

#include <iomanip>
#include <fstream>
#include <iostream>
#include <limits>
using namespace std;

Define_Module(AdHocWiFiApplLayer);

// static variable
map<int,map<int,int> > AdHocWiFiApplLayer::networkAddresses;
ScheduleItemCollection * AdHocWiFiApplLayer::schedule;
RequestsCollection * AdHocWiFiApplLayer::requestsReader;
map<int,VideoStreamAppl*> AdHocWiFiApplLayer::applications;
Int2IntMap AdHocWiFiApplLayer::streamDestinations;
Int2IntMap AdHocWiFiApplLayer::streamSources;
unsigned long AdHocWiFiApplLayer::msgID = 0 ;
vector<AdHocWiFiApplLayer::MessagesStatistics> AdHocWiFiApplLayer::messagesStatistics;
double AdHocWiFiApplLayer::slotMaxTime = 0.01;
double AdHocWiFiApplLayer::roundTime = 1;
double AdHocWiFiApplLayer::slotSamplePart = 1.0/SLOT_Q_SAMPLE;
int AdHocWiFiApplLayer::slots = 100;
int AdHocWiFiApplLayer::rounds = 10;
int AdHocWiFiApplLayer::channels = NUMBER_OF_CHANNELS;
int AdHocWiFiApplLayer::msgSizeBits = 16000;
map<int,ofstream*>  AdHocWiFiApplLayer::finalTimes;
bool AdHocWiFiApplLayer::rtpStatistics = true;
bool AdHocWiFiApplLayer::enableArbitration = true;
bool AdHocWiFiApplLayer::oneSlot = true;
AdHocWiFiApplLayer::Mode AdHocWiFiApplLayer::mode = AdHocWiFiApplLayer::VIDEO;
AdHocWiFiApplLayer::StatisticsMode AdHocWiFiApplLayer::statisticsMode = AdHocWiFiApplLayer::Part;
cOutVector * AdHocWiFiApplLayer::schedFromVec=NULL;
cOutVector * AdHocWiFiApplLayer::schedToVec=NULL;
cOutVector * AdHocWiFiApplLayer::schedSlotVec=NULL;
cOutVector * AdHocWiFiApplLayer::schedChannelVec=NULL;
cOutVector * AdHocWiFiApplLayer::schedStreamVec=NULL;
cOutVector * AdHocWiFiApplLayer::schedDataVec=NULL;
cOutVector * AdHocWiFiApplLayer::schedMcsVec=NULL;

int AdHocWiFiMsg::s_cnt = 0;

// do some initialization , take variables from omnetpp initialization script
void AdHocWiFiApplLayer::initialize(int stage)
{
	sscanf(getFullPath().c_str(),"sim.channel%d_node",&channel);
	nodeID = myApplAddr();
    currSlot = 0;
    currRound = 1;
    BaseApplLayer::initialize(stage);
    prevSlotCnt = 0;
    active = false;
    if (stage==0){

        if (hasPar("rtpStatistics"))
        	rtpStatistics = par("rtpStatistics");
        else
        	rtpStatistics = false;

	if (!par("adjustRoundTime")) {
	  if (hasPar("slots"))
        	slots = par("slots");
	  else
        	slots = 100;
	}
        if (hasPar("slotMaxTime"))
        	slotMaxTime = par("slotMaxTime");
        else
        	slotMaxTime = 0.01;

	if (hasPar("one_slot"))
          oneSlot = par("one_slot");
	else
          oneSlot = 0;
    

	if (oneSlot) {
       	  slotMaxTime = slotMaxTime*slots;
       	  slots = 1;
        }


        if (hasPar("rounds"))
        	rounds = par("rounds");
        else
        	rounds = 10;

        if(hasPar("channels"))
        	channels = par("channels");
        else
        	channels = NUMBER_OF_CHANNELS;
        assert(channels==NUMBER_OF_CHANNELS);

        if (hasPar("mode"))
        	mode = (string(par("mode").stringValue())==string("video")) ? VIDEO : RAW;
        else
        	mode = VIDEO;

        if (hasPar("statisticsMode"))
        	statisticsMode = (string(par("statisticsMode").stringValue())==string("full")) ? Full : Part;
        else
        	statisticsMode = Full;

        if (hasPar("enableArbitration"))
        	enableArbitration = (par("enableArbitration"));
        else
        	enableArbitration = false;

        if (schedule==NULL) {

        	// done once
        	EV << "AdHocWiFi simulation " << AD_HOC_WI_FI_MAJOR << "." << AD_HOC_WI_FI_MINOR << endl;
        	cerr << "AdHocWiFi simulation " << AD_HOC_WI_FI_MAJOR << "." << AD_HOC_WI_FI_MINOR << endl;
        	//

           schedule = new ScheduleItemCollection();
           // read schedule of tasks
           const char * scheduleFileName;
           if (hasPar("schedule_file"))
        	   scheduleFileName = par("schedule_file");
           else
        	   scheduleFileName = "schedule.txt";
           if (!schedule->readFromFile(scheduleFileName))
        	      endSimulation();
           if (par("adjustRoundTime")) {
	          slots = schedule->getMaxSlot();
              roundTime = slotMaxTime*slots;
	       }
           slotSamplePart = roundTime/SLOT_Q_SAMPLE;
        }
        tasks = schedule->getSenderSchedule(nodeID);

        if (requestsReader==NULL) {
        	requestsReader = new RequestsCollection();
            const char * requestFileName;
            if (hasPar("requests_file"))
            	requestFileName = par("requests_file");
            else
            	requestFileName = "Requests.txt";
        	if (!requestsReader->InitFromFile(requestFileName))
        	      endSimulation();
        }

        std::vector<CommunicationRequest *> requests = requestsReader->GetRequests();
        FOR_EACH(std::vector<CommunicationRequest *>,reqIter,requests) {
        	streamDestinations[(*reqIter)->GetRequestNum()] = (*reqIter)->GetFirstDestination();
        	if (mode==VIDEO && channel==1 && nodeID==0) {
        		stringstream sstr;
        		sstr << "results/node" << (*reqIter)->GetFirstDestination() << "_str" << (*reqIter)->GetRequestNum() << ".txt";
        		finalTimes[(*reqIter)->GetRequestNum()] = new ofstream(sstr.str().c_str());
        		if (!finalTimes[(*reqIter)->GetRequestNum()]->is_open()) {
        			cerr << "Unable to open times file " << sstr.str().c_str() << endl;
        			endSimulation();
        		}
        	}
        	streamSources[(*reqIter)->GetRequestNum()] = (*reqIter)->GetSource();
        	if ((*reqIter)->GetSource()==nodeID) {
        		initiatorStreams.insert((*reqIter)->GetRequestNum());
        		EV << nodeID << " initiator of " << (*reqIter)->GetRequestNum() << endl;
        	}
        }

    	int rtpNumber = 1;
        if(hasPar("rtpNumber"))
        	rtpNumber = par("rtpNumber");
    	const char * rtpName = "camera";
        if(hasPar("rtpName"))
	      rtpName = par("rtpName");
        if (channel==1) {
        	VideoStreamAppl * vidStreamAppl = new VideoStreamAppl(nodeID,rtpName,rtpNumber);
        	applications[nodeID] = vidStreamAppl;
        	vidStreamAppl->initialize();
        }
    }

    queueSampleOffset = 1;
    idleSlots = 0;
    if (stage == 0) {
        slotMsg = new cMessage( "new-slot", MSG_NEW_SLOT );
        transmitMsg = new cMessage( "transmit-packet", MSG_TRANSMIT );
    }
    else if (stage==1) {
        msgSizeBits = headerLength;
        int addr = getParentModule()->getSubmodule("net")->getId();
        networkAddresses[nodeID][channel] = addr;

    	const char * rtpName = "camera";
        if(hasPar("rtpName"))
	      rtpName = par("rtpName");
    	applications[nodeID]->addChannel(this,channel,tasks,requestsReader->GetRequests(),rtpName);
        scheduleAt(simTime(), slotMsg);
        if (channel==NUMBER_OF_CHANNELS) {
        	FOR_EACH(IntSet,initIt,initiatorStreams) {
        		char name[20];
        		sprintf(name,"create-packet-%d",*initIt);
        		cMessage * inputMsg = new cMessage( name, MSG_INPUT );
         	    if (setCameraEvent(*initIt,inputMsg))
         	    	inputControlMessages[*initIt-1] = inputMsg;
         	    else cancelAndDelete(inputMsg);
        	}
        }
        if (nodeID==0 && channel==1) { // once

        	cOutVector sourcesVec("Sources");
        	cOutVector destinationsVec("Destinations");
        	cOutVector slotsVec("Slots");
        	cOutVector roundTimeVec("roundTime");
        	cOutVector roundsVec("Rounds");
        	roundsVec.recordWithTimestamp((simtime_t)1,rounds);
        	slotsVec.recordWithTimestamp((simtime_t)1,slots);
        	roundTimeVec.recordWithTimestamp((simtime_t)1,roundTime);
        	cOutVector msgSizeVec("MsgSizeBits");
        	msgSizeVec.recordWithTimestamp((simtime_t)1,headerLength);
        	for (unsigned i=1;i<=streamSources.size();i++) {
				sourcesVec.recordWithTimestamp((simtime_t)i,streamSources[i]);
        		destinationsVec.recordWithTimestamp((simtime_t)i,streamDestinations[i]);
        	}
        }
        if (schedFromVec==NULL) {
        	schedFromVec = new cOutVector("Table_From");
        	schedToVec = new cOutVector("Table_To");
        	schedSlotVec = new cOutVector("Table_Slot");
        	schedChannelVec = new cOutVector("Table_Channel");
        	schedStreamVec = new cOutVector("Table_Stream");
        	schedMcsVec = new cOutVector("Table_MCS");
        	schedDataVec = new cOutVector("Table_Data");
        }
		int i = schedFromVec->getValuesStored()+1;
		if (channel==1)
			FOR_EACH_C(ScheduleItemCollection::Stream2SchedulesMap,st2scIt,tasks) {
				FOR_EACH_C(ScheduleItemCollection::NodeSchedules,nScIt,st2scIt->second) {
					ScheduleItemSPtr entry = *nScIt;
					schedFromVec->recordWithTimestamp((simtime_t)i,entry->GetSender());
					schedToVec->recordWithTimestamp((simtime_t)i,entry->GetReciever());
					schedSlotVec->recordWithTimestamp((simtime_t)i,entry->GetTimeSlot());
					schedChannelVec->recordWithTimestamp((simtime_t)i,entry->GetFreqNum());
					schedStreamVec->recordWithTimestamp((simtime_t)i,entry->GetRequestNum());
					schedMcsVec->recordWithTimestamp((simtime_t)i,entry->GetMCS());
					schedDataVec->recordWithTimestamp((simtime_t)i,entry->GetFlowInSlot());
					i++;
				}
			}
    }
}

bool AdHocWiFiApplLayer::setCameraEvent(int stream,cMessage * msg)
{
	void * msgContext = msg->getContextPointer();
	if (msgContext) {
		ApplPkt * applMsg = (ApplPkt*) msgContext;
	    applications[nodeID]->addMessage(stream,applMsg);
	}
	unsigned bits  = headerLength;
	ApplPkt * applMsg = applications[nodeID]->getInitiatorMessage(stream,0,0,bits,msgID);
    if (applMsg) {
      msgID++;
      messagesStatistics.push_back(MessagesStatistics(stream)); // add to statistics
      AdHocWiFiMsg msgDetails(stream,0,0,nodeID,nodeID,0,applMsg);
      applMsg->setName(msgDetails.header);
      msg->setContextPointer((void*)applMsg);
    }
    if (mode==VIDEO) {
    	simtime_t arrival = applMsg->getArrivalTime();
    	if (arrival<simTime())
    		arrival = simTime() + 0.000001;
		scheduleAt(arrival,msg);
		return true;
    }
    else {
    	const VideoStreamAppl::Stream *streamPtr = applications[nodeID]->getStream(stream);
    	double r = (stream) ?  streamPtr->getTotalOutRate() : requestsReader->GetRequests()[stream-1]->GetDemand()*1E6;
    	if (r) {
    		double time_ofs = bits/r*slotMaxTime*slots;
    		scheduleAt(simTime()+time_ofs,msg);
    		return true;
    	}
    }
    return false;
}

void AdHocWiFiApplLayer::changeSlot()
{
  currSlot++;
  if (nodeID==1 && channel==1) {
    EV << "Switching to slot " << currSlot << " in round " << currRound << endl;
    if (currSlot==1 && currRound==1) {
      AdHocWiFiConnectionManager* w = FindModule<AdHocWiFiConnectionManager*>::findGlobalModule();
      w->checkConnections();
      FOR_EACH(ApplicationsMap,apIter,applications)
        apIter->second->initStartRound(true);
    }
  }
  if (slots>1 && channel==1) {
	  int slotPart = slots/SLOT_Q_SAMPLE;
	  if (currSlot % slotPart==0) {
	    int offset = currSlot / slotPart;
	    if (offset!=SLOT_Q_SAMPLE)
	    	applications[nodeID]->sampleQueueLength(currRound,offset);
	  }
  }
  if (currSlot <= slots)
    scheduleAt(simTime() + slotMaxTime, slotMsg);
  else {
    if (currRound < rounds) {
      if (channel==1)
	    applications[nodeID]->endOfRound(currRound);
      currSlot = 1; // start over
      queueSampleOffset = 1;
      scheduleAt(simTime() + slotMaxTime, slotMsg);
      currRound++;
    }
    else
      endSimulation();
  }

  // initiate the task mode
  hasTask = false;
  currentTasks.clear();
  applications[nodeID]->checkAndSetTask(currSlot,channel);

}

double AdHocWiFiApplLayer::getSlotPart()
{
	double now = SIMTIME_DBL(simTime());
	while (now>roundTime) {
		now-=roundTime;
	}
	return (roundTime-now)/roundTime;
}

void AdHocWiFiApplLayer::handleSelfMsg(cMessage *msg)
{
	const char * name = msg->getName();
	switch(msg->getKind())
	  {
	  case MSG_NEW_SLOT:
	    changeSlot();
	    break;
	  case MSG_TRANSMIT:
		if (slots==1 && channel==1) { // perform queue sampling if in one slot
              double part = 1-getSlotPart();
              if ((part>slotSamplePart*queueSampleOffset))
  			    applications[nodeID]->sampleQueueLength(currRound,queueSampleOffset++);
		}
	    if (hasTask)
	      transmitMessage();
	    break;
	  case MSG_INPUT:
	    int stream;
	    sscanf(name,"create-packet-%d",&stream);
	    setCameraEvent(stream,msg);
	    break;
	  default:
	    EV <<" Unknown self message! -> delete, kind: "<<msg->getKind()<<endl;
	  }
}

void AdHocWiFiApplLayer::handleLowerMsg( cMessage* msg )
{
    ApplPkt *m;
    switch( msg->getKind() ){
    case MSG_VID_STREAM:
        m = static_cast<ApplPkt *>(msg);
       	if (m->getDestAddr()==nodeID) { // only for me
       		AdHocWiFiMsg msgDetails(m->getName());
       		applications[nodeID]->addMessage(msgDetails.stream,msg);
       		if (streamDestinations[msgDetails.stream]==nodeID) {// final destination
       			FOR_EACH(IntList,idsIter,msgDetails.IDs)  // mark as receive
					messagesStatistics[*idsIter].lastRecieve = simTime();
       			if (mode==VIDEO)
       				FOR_EACH(StringList,idsIter,msgDetails.dataIDs)
						(*finalTimes[msgDetails.stream]) << *idsIter << "@" << simTime() << endl;
       		}
       		if (statisticsMode == Full)
       			msgDetails.recordLog(AdHocWiFiMsg::Receive);
       	}
       	else delete msg;
        break;
    default:
	EV <<"Error! got packet with unknown kind: " << msg->getKind()<<endl;
        delete msg;
    }
}

void AdHocWiFiApplLayer::txOver()
{
	Enter_Method("tx-over"); // needed for simulator
	if (!active)
		return; // ack messages and etc.
	if (!hasTask) {
		active = false;
		return;
	}
	if (transmitMsg->isScheduled())
		cancelEvent(transmitMsg);
	scheduleAt(simTime()+0.000000001, transmitMsg); // schedule new message , advance some delta time from current
}

void AdHocWiFiApplLayer::setCurrentTask(const ScheduleItemSPtr task)
{
	Enter_Method("set-current-task");

	int rate=AdHocWiFiMac::getAdjustedRate(task);
	task->SetMCS(rate);

	currentTasks.push_back(make_pair(0,task));
	hasTask = true;
	if (active) { // check if lost in tx
		if ((prevSlotCnt==cnt) && (idleSlots++==2)) { // no send in last slots
			active = false;
			DEBUG_OUT << "No send in round = " << currRound << " slot=" << currSlot << " for node " << nodeID << " cnt = " << cnt << endl;
		}
	}
	prevSlotCnt = cnt;
	if (!active)
		transmitMessage();
}

ScheduleItemSPtr AdHocWiFiApplLayer::getArbitrarlyTask()
{
  double maxWeight = 0;
  ActiveTasks::iterator maxWeightTask;
  FOR_EACH(ActiveTasks,it,currentTasks) {
    double weight = it->second->GetFlowInSlot()/(it->first+1);
    if (maxWeight<=weight) {
      maxWeight = weight;
      maxWeightTask = it;
    }
  }
  maxWeightTask->first++;
  return maxWeightTask->second;
}

/**
 * This function creates a new test message and sends it down to
 * the network layer
 **/
void AdHocWiFiApplLayer::transmitMessage()
{
  if (enableArbitration) // send one message from existing according to weight
    transmitMessage(getArbitrarlyTask());
  else {  // send all of the tasks messages
    FOR_EACH(ActiveTasks,it,currentTasks) {
      transmitMessage(it->second);
    }
  }
}

void AdHocWiFiApplLayer::delayMessage(int rate)
{
	Enter_Method("delay-Message");
	cancelEvent(transmitMsg);
	double packetTime = AdHocWiFiMac::getActualBitrate(rate,headerLength);
	packetTime = headerLength/packetTime;
	scheduleAt(simTime()+packetTime+1, transmitMsg);
}

/**
 * This function creates a new test message and sends it down to
 * the network layer
 **/
void AdHocWiFiApplLayer::transmitMessage(ScheduleItemSPtr task)
{
  if (!hasTask) {
    active = false;
    return;
  }

  int rate = task->GetMCS();
 

  // adjust actual size based on actual rate
  long bits = headerLength;
#ifdef MSG_SIZE_PER_MCS
  double a = AdHocWiFiMac::bitrates[8]/AdHocWiFiMac::bitrates[rate];
  bits /= a;
#endif
    ApplPkt * msg = applications[nodeID]->getMessage(task,currSlot,channel,bits);
	active = true;
	if (msg==NULL) {
		if (oneSlot==true) {
			delayMessage(rate);
		}
		else
			active = false;
		return;
	}
	DEBUG_MSG(nodeID << " Sending message " << simTime() << " str#" << task->GetRequestNum() << " msg=" << msg->getFullName() << endl;)
	idleSlots = 0;
	int dest = task->GetReciever();
	AdHocWiFiMsg msgTitle(task->GetRequestNum(),task->GetFreqNum(),rate,nodeID,dest,currSlot,msg);
	cnt = msgTitle.cnt;
	if (statisticsMode==Full)
	  msgTitle.recordLog(AdHocWiFiMsg::Transmit);

	msg->setName(msgTitle.header);
	// the dest is both L2 and L3 since app/mac use same address for simplicity
    msg->setDestAddr(dest);
    // we use the host modules getIndex() as a appl address
    msg->setSrcAddr( myApplAddr());

    // set the control info to tell the network layer the layer 3
    // address;
    msg->setControlInfo( new NetwControlInfo(networkAddresses[dest][channel] /*L3BROADCAST*/) );

    sendDown( msg );

}

void AdHocWiFiApplLayer::changeRate(int stream,int rate,int fps)
{
	if (mode==VIDEO)
	  (*finalTimes[stream]) << "#" << rate << "_" << 10+fps*5 << endl;
}


void AdHocWiFiApplLayer::finishGlobal()
{
	int lost = 0, dropped = 0 , inQ = 0;
	vector<cOutVector *> tx,rx,send;

	delete schedFromVec;
	delete schedToVec;
	delete schedSlotVec;
	delete schedChannelVec;
	delete schedStreamVec;
	delete schedDataVec;
	delete schedMcsVec;
	FOR_EACH(ApplicationsMap,appIter,applications)
		appIter->second->finishSerial();
	int i = 0;
	FOR_EACH(vector<MessagesStatistics>,msgIter,messagesStatistics) {
		i++;
		while (msgIter->stream >= (int) send.size()) {
			int stream = send.size();
			char titleStr[MAX_HEADER_SIZE];
			sprintf(titleStr, "Packet's End2End delay str%d(%d->%d)[sec]",
					stream, streamSources[stream], streamDestinations[stream]);
			send.push_back(new cOutVector(titleStr));
		}
		// notice timestamp here is simply serial and not real and done only for formatting
		int index = send[msgIter->stream]->getValuesReceived()+1;
		double e2e = SIMTIME_DBL(msgIter->lastRecieve - msgIter->initialSend);
		if (e2e>=0)
		  //e2e = numeric_limits<double>::quiet_NaN();
		  send[msgIter->stream]->recordWithTimestamp((simtime_t) index, e2e);
		if (msgIter->lastRecieve == PKT_LOST)
		  lost++;
		else if (msgIter->lastRecieve == PKT_IN_QUEUE)
		  inQ++;
		else if (msgIter->lastRecieve == PKT_DROPPED)
		  dropped++;
		
	}
	FOR_EACH (vector<cOutVector *>,it,tx)
		delete *it;
	FOR_EACH (vector<cOutVector *>,it,rx)
		delete *it;
	FOR_EACH (vector<cOutVector *>,it,send)
		delete *it;
    for (map<int,ofstream*>::iterator it = finalTimes.begin(); it != finalTimes.end(); it++) {
		it->second->close();
		delete it->second;
	}
	finalTimes.clear();
	AdHocWiFiMsg::log.close();
	DEBUG_OUT<< "\nTotal " << lost << " messages are lost, " << inQ << " messages in queues and " << dropped <<" messages are dropped.\n";
}

void AdHocWiFiApplLayer::finish()
{
	BaseApplLayer::finish();
	cancelAndDelete(slotMsg);
	cancelAndDelete(transmitMsg);
	FOR_EACH(Int2MessageCollection,it,inputControlMessages) {
		cancelAndDelete(it->second);
	}
    if (channel==1) { // only once per node
		applications[nodeID]->finish();
		if (nodeID==1) // only once globally
			finishGlobal();
    }
}

AdHocWiFiMsg::AdHocWiFiMsg(const char * headerP)
	: header(NULL)
{
	char idsStr[MAX_HEADER_SIZE];
	if (!sscanf(headerP,"stream=%d,channel=%d,rate=%d,%d to %d,slot=%d,cnt=%d,IDs=%s",&stream,&channel,&rate,&from,&to,&slot,&cnt,idsStr))
		stream = -1;
	else decodeIDs(idsStr);
}

AdHocWiFiMsg::AdHocWiFiMsg(int stream,int channel,int rate,int from,int to,int slot,ApplPkt * msg)
	:stream(stream),channel(channel),rate(rate),from(from),to(to),slot(slot),header((char*)malloc(MAX_HEADER_SIZE))
{
	char idsStr[MAX_HEADER_SIZE];
	if (msg) {
	  sprintf(idsStr,"%s",msg->getName());
		decodeIDs(idsStr);
	}
	else sprintf(idsStr,"?");
	cnt = s_cnt;
	sprintf(header,"stream=%d,channel=%d,rate=%d,%d to %d,slot=%d,cnt=%d,IDs=%s",stream,channel,rate,from,to,slot,s_cnt++,(char*)idsStr);
}

void AdHocWiFiMsg::decodeIDs(string idsStr)
{
	string::size_type pos = 0;
	do {
		string numStr = idsStr;
		pos = idsStr.find(",");
		if (pos!=string::npos) {
			numStr = idsStr.substr(0,pos);
		}
		string::size_type colon = numStr.find(":");
		string dataStr = numStr.substr(colon+1);
		numStr = numStr.substr(0,colon);
		IDs.push_back(atoi(numStr.c_str()));
		dataIDs.push_back(dataStr);
		if (pos!=string::npos)
			idsStr = idsStr.substr(pos+1);
	} while (pos!=string::npos);
}


void AdHocWiFiMsg::decodeMsg(const char * header,string& name)
{
	if (name.length()>0)
		name += ",";
	string s = header;
	string::size_type p = s.find(",IDs=");
	assert(p!=string::npos);
	name += s.substr(p+5);
}

ofstream AdHocWiFiMsg::log;

void AdHocWiFiMsg::recordLog(AdHocWiFiMsg::EventType evt) const
{
	if (!log.is_open()) {
		log.open(string("results/"+string(cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName()) + "_log.csv").c_str());
		log << "Message ID,from,to,stream,slot,channel,SINR,Event__0_Recv__1_Send__2_Drop__3_Lost__4_Rate,Time\n";
	}
	FOR_EACH_C(IntList,idIter,IDs) {
	  double sinr = 0;
	  if (*idIter>=0)
		  sinr = AdHocWiFiApplLayer::messagesStatistics[*idIter].lastSINR;
	  if (sinr!=sinr)
		  sinr = 0; // NaN
	  else if ((sinr<9e99)&&(sinr>-9e99))
		  sinr = sinr; // normal
	  else
		  sinr = 0; // inf
	  double time = SIMTIME_DBL(simTime());
	  log << *idIter << "," <<  from << "," << to << "," << stream << "," << slot << "," << channel << "," << setprecision (6)  << sinr << "," << evt << "," << setprecision (6) << time << endl;
	}
}
