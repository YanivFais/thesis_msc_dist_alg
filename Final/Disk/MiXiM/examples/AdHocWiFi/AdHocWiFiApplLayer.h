/* ********************************************************
 * File:        AdHocWiFiApplLayer.h
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: send messages with random delay and size
 * halts every interval to allow other side to move
 **************************************************************************/


#ifndef AD_HOC_WI_FI_APPL_LAYER_H
#define AD_HOC_WI_FI_APPL_LAYER_H

#include "AdHocWiFi.h"
#include <vector>
#include <deque>
#include "ScheduleItemCollection.h"
#include "RequestsCollection.h"
#include <fstream>
using namespace std;

#include <BaseApplLayer.h>

/**
 * @brief Application layer to test lower layer implementations
 *
 * This application layer is very similar to the BurstApplLayer.
 * The difference is that is sends messages with different sizes and waits every interval
 * The parameters can be set in omnetpp.ini
 *
 * @sa TestApplALyer
 * @ingroup applLayer
 * @author Yaniv Fais
 **/

class VideoStreamAppl;

/**
 * Application layer (per channel) of node
 * Performs send/recieve to channel and communicates with high-level node application which stores messages
 */
class AdHocWiFiApplLayer : public BaseApplLayer
{
 public:

	 /**
	  * initialize object with stage
	  * @param stage initialization stage
	  */
    virtual void initialize(int stage);

    /**
     * finish run
     */
    virtual void finish();

    /**
     * types of messages sent
     */
    enum AdHocWiFiApplLayerMsg {
    	MSG_VID_STREAM = LAST_BASE_APPL_MESSAGE_KIND,
    	MSG_NEW_SLOT,
    	MSG_TRANSMIT,
    	MSG_INPUT,
    	LAST_AD_HOC_WI_FI_APPL_MESSAGE_KIND
    	};

    /**
     * mode of run - video or raw data
     */
    enum Mode { VIDEO, RAW };

    /**
     * mode of statistics - part or full
     */
    enum StatisticsMode { Part, Full };

    /**
     * set the current active task
     */
    void setCurrentTask(const ScheduleItemSPtr task);

    /**
     * mark transmit as over
     */
    void txOver();

    /**
     * return an initiator (camera) message
     */
    unsigned getInitiatorMessage(int stream, int slot, long  bits);

    /**
     * cancel last message requested to send
     */
    void cancelLastInitiatorMessage();

    /**
     * @return message length in bits
     */
    unsigned long getMsgLength() const
    {
        return headerLength;
    }

    /**
     * @return ID of node
     */
    int getNodeID() const 
    {
      return nodeID;
    }

    /**
     * delay message send by a message time in rate
     */
    void delayMessage(int rate);

protected:

    /**
     * perform weighted arbitration on current tasks
     */
    ScheduleItemSPtr getArbitrarlyTask();
    
    /**
     * sent a message from active tasks
     */
    void transmitMessage();

    /**
     * sent a specific message per task
     * @param task task to send message for
     */
    void transmitMessage(ScheduleItemSPtr task);

    /**
     * handle control message
     * @param msg message to handle
     */
    virtual void handleSelfMsg(cMessage* msg);

    /**
     * handle mac message
     * @param msg message to handle
     */
    virtual void handleLowerMsg(cMessage* msg);

    /**
     * change slot in scheduling table
     */
    void changeSlot();
    
    /**
     * set a camera message event
     * @param stream number of stream for video
     * @param msg event message
     */
    bool setCameraEvent(int stream,cMessage * msg);

    /**
     * is current application active (sending) currently
     */
    bool active;

    /**
     * has transmission task in current slot
     */
    bool hasTask;

    /**
     * current scheduling task, if multiple then can perform weighted arbitration (non normal mode), holds pair of sent messages per task and the task itself
     */
    typedef list<pair<int,ScheduleItemSPtr> > ActiveTasks;
    ActiveTasks currentTasks;

    /**
     * WiFi channel of node and node id
     */
    int channel, nodeID;

    /**
     * current slot
     */ 
    int currSlot;

    /**
     * offset (part of slot) for sampling queue size statistics in one slot mode
     */
    int queueSampleOffset;

    /**
     * current scheduling iterative round
     */
    int currRound;

    /**
     * internal control messages - change slot and route message
     */
    cMessage *slotMsg, *transmitMsg;

    /**
     * internal control message for getting input (camera) message
     */
    typedef map<int,cMessage*> Int2MessageCollection;
    Int2MessageCollection inputControlMessages;

