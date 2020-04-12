/*********************************************************
 * File:        VideoStreamAppl.h
 *
 * Author:      Yaniv Fais
 *
 *
 ***************************************************************************
 * description: top application layer for node,
 * contains the sending AdHocWiFiApplLayer objects which are used for communication per channel (frequency)
 **************************************************************************/

#ifndef VID_STREAM_APPL_LAYER_H
#define VID_STREAM_APPL_LAYER_H

#include "AdHocWiFi.h"
#include <vector>
#include <deque>
#include "AdHocWiFiApplLayer.h"
#include "DeciderResult80211.h"
#include "RtpData.h"
using namespace std;

/**
 * @brief Video Application layer
 *
 * @sa VideoStreamAppl
 * @ingroup applLayer
 * @author Yaniv Fais
 **/

class AdHocWiFiApplLayer;

/**
 * High level application per node
 * holds applications per channel which to channel send/receive
 * gathers messages to queues and stores statistics
 */
class VideoStreamAppl {
public:

  /**
   * @param id node id for application node
   * @param rtpName prefix name of RTP video file
   * @param number number of RTP file
   */
	VideoStreamAppl(int id,const string& rtpName,int rtpNumber);

	~VideoStreamAppl();

	typedef std::vector<CommunicationRequest *> Requests;

	/**
	 * add a channel lower application
	 * @param appl application per channel pointer
	 * @param channel channel of application
	 * @param tasks node tasks from scheduler
	 * @param requests sourcexdestination requests collection
     * @param rtpName prefix name of RTP video file
	 */
	void addChannel(AdHocWiFiApplLayer* appl, short channel,
			ScheduleItemCollection::Stream2SchedulesMap& tasks,
			const Requests& requests, const string& rtpName);

	/**
	 * route message in node
	 * @param currSlot current slot in schedule
	 * @param channel channel to set
	 */
	void checkAndSetTask(int currSlot,int channel);

	/**
	 * initialize
	 * called from AdHocWiFiApplLayer
	 */
	void initialize();

	/**
	 * report in end of round
	 * @param round - number of round which ended
	 */
	void report(int round);

	/**
	 * report in end of round for all streams
	 * @param round - number of round which ended
	 */
	void reportStreams(int round);

	/**
	 * end of round - perform statistics gathering and flow control
	 * @param round - number of round which ended
	 */
	void endOfRound(int round);

	/**
	 * initialize parameters for start of round
	 * @param reportOnly if true then only report and not initialize
	 */
	void initStartRound(bool reportOnly);

	/**
	 * called upon finish of simulation - store statistics and perform cleanup
	 */
	void finish();

	/**
	 * define the names  of the RTP file base bane and number
	 * @param name base name of RTP files
	 * @param number number of streams
	 */
	void setRtpFiles(const string& name, int number);

	/**
	 * receive a message
	 * @param stream number of stream for the message
	 * @param msg received msg message to add to application
	 */
	void addMessage(int stream,cMessage * msg);

	/**
	 * get a message from queue
	 * @param task task to transmit
	 * @param slot number of current slot
	 * @param channel number of channel to send message to
	 * @param bits number of bits to get from queue -
	 * 	    * note passed by reference since if queue has less than needed bits would be changed to what actually can provide
	 * @return application packet with details in header and bit size
	 */
	ApplPkt * getMessage(ScheduleItemSPtr task, int slot, int channel, long& bits);

	/**
	 * @return id of node
	 */
	int getNodeID() const;

	/**
	 * finish serial
	 */
	void finishSerial();

	/**
	 * @param msg message of receive
	 * @param dr decider result for message
	 */
	static void addSINR(const ApplPkt * msg,const DeciderResult80211 * dr);

	/**
	 * sample the queue length in offset inside round
	 * @param round round number
	 * @param offset - offset number inside round (like 4 if we are in 4/5 part)
	 */
	void sampleQueueLength(int round,int offset);

private:

	/**
	 * add an incoming task which I'm suppose to recieve data from
	 */
	void addIncomingTask(ScheduleItemSPtr& task);

	// map between channel number and channel-application object
	map<int, AdHocWiFiApplLayer*> channels;

	// ID of node
	int nodeID;

	/**
	 * check and create a stream struct
	 */
	void checkAndCreateStream(int stream);

