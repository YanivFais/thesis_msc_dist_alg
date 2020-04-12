/*********************************************************
 * File:        VideoStreamAppl.cpp
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: top application layer for node,
 * contains the sending AdHocWiFiApplLayer objects which are used for communication per channel (frequency)
 **************************************************************************/


#include "VideoStreamAppl.h"
#include "NetwControlInfo.h"
#include <SimpleAddress.h>
#include <AdHocWiFiDecider.h>
#include <AdHocWiFiMac.h>
#include <AdHocWiFiApplLayer.h>
#include <AdHocWiFiMobility.h>
#include <BaseMacLayer.h>
#include <sstream>
#include <limits>
using namespace std;


// static variables
vector<VideoStreamAppl*> VideoStreamAppl::apps;
vector<cOutVector*> VideoStreamAppl::throughput;
VideoStreamAppl::InitiatorsData VideoStreamAppl::initiatorsData;
unsigned VideoStreamAppl::appsEnded = 0;

VideoStreamAppl::VideoStreamAppl(int id,const string& rtpName,int rtpNumber)
	 : nodeID(id)
{
	 while (apps.size()<=(unsigned)id)
		 apps.push_back(NULL);
	apps[id] = this;

	if (id==0) {
		if (AdHocWiFiApplLayer::mode==AdHocWiFiApplLayer::VIDEO) {
			RtpPerRate * rtp = new RtpPerRate();
			for (int i=1;i<=rtpNumber;i++) {
				RtpPerFPS * rtpfps = new RtpPerFPS();
				for (int fps=10;fps<=30;fps+=5)
					rtpfps->push_back(RtpStream(rtpName,i,fps));
				rtp->push_back(rtpfps);
			}
			initiatorsData.insert(make_pair(rtpName,rtp));
		}
	}
}

VideoStreamAppl::~VideoStreamAppl()
{
}


VideoStreamAppl::RtpStream::RtpStream(const string & rtpName,int rate,int fps) {
	  stringstream sstr;
	  sstr << rtpName  << rate << "_" << fps << ".rtp";
	  video = new RtpData(sstr.str(),false);
	  unsigned long videoSize = video->getTotalSize();
	  unsigned long offset = video->getTimes(video->getTimes()-1);
	  totalTime = offset/1000;
	  totalSize = videoSize;
	  bpsRate = ((totalSize/totalTime)/1000000);
	  DEBUG_OUT << rtpName << " rate " << rate << " fps " << fps << " with video#" << video->getSizes() << "[" << videoSize << "] " << "] - Time=" << totalTime << " " << bpsRate << "Mbps" << endl;
	  if (AdHocWiFiApplLayer::rtpStatistics) {
		  cOutVector * v = new cOutVector(sstr.str().c_str());
		  for (int t=0;t<video->getSizes();t++)
			  v->record(video->getSizes(t));
		  delete v;
	  }
}

void VideoStreamAppl::Stream::changeRate(VideoStreamAppl * parent,int rate,int fps,double time)
{
	if (fps==-1) {
		if (rate==-1)
			rate = fps = 0;
		else
			fps = (*initiatorData)[rate]->size()-1;
	}
	if (rate==-1)
		rate = fps = 0;
	if ((currInitRate==(unsigned)rate) && ((unsigned)fps==currInitFPSindex))
		return;
	currInitRate = rate;
	currInitFPSindex = fps;
	DEBUG_OUT << "Node " << parent->getNodeID() << " stream " << streamNumber <<" Changing rate to " << rate+1 << "-" << 10+fps*5 << " rate=" << this->getTotalOutRate() << " bps=" << (*(*initiatorData)[rate])[fps].bpsRate << endl;
	(*(*initiatorData)[rate])[fps].advanceTime(currVidMsg,time);
	AdHocWiFiApplLayer::changeRate(streamNumber,rate,fps);
}

void VideoStreamAppl::RtpStream::advanceTime(unsigned & id,double time)
{
	time *= 1000; // switch to ms
	int num = video->getTimes();
	double max = video->getTimes(num-1);
	if (time>max) {
		while (time>max)
			max-= time;
		time = max;
	}
	for (int i=0;i < num;i++) {
	  bool above = video->getTimes(i) >= time;
	  if (above) {
		  if (i==0)
			  id = 0;
		  else
			  id = i-1;
		return;
	  }
	}
}