    /**
     * network addresses map from nodeIdxchannl to address
     */
    static map<int,map<int,int> > networkAddresses;

    /**
     * tasks of node/channel
     */
    ScheduleItemCollection::Stream2SchedulesMap tasks;

    /**
     * streams which are initiators (sources) for this node
     */
    IntSet initiatorStreams;
    
    typedef map<int,VideoStreamAppl*> ApplicationsMap;

    /**
     * collection of high level applications map from id
     */
    static ApplicationsMap applications;

    /**
     * cnt is last message id of transmit,prevSlotCnt is sample of cnt and idleSlots is number of slots in which there was no send
     */
    int cnt, prevSlotCnt, idleSlots;
    
    /**
     * message id overall counter
     */
    static unsigned long msgID;

    /**
     * final times per stream map output file (for video playback)
     */
    static map<int,ofstream*> finalTimes;

    /**
     * vectors for storing schedule table
     */
    static cOutVector * schedFromVec,* schedToVec,* schedSlotVec,* schedChannelVec,* schedStreamVec,*schedDataVec,*schedMcsVec;


public:

    /**
     * message statistics storage per packet
     */
    struct MessagesStatistics
    {

      /**
       * time of first send
       */
      SimTime initialSend;

      /**
       * time of last recieve
       */
      SimTime lastRecieve;

      /**
       * stream number
       */
      int stream;

      /**
       * SINR of last message received
       */
      double lastSINR;
      
      /**
       * @param s number of stream
       */
      MessagesStatistics(int s)
        :initialSend(simTime()), stream(s),lastSINR(0)
        {
        }

    };

    /**
     * statistics of all messages
     */
    static vector<MessagesStatistics> messagesStatistics;

    /**
     * requests collection of streams
     */
    static RequestsCollection *requestsReader;

    /**
     * collection of stream destinations and sources
     */
    static Int2IntMap streamDestinations, streamSources;

    /**
     * max time of any slot
     */
    static double slotMaxTime;

    /**
     * number of slots in schedule
     */
    static int slots;

    /**
     * time of round, usually slotMaxTime*slots however can be different in special mode
     */
    static double roundTime;

    /**
     * slot sample part for queue size if in one slot
     */
    static double slotSamplePart;

    /**
     * number of total rounds to repeat scheduling table iteratively
     */
    static int rounds;

    /**
     * number of frequency channels
     */
    static int channels;

    /**
     * schedule table
     */
    static ScheduleItemCollection *schedule;

    /**
     * if true store RTP statistics
     */
    static bool rtpStatistics;

    /**
     * if true enable arbitration between queues when sending
     */
    static bool enableArbitration;

    /**
     * if true all entries in table would be scheduled every slot
     */
    static bool oneSlot;

    /**
     * mode of transmission (raw,video)
     */
    static Mode mode;

    /**
     * mode of statistics
     */
    static StatisticsMode statisticsMode;

    /**
     * change rate per stream
     */
    static void changeRate(int stream, int rate, int fps);

    /**
     * @return message size in bits
     */
    static int getMessageSizeBits() { return msgSizeBits; }


    /**
     * @return part of slot remaining from current time
     */
    static double getSlotPart();

private:

    /**
     * finish simulation and process statistics
     */
    void finishGlobal();

    /**
     * message size in bits
     */
    static int msgSizeBits;
};


#define MAX_HEADER_SIZE 500
/**
 * actual Messsage formatting (contains all info in header as means to transfer data and formatting in messages)
 */
struct AdHocWiFiMsg {
  
  int stream,channel,rate,from,to,slot,cnt;

  /**
   * ID's of initial messages in this (if wrapping several)
   */
  IntList IDs;

  /**
   * ID's of video data packets
   */
  StringList dataIDs;
  
  /**
   * header as string
   */
  char * header;
  
  /**
   * static counter for all messages
   */
  static int s_cnt;
  /*
   * construct message details from parameters
   */
  AdHocWiFiMsg(int stream,int channel,int rate,int from,int to,int slot,ApplPkt * msg);
  
  /**
   * construct message details from header string
   */
  AdHocWiFiMsg(const char * headerP);
  
  /*
   * decode locally the messages id's
   */
  void decodeIDs(string idsStr);
  
  /**
   * decode to IDs the id's in header string
   */
  static void decodeMsg(const char * header,string& IDs);

  enum EventType { Receive , Transmit, Drop , Lost , Rate };

  /**
   * csv log file for events
   */
  static ofstream log;

  /**
   * record log event
   */
  void recordLog(EventType evt) const;

};

#endif // AD_HOC_WI_FI_APPL_LAYER_H