	typedef set<ScheduleItemSPtr> TasksCollection;
	typedef map<int, TasksCollection> Int2TasksMap;
	//map between slot number and task
	Int2TasksMap nodeTasks;

	// store also streams which are one hop prior to destination (for rate increase)
	IntSet preEndStreams;

	// all video-stream applications collection (by nodeID index)
	static vector<VideoStreamAppl*> apps;

	static unsigned appsEnded;

	// per stream final throughput
	static vector<cOutVector*> throughput;


public:

	/**
	 * RTP stream data for initiators
	 */
	struct RtpStream {
	  /**
	   * video data
	   */
	  RtpData * video;
	  
	  /**
	   * @param name file name prefix
	   * @param rate bitrate indicator of video
	   * @param fps frame per seconds rate
	   */
	  RtpStream(const string& name, int rate, int fps);
	  
	  RtpStream() :
	    video(NULL) {
	  }
	  
	  /*
	   * @return message from pool
	   * @param vidMsg message id
	   * @param id current id
	   * @param time time of message arrival (camera)
	   */
	  int getMsg(unsigned& vidMsg, unsigned & id,simtime_t& time);
	  
	  /**
	   * advance time of streaming pool
	   * @param id id of message
	   * @param time time to advance to
	   */
	  void advanceTime(unsigned & id, double time);
	  
	  /**
	   * bit per second of video
	   */
	  double bpsRate;
	  
	  /**
	   * total time of video
	   */
	  double totalTime;
	  
	  /**
	   * total size of video
	   */
	  double totalSize;
	};
	
	typedef vector<RtpStream> RtpPerFPS;
	typedef vector<RtpPerFPS*> RtpPerRate;
	

public:
	// data from input (camera) per rate
	typedef map<string, RtpPerRate *> InitiatorsData;

	/**
	 * initiators (camera) data for applications per video bps/fps rates
	 */
	static InitiatorsData initiatorsData;

	/**
	 * represent a stream member of application
	 */
	struct Stream {

	  /**
	   * @param nodeID id of parent node
	   * @param sn stream number
	   */
	  Stream(int nodeID,int sn);
	  
	  ~Stream();
	  
	  /**
	   * add a message to incoming queue
	   * @param msg actual message received
	   */
	  void addMessage(cMessage * msg);
	  
	  /**
	   * get a message from queue
	   * @param vid video application
	   * @param task task for transmit
	   * @param slot number of current slot
	   * @param channel number of channel to send message to
	   * @param bits number of bits to get from queue -
	   * 	    * note passed by reference since if queue has less than needed bits would be changed to what actually can provide
	   * @return application packet with details in header and bit size
	   */
	  ApplPkt * getMessage(VideoStreamAppl * vid,ScheduleItemSPtr task, int slot, int channel,
			       long& bits);
	  
	  /**
	   * @return number of remaining phantom messages to send if queues were full in this slot
	   * @param task task for service
	   * @param bits bits per message
	   */
	  unsigned long phantomRequests(ScheduleItemSPtr task,int bits);

	  /**
	   * activate flow control tasks and send back flow control message
	   * @param nodeID id of node of work
	   * @param round number of round
	   */
	  void flowControl(int nodeID,int round);

	 /**
	  * drop from queue according to rate
	  * @param nodeID ID of parent node
	  * @param rate stream new rate
	  */
	  void queueDrop(int nodeID,unsigned long rate);

	  /**
	   * finish serial (synchronized)
	   * @param nodeID id of node
	   */
	  void finishSerial(int nodeID);
	  
	  /**
	   * @param details detailed information of message (from,to,rate and etc.)
	   * @param msg message of receive
	   * @param dr decider result for message
	   */
	  void addSINR(const AdHocWiFiMsg& details,const ApplPkt * msg,const DeciderResult80211 * dr);

	  /**
	   * An edge in the out stream
	   */
	  struct OutEdge {
		  int to;
		  unsigned int rate; // bits out allowed
		  unsigned int maxRate; // bits out allowed from scheduler
		  unsigned long roundOut; // rate in the current round already transmitted
		  // store number of requests bits per stream which are not being used since queue is full or rate too low
		  unsigned long inactiveBits;

		  /**
		   * @return round rate including phantoms
		   */
		  unsigned long getTotalRoundRate() { return roundOut+inactiveBits; }
	  };