bool VideoStreamAppl::Stream::setRtpPool(VideoStreamAppl * parent,int rtpRate,double time,bool useHighest)
{
	unsigned int fpsRate = 0;
	FOR_EACH(RtpPerFPS,fpsIter,*(*initiatorData)[rtpRate]) {
		if (!(useHighest && (fpsRate+1<(*initiatorData)[rtpRate]->size()))) {
			bool above = (fpsIter->bpsRate*1E6 > getTotalOutRate());
			if (above && (fpsRate==0)) {
				changeRate(parent,rtpRate-1,-1,time);
				return true;
			}
			if (above) {
				changeRate(parent,useHighest ? rtpRate-1 : rtpRate,useHighest ? fpsRate : fpsRate-1,time);
				return true;
			}
		}
		fpsRate++;
	}
	if ((unsigned)rtpRate==initiatorData->size()-1) {
		changeRate(parent,rtpRate,fpsRate-1,time);
		return true;
	}
	return false;
}

void VideoStreamAppl::setRtpPool(int stream,double time)
{
	if (streams[stream]->getTotalOutRate()==0)
		return; //
	if (streams[stream]->initiatorData==NULL)
		return;
	int iterRate = 0;
	if (streams[stream]->currInitFPSindex==(unsigned)-1) {
		FOR_EACH(RtpPerRate,rIter,*(streams[stream]->initiatorData)) {
			if (streams[stream]->setRtpPool(this,iterRate,time,true))
				return;
			iterRate++;
		}
	}

	if (streams[stream]->setRtpPool(this,streams[stream]->currInitRate,time))
			return;
	iterRate = 0;
	FOR_EACH(RtpPerRate,rIter,*(streams[stream]->initiatorData)) {
		if (streams[stream]->setRtpPool(this,iterRate,time))
			return;
		iterRate++;
	}
}

int VideoStreamAppl::RtpStream::getMsg(unsigned& vidMsg,unsigned & id,simtime_t& time)
{
	unsigned long videoTime = video->getTimes(vidMsg);

	//double now = SIMTIME_DBL(simTime());
	time = ((double)(videoTime))/1000;

	id = vidMsg;
	return video->getSizes(vidMsg++);
}

void VideoStreamAppl::initialize()
{
    if (throughput.empty()) {  // once per all video streams classes
      int size = AdHocWiFiApplLayer::streamDestinations.size()+1;
      throughput.reserve(size);
      throughput.push_back(NULL);
      for (int t=1;t<size;t++) {
    	  stringstream str;
    	  str << "Throughput str" << t << "(" << AdHocWiFiApplLayer::streamSources[t] << "->" << AdHocWiFiApplLayer::streamDestinations[t] << ")[Mbps]";
    	  throughput.push_back(new cOutVector(str.str().c_str()));
      }

    }

}


int VideoStreamAppl::getNodeID() const {
	return nodeID;
}

// route messages in node per tasks in slot
void VideoStreamAppl::checkAndSetTask(int currSlot,int channel)
{
	Int2TasksMap::const_iterator it = nodeTasks.find(currSlot);
	if (it!=nodeTasks.end()) {
		FOR_EACH_C(TasksCollection,tit,it->second)
		  if ((*tit)->GetFreqNum()==channel)
			channels[(*tit)->GetFreqNum()]->setCurrentTask(*tit);
	}
}

void VideoStreamAppl::report(int round)
{
	FOR_EACH(Streams,streamIter,streams) { // over all incoming messages
		DEBUG_OUT << "Host: " << nodeID << " with " << streamIter->second->incomingMessages.size() << " on stream #" << streamIter->first << " at round " << round << endl;
	}
}

void VideoStreamAppl::reportStreams(int round)
{
  for (unsigned int i =1;i<throughput.size();i++) {
    int destination = AdHocWiFiApplLayer::streamDestinations[i];
    VideoStreamAppl * app = apps[destination];
    app->checkAndCreateStream(i);
    throughput[i]->recordWithTimestamp((simtime_t)round,((double(app->streams[i]->getTotalInRoundRate(destination)))/1E6)); //Mbps
  }
}

void VideoStreamAppl::sampleQueueLength(int round,int offset)
{
 static int msgLenBytes = channels[1]->getMsgLength()/8;
 FOR_EACH(Streams,iter,streams) {
	 Stream * stream = iter->second;
     if (AdHocWiFiApplLayer::statisticsMode==AdHocWiFiApplLayer::Full) {
	      if (stream->queueSizePerSlotAvgStat==NULL) {
	    	  stringstream str;
	    	  str << "n" << nodeID << ":Q length_mean_str" << iter->first << "[#msg]";
	    	  stream->queueSizePerSlotAvgStat = new cOutVector(str.str().c_str());
	    	  str.str("");
	    	  str << "n" << nodeID << ":Q length_max_str" << iter->first << "[#msg]";
	    	  stream->queueSizePerSlotMaxStat = new cOutVector(str.str().c_str());
	      }
	      double d = (round-1)*SLOT_Q_SAMPLE+offset;
	      stream->queueSizePerSlotAvgStat->recordWithTimestamp((simtime_t)d,(stream->queueSizePerSlotTotal/stream->queueSizePerSlotActions)/msgLenBytes); // record queue length in #messages
	      stream->queueSizePerSlotMaxStat->recordWithTimestamp((simtime_t)d,(stream->queueSizePerSlotMax)/msgLenBytes); // record queue length in #messages
	      stream->queueSizePerSlotTotal = stream->queueSize;
	      stream->queueSizePerSlotActions = 1;
	      stream->queueSizePerSlotMax = stream->queueSize;
	    }
 }
}

void VideoStreamAppl::endOfRound(int round)
{
  appsEnded++;
  if (nodeID==1)
    reportStreams(round);
  if (round>=0)
	  report(round);

  sampleQueueLength(round,SLOT_Q_SAMPLE);

  if (appsEnded==apps.size()) { // synchronized out of end-of-round and into start round

    std::vector<CommunicationRequest *> requests = AdHocWiFiApplLayer::requestsReader->GetRequests();
    std::vector<CommunicationRequest *>::iterator reqsIter;
    // go over the requests, start flow control message up-stream from the destination
    for (reqsIter = requests.begin(); reqsIter != requests.end(); reqsIter++) {
      VideoStreamAppl * app = apps[(*reqsIter)->GetFirstDestination()];
      app->streams[(*reqsIter)->GetRequestNum()]->flowControl((*reqsIter)->GetFirstDestination(),round);
    }

    // now re-init the data of the round
    appsEnded = 0;
    for (unsigned a=0;a<apps.size();a++)
      apps[a]->initStartRound(false);
  }

}

void VideoStreamAppl::initStartRound(bool reportOnly)
{
  FOR_EACH(Streams,iter,streams) {
    Stream * stream = iter->second;
    FOR_EACH(Stream::OutEdgesCollection,eIter,stream->outEdges) {
    	if (!reportOnly) {
    		eIter->second.roundOut = 0;
    		eIter->second.inactiveBits = 0;
    	}
      
      AdHocWiFiMsg msgDetails(iter->first,eIter->second.rate,0,nodeID,eIter->first,0,NULL);
      msgDetails.IDs.push_back(-1);
      msgDetails.recordLog(AdHocWiFiMsg::Rate);
    }
  }
}