		/**
		 * check if edge exists and it so add rate to the outgoing rate
		 * @param to destination of edge
		 * @param rate rate to add to out stream
		 */
		void checkAndCreateOutEdgeAddRate(int to,int rate);

		/**
		 * @retun total rate from all out edges
		 */
		unsigned long getTotalOutRate() const;

		typedef map<int,OutEdge> OutEdgesCollection;
		OutEdgesCollection outEdges;

	    /**
	     * An edge in the in stream
	     */
		struct InEdge {
		  InEdge(int fromP=0) : from(fromP) {}
		  int from;

		  /**
		   * @return round rate including phantoms
		   * @param nodeID id of parent node
		   * @param streamNumber number of parent stream
		   */
		  unsigned long getTotalRoundRate(int nodeID,int streamNumber) { 
		    return VideoStreamAppl::apps[from]->streams[streamNumber]->outEdges[nodeID].getTotalRoundRate(); 
		  }

		};

 	    /**
	     * @retun total rate from all in edges
	     */
	    unsigned long getTotalInRate() const;

 	    /**
	     * @return total rate from all in edges received in current round
	     * @param nodeID parent node number
	     */
	    unsigned long getTotalInRoundRate(int nodeID) const;

	    typedef map<int,InEdge> InEdgesCollection;
	    InEdgesCollection inEdges;

	 /**
	  * number of stream
	  */
	  int streamNumber;
	  
	  // queue of application messages
	  typedef deque<ApplPkt*> MessagesDeque;
	  
	  MessagesDeque incomingMessages;

	  // stream to number: number of incoming bits,outgoing bits, size of queue in bytes
	  cOutVector *inLength, *outLength, /**queueSizeStat,*/*queueSizePerSlotAvgStat,*queueSizePerSlotMaxStat;

	  // per stream queue size in byte and max inside the slot, plus total size (accumulated) and number of actions for computing average
	  unsigned int queueSize,queueSizePerSlotTotal,queueSizePerSlotMax,queueSizePerSlotActions;

	  //map between stream number and nodes I'm suppose to get data from
	  IntSet incomingTasks;
	  
	  // pointer to initiator video RTP data
	  RtpPerRate * initiatorData;

	  /**
	   * current initiator rate , current video message id in RTP stream and current frame per second for RTP video index
	   */
	  unsigned currInitRate, currVidMsg, currInitFPSindex;

	  // number of flow control messages received in round (for synchronization)
	  unsigned int flowControlMessagesReceivedInRound;

	  /**
	   * set RTP pool for transmit
	   * @param parent parent application
	   * @param rtpFps frame per second for RTP
	   * @param time time in video
	   * @param useHighest if to use heighest rate possible, otherwise adapt from current rate
	   */ 
	  bool setRtpPool(VideoStreamAppl * parent, int rtpFps, double time,
			  bool useHighest = false);

	  /**
	   * change rate of transmit
	   * @param parent parent application
	   * @param rate rate index in vector pool
	   * @param fps frame per second for RTP
	   * @param time time in video to start from   
	   */
	  void changeRate(VideoStreamAppl * parent, int rate, int fps,
			  double time);
	  
	};

 private:
	typedef map<int, Stream*> Streams;

	/**
	 * Streams collection data per this node/application
	 */
	Streams streams;

	/**
	 * auto set the RTP pool for stream and time
	 * @param stream number of stream to set rate
	 * @param time time to set start of video pool to
	 */
	void setRtpPool(int stream, double time = SIMTIME_DBL(simTime()));

public:
	/**
	 * @return Stream element of application
	 * @param stream stream number
	 */
	const Stream * getStream(int stream) const {
		map<int,Stream*>::const_iterator it = streams.find(stream);
		if (it!=streams.end())
			return it->second;
			return NULL;
	}

	/**
	 * set node as initiator of stream
	 */
	void setInitiator(int stream);

	/**
	 * @return application packet which is of initiator (camera)
	 * @param stream number of stream packet belongs to
	 * @param slot number of slot
	 * @param channel channel to send in
	 * @param bits size in bits of packet
	 * @param id unique id of message
	 */
	ApplPkt * getInitiatorMessage(int stream, int slot, int channel, unsigned& bits,unsigned id);

};

#endif // VID_STREAM_APPL_LAYER_H