void VideoStreamAppl::Stream::flowControl(int nodeID,int round)
{

  if (++flowControlMessagesReceivedInRound < outEdges.size())
    return; // wait till we get all the flow control messages from out edges
  
  flowControlMessagesReceivedInRound = 0;
  unsigned long int out=0,roundOut=0,totalIn=0,rateOut=0,maxRateOut=0;

	if (outEdges.empty()) {
		std::vector<CommunicationRequest *> requests = AdHocWiFiApplLayer::requestsReader->GetRequests();
		long request = requests[streamNumber-1]->GetDemand()*1E6;
		roundOut = request*DESTINATION_REQUEST_MULTIPLIER;
		rateOut = request*DESTINATION_REQUEST_MULTIPLIER;
		maxRateOut = request*DESTINATION_REQUEST_MULTIPLIER;
	}
	else
	  FOR_EACH(OutEdgesCollection,eIter,outEdges) {
		roundOut += eIter->second.getTotalRoundRate();
		rateOut += eIter->second.rate;
		maxRateOut += eIter->second.maxRate;
	  }

	if (inEdges.empty())
		totalIn = rateOut;
	else
	  FOR_EACH(InEdgesCollection,inIter,inEdges)
	    totalIn += inIter->second.getTotalRoundRate(nodeID,streamNumber);

    unsigned long rateIn = rateOut;
    if (rateIn > roundOut)
    	rateIn = roundOut;
    if (rateIn > totalIn)
    	rateIn = totalIn;
//    if (rateIn > maxRateOut)
//    	rateIn = maxRateOut;

    FOR_EACH(IntSet,it,incomingTasks) {
      int sendingToMeNodeID = *it;
      if (sendingToMeNodeID==nodeID)
        continue;
      VideoStreamAppl * sendingToMeAppl = VideoStreamAppl::apps[sendingToMeNodeID];

      unsigned long actual = sendingToMeAppl->streams[streamNumber]->outEdges[nodeID].roundOut;
      unsigned long nodeIn = sendingToMeAppl->streams[streamNumber]->outEdges[nodeID].getTotalRoundRate();
      float weight = (((float)nodeIn)/totalIn);
      unsigned long newRate = weight*rateIn;
      unsigned long oldRate = sendingToMeAppl->streams[streamNumber]->outEdges[nodeID].rate;

      sendingToMeAppl->streams[streamNumber]->outEdges[nodeID].rate = newRate;
      if (newRate != (unsigned long)oldRate) {
        DEBUG_OUT << "FC: Host " << nodeID << "(" << sendingToMeNodeID << ") weight=" << weight << 
	  " new rate = " << newRate << " old rate=" << oldRate << " actual=" << actual << " in=" <<
	  nodeIn <<  ",total-in=" << totalIn <<  " Stream #" << streamNumber << " " << endl;
        sendingToMeAppl->setRtpPool(streamNumber);
	sendingToMeAppl->streams[streamNumber]->flowControl(sendingToMeNodeID,round);
      }
    }

	DEBUG_OUT<< "Node:" << nodeID << " Rate=" << rateIn << " prev rate= " << rateOut << " In=" <<
	  totalIn << " out=" << out  << " total out="<< roundOut << " stream #" << streamNumber << " @" << round << endl;
	if (!outEdges.empty())
		queueDrop(nodeID,rateIn);
}

void VideoStreamAppl::Stream::queueDrop(int nodeID,unsigned long rate)
{
  if (queueSize*8<rate)
		return;
  int reduce = (queueSize*8-rate)/8; // in,out in bits,queue in bytes
  int newSize =  queueSize - reduce;
  if (newSize<0)
		  newSize = 0;
  unsigned removeSize = 0;
  while ((unsigned)newSize < queueSize) {
	ApplPkt * last = incomingMessages.front();
    AdHocWiFiMsg msgDetails(last->getName());
   	FOR_EACH(IntList,idsIter,msgDetails.IDs)  {// mark as dropped
   	  AdHocWiFiApplLayer::messagesStatistics[*idsIter].lastRecieve = PKT_DROPPED;
   	  msgDetails.recordLog(AdHocWiFiMsg::Drop);
   	}
	int packetSize = last->getBitLength()/8;
	delete last;
	queueSize -= packetSize;
	removeSize += packetSize;
	incomingMessages.pop_front();
  }
  if (removeSize)
	  DEBUG_OUT << "Node:" << nodeID << " str#" << streamNumber << " chopped " << removeSize*8 << " bits from queue" << endl;
}


void VideoStreamAppl::setInitiator(int stream)
{
  checkAndCreateStream(stream);
  streams[stream]->incomingTasks.insert(nodeID); // incoming from self
}

void VideoStreamAppl::addChannel(AdHocWiFiApplLayer* appl, short channel,
								ScheduleItemCollection::Stream2SchedulesMap& tasks,
								const Requests& requests,
								const string& rtpName) {
	 channels[channel] = appl;
	 FOR_EACH(ScheduleItemCollection::Stream2SchedulesMap,tasksIter,tasks) {
		 double totalRate = 0;
		 FOR_EACH(ScheduleItemCollection::NodeSchedules,schIter,tasksIter->second) { /// these tasks are only mine
			 if (schIter->get()->GetFreqNum()==channel) {
				 nodeTasks[schIter->get()->GetTimeSlot()].insert(*schIter);
				 double slotRate = (schIter->get()->GetFlowInSlot()*1E6);
				 totalRate+= slotRate;
				 int stream = schIter->get()->GetRequestNum();
				 checkAndCreateStream(stream);
				 apps[schIter->get()->GetReciever()]->addIncomingTask(*schIter);
				 streams[stream]->checkAndCreateOutEdgeAddRate(schIter->get()->GetReciever(),slotRate);
				 FOR_EACH_C(Requests,rIt,requests) {
					 if (((*rIt)->GetRequestNum()==schIter->get()->GetRequestNum()) &&
							 ((*rIt)->GetFirstDestination()==schIter->get()->GetReciever()))
						 preEndStreams.insert((*rIt)->GetRequestNum()); // add to pre end streams
				 }
			 }
		 }
		 DEBUG_OUT << "Rate for node " << nodeID << " stream " << tasksIter->first << " channel " << channel << " is " << totalRate << endl;
	 }
	 if (channels.size()==NUMBER_OF_CHANNELS) {
		 FOR_EACH(Streams,sIt,streams) {
			if (AdHocWiFiApplLayer::mode==AdHocWiFiApplLayer::VIDEO) {
				InitiatorsData::iterator it = initiatorsData.find(rtpName);
				if (it != initiatorsData.end())
					sIt->second->initiatorData = it->second;
				else {
					assert(0);
				}
				setRtpPool(sIt->first,0);
			}
			else sIt->second->initiatorData = NULL;
		 }
	 }
}

void VideoStreamAppl::checkAndCreateStream(int stream)
{
	if (streams.find(stream)==streams.end()) {
		streams[stream] = new Stream(nodeID,stream);
	}
}

void VideoStreamAppl::addIncomingTask(ScheduleItemSPtr& task)
{
	int stream = task->GetRequestNum();
	checkAndCreateStream(stream);
    streams[stream]->incomingTasks.insert(task->GetSender());
    Stream::InEdgesCollection::iterator it = streams[stream]->inEdges.find(task->GetSender());
    if (it==streams[stream]->inEdges.end())
    	streams[stream]->inEdges[task->GetSender()] = Stream::InEdge(task->GetSender());
    apps[task->GetSender()]->checkAndCreateStream(stream);
}

void VideoStreamAppl::addMessage(int stream,cMessage * msg) {
	 streams[stream]->addMessage(msg);
	 DEBUG_MSG(nodeID << " got message str#" << stream << " @" << simTime() << " " << msg->getFullName() << "\n";)
}

void VideoStreamAppl::Stream::addMessage(cMessage * msg) {
	 ApplPkt * pkt = check_and_cast<ApplPkt*>(msg);
	 incomingMessages.push_back(pkt);
	 queueSize += pkt->getBitLength()/8; // Bytes
	 queueSizePerSlotMax = max(queueSize,queueSizePerSlotMax);
	 queueSizePerSlotActions++;
	 queueSizePerSlotTotal += queueSize;
}

ApplPkt * VideoStreamAppl::getMessage(ScheduleItemSPtr task,int slot,int channel,long& bits) {
	ApplPkt * pkt = streams[task->GetRequestNum()]->getMessage(this,task,slot,channel,bits);
	return pkt;
}


unsigned long VideoStreamAppl::Stream::phantomRequests(ScheduleItemSPtr task,int bits) {
	unsigned long total = (outEdges[task->GetReciever()].rate - outEdges[task->GetReciever()].roundOut);
	int mcs = task->GetMCS();
	int bitsPerSlot = AdHocWiFiMac::getActualBitrate(mcs,bits)/AdHocWiFiApplLayer::slots;
	double partLeft = AdHocWiFiApplLayer::getSlotPart();
	unsigned long bitsToTx = bitsPerSlot*partLeft;
	if (bitsToTx>total)
		bitsToTx = total;
	return bitsToTx;
}

// get a message from queue (currently FIFO)
ApplPkt * VideoStreamAppl::Stream::getMessage(VideoStreamAppl * vid,ScheduleItemSPtr task,int slot,int channel,long& bits) {
	int to = task->GetReciever();
	double currRate = outEdges[to].roundOut;
	int diff = (outEdges[to].rate - currRate);
	if (diff<bits) { // if more than rate
		outEdges[to].inactiveBits += bits;
		return NULL; // exceeding limit, stop
	}
	if (diff < bits) { // no partial message
		//bits = rates[stream]-currRate;
		return NULL; //
	}
	MessagesDeque::iterator it = incomingMessages.begin();
	ApplPkt * msg = NULL;
	if (it!=  incomingMessages.end()) {
	  if (!incomingMessages.empty()) {
		 long msgBits = 0;
 		 msg = new ApplPkt("", AdHocWiFiApplLayer::MSG_VID_STREAM);
 		 string name; // get messages with size of bits as desires and put all their IDs in message details
#define MSG_COLLECT false
 		 bool msg_collect = MSG_COLLECT;
		 do {
			 ApplPkt * pkt = incomingMessages.front();
			 incomingMessages.pop_front();
			 msgBits+= pkt->getBitLength();
			 AdHocWiFiMsg::decodeMsg(pkt->getName(),name);
#ifdef NO_DATA_MSG
			 if (msgBits > bits) { // got too much bits, return some to queue
				 pkt->setBitLength(msgBits-bits);
				 incomingMessages.push_back(pkt);
				 msgBits -= (msgBits-bits);
			 }
			 else
#endif
				 delete pkt;
		 } while ((msgBits < bits) && (incomingMessages.size()>0) && (msg_collect));
 	     msg->setBitLength(msgBits);
 	     msg->setName(name.c_str());
 	     bits = msgBits;
	  }
	}

	if (msg) { // got message for tx - perform statistics
		outEdges[to].roundOut += bits;
	  if (queueSize) {// not initiator
		 queueSize -= bits/8;
	  }
	  queueSizePerSlotActions++;
	  queueSizePerSlotTotal += queueSize;
	  return msg;
	}
	else {
		if (AdHocWiFiApplLayer::oneSlot)
			outEdges[to].inactiveBits += bits; // phantom one message, we don't inactivate the slot in "one slot" mode
		else
	      outEdges[to].inactiveBits += phantomRequests(task,bits); // mark statistics for more than rate
	}
	 // may return NULL if not found
	return NULL;
}

unsigned long VideoStreamAppl::Stream::getTotalOutRate() const
{
	int r = 0;
	FOR_EACH_C(OutEdgesCollection,eIter,outEdges) {
		r+= eIter->second.rate;
	}
	return r;
}

unsigned long VideoStreamAppl::Stream::getTotalInRoundRate(int nodeID) const
{
	int r = 0;
	FOR_EACH_C(InEdgesCollection,eIter,inEdges) {
		r+= VideoStreamAppl::apps[eIter->first]->streams[streamNumber]->outEdges[nodeID].roundOut;
	}
	return r;
}

void VideoStreamAppl::Stream::checkAndCreateOutEdgeAddRate(int to,int rate)
{
	OutEdgesCollection::iterator eIt = outEdges.find(to);
	if (eIt==outEdges.end()) {
		OutEdge edge;
		edge.to = to;
		edge.rate = rate;
		edge.roundOut = 0;
		edge.inactiveBits = 0;
		edge.maxRate = rate;
		outEdges[to] = edge;
	}
	else {
	  eIt->second.rate += rate;
  	  eIt->second.maxRate = eIt->second.rate;
	}
}

ApplPkt * VideoStreamAppl::getInitiatorMessage(int stream,int slot,int channel,unsigned& bits,unsigned id)
{
	ApplPkt * msg = NULL;
	unsigned msgId = 0;
	simtime_t time;
	if (AdHocWiFiApplLayer::mode==AdHocWiFiApplLayer::VIDEO) {
		//TODO cout << "node=" << nodeID << " stream=" << stream;
		//cout << "streams[stream]->currInitRate=" << streams[stream]->currInitRate << " streams[stream]->currInitFPSindex=" << streams[stream]->currInitFPSindex << " streams[stream]->currVidMsg=" << streams[stream]->currVidMsg << endl;
		bits = (*(*streams[stream]->initiatorData)[streams[stream]->currInitRate])[streams[stream]->currInitFPSindex].getMsg(streams[stream]->currVidMsg,msgId,time);
	}
	if (bits) {
		char msgHeader[10];
		sprintf(msgHeader,"%u:%u",id,msgId);
		// create message with id and size in bits
		msg = new ApplPkt(msgHeader,AdHocWiFiApplLayer::MSG_VID_STREAM);
		msg->setBitLength(bits);
		msg->setArrivalTime(time);
		// payload
	}
	else {
		delete msg;
		return NULL;
	}
	return msg;
}

void VideoStreamAppl::addSINR(const ApplPkt * msg,const DeciderResult80211 * dr)
{
	AdHocWiFiMsg msgDetails(msg->getName());
	apps[msgDetails.to]->streams[msgDetails.stream]->addSINR(msgDetails,msg,dr);
}

void VideoStreamAppl::Stream::addSINR(const AdHocWiFiMsg& details,const ApplPkt * msg,const DeciderResult80211 * dr)
{
  if (AdHocWiFiApplLayer::statisticsMode==AdHocWiFiApplLayer::Full) {
    FOR_EACH_C(IntList,idsIter,details.IDs)  {// mark SINR
      AdHocWiFiApplLayer::messagesStatistics[*idsIter].lastSINR = dr->getSnr();
      if (!dr->isSignalCorrect())
    	  details.recordLog(AdHocWiFiMsg::Lost);
    }
  }
}

void VideoStreamAppl::finishSerial()
{
	report((int)simTime().dbl());
	FOR_EACH(Streams,strIter,streams) {
		strIter->second->finishSerial(nodeID);
		delete strIter->second;
	}
	streams.clear();
}

void VideoStreamAppl::finish()
{
  	for (vector<cOutVector*>::iterator it = throughput.begin(); it != throughput.end(); it++)
    		delete *it;
  	throughput.clear();

  	FOR_EACH(InitiatorsData,it,initiatorsData)
		delete it->second;
  	initiatorsData.clear();
}

void VideoStreamAppl::Stream::finishSerial(int nodeID)
{
	bool hop = AdHocWiFiApplLayer::streamDestinations[streamNumber]!=nodeID;
	FOR_EACH(MessagesDeque,msgIt,incomingMessages) {
	  if (hop) {
	    AdHocWiFiMsg msgDetails((*msgIt)->getName());
	    FOR_EACH(IntList,idsIter,msgDetails.IDs)  // mark as receive
	      AdHocWiFiApplLayer::messagesStatistics[*idsIter].lastRecieve = PKT_IN_QUEUE; // mark as in queue
	  }
	  delete *msgIt;
	}
	incomingMessages.clear();
	
}

VideoStreamAppl::Stream::Stream(int nodeID,int sn)
 :
	 streamNumber(sn),
	 inLength(NULL),outLength(NULL),/*queueSizeStat(NULL),*/queueSizePerSlotAvgStat(NULL),queueSizePerSlotMaxStat(NULL),
	 queueSize(0),queueSizePerSlotTotal(0),queueSizePerSlotMax(0),queueSizePerSlotActions(1),initiatorData(NULL),currInitRate(-1),currVidMsg(0),currInitFPSindex(-1)
 {
 }

VideoStreamAppl::Stream::~Stream()
{
	incomingMessages.clear();
	delete inLength;
	delete outLength;
	delete queueSizePerSlotAvgStat;
	delete queueSizePerSlotMaxStat;
  	initiatorData = NULL;
}
