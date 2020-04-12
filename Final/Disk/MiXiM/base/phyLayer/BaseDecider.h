/*
 * BaseDecider.h
 *
 *  Created on: 24.02.2009
 *      Author: karl
 */

#ifndef BASEDECIDER_H_
#define BASEDECIDER_H_

#include "Decider.h"

#define debugEV (ev.isDisabled()||!debug) ? ev : ev << "[Host " << myIndex << "] - PhyLayer(Decider): "

/**
 * @brief Provides some base functionality for most common deciders.
 *
 * Forwards the AirFrame from "processSignal" to "processNewSignal",
 * "processSignalHeader" or "processSignalEnd" depending on the
 * state for that AirFrame returned by "getSignalState".
 *
 * Provides answering of ChannelSenseRequests (instantaneous and over time).
 *
 * Subclasses should define when they consider the channel as idle by
 * calling "setChannelIdleStatus" because BaseDecider uses that to
 * answer ChannelSenseRequests.
 *
 * If a subclassing Decider only tries to receive one signal at a time
 * it can use BaseDeciders "currentSignal" member which is a pair of
 * the signal to receive and the state for that signal. The state
 * is then used by BaseDeciders "getSignalState" to decide to which
 * "process***" method to forward the signal.
 * If a subclassing Decider needs states for more than one Signal it
 * has to store these states by itself and should probably override
 * the "getSignalState" method.
 *
 * @ingroup decider
 * @ingroup baseModules
 */
class BaseDecider: public Decider {
protected:
	enum SignalState{
		NEW, 			/** @brief Signal is received the first time. */
		EXPECT_HEADER,	/** @brief Waiting for the header of the signal. */
		EXPECT_END,		/** @brief Waiting for the end of the signal. */
	};

	/** @brief sensitivity value for receiving an AirFrame */
	double sensitivity;

	/** @brief Pair of a AirFrame and the state it is in. */
	typedef std::pair<AirFrame*, int> ReceivedSignal;

	/** @brief pointer to the currently received AirFrame */
	ReceivedSignal currentSignal;

	/** @brief Stores the idle state of the channel.*/
	bool isChannelIdle;

	/** @brief Pair of a ChannelSenseRequest and the simtime it started. */
	typedef std::pair<ChannelSenseRequest*, simtime_t> CSRInfo;

	/** @brief pointer to the currently running ChannelSenseRequest and its start-time */
	CSRInfo currentChannelSenseRequest;

	/** @brief index for this Decider-instance given by Phy-Layer (mostly Host-index)
	 * default-value for myIndex is -1, i.e. no parameter passed to constructor-call */
	int myIndex;

	/** @brief toggles display of debugging messages */
	bool debug;

public:
	BaseDecider(DeciderToPhyInterface* phy, double sensitivity, int myIndex, bool debug):
		Decider(phy),
		sensitivity(sensitivity),
		isChannelIdle(true),
		myIndex(myIndex),
		debug(debug)
	{
		currentSignal.first = 0;
		currentSignal.second = NEW;
		currentChannelSenseRequest = std::pair<ChannelSenseRequest*, simtime_t>(0, -1);
	}

	virtual ~BaseDecider() {}

public:
	/**
	 * @brief Processes an AirFrame given by the PhyLayer
	 *
	 * Returns the time point when the decider wants to be given the AirFrame again.
	 */
	virtual simtime_t processSignal(AirFrame* frame);

	/**
	 * @brief A function that returns information about the channel state
	 *
	 * It is an alternative for the MACLayer in order to obtain information
	 * immediately (in contrast to sending a ChannelSenseRequest,
	 * i.e. sending a cMessage over the OMNeT-control-channel)
	 */
	virtual ChannelState getChannelState();

	/**
	 * @brief This function is called by the PhyLayer to hand over a
	 * ChannelSenseRequest.
	 *
	 * The MACLayer is able to send a ChannelSenseRequest to the PhyLayer
	 * that calls this function with it and is returned a time point when to
	 * re-call this function with the specific ChannelSenseRequest.
	 *
	 * The Decider puts the result (ChannelState) to the ChannelSenseRequest
	 * and "answers" by calling the "sendControlMsg"-function on the
	 * DeciderToPhyInterface, i.e. telling the PhyLayer to send it back.
	 */
	virtual simtime_t handleChannelSenseRequest(ChannelSenseRequest* request);

protected:
	/**
	 * @brief Processes a new Signal. Returns the time it wants to
	 * handle the signal again.
	 *
	 * Default implementation checks if the signals receiving power
	 * is above the sensitivity of the radio and we are not already trying
	 * to receive another AirFrame. If thats the case it waits for the end
	 * of the signal.
	 */
	virtual simtime_t processNewSignal(AirFrame* frame);

	/**
	 * @brief Processes the end of the header of a received Signal.
	 *
	 * Returns the time it wants to handle the signal again.
	 *
	 * Default implementation does not handle signal headers.
	 */
	virtual simtime_t processSignalHeader(AirFrame* frame) {
		opp_error("BaseDecider does not handle Signal headers!");
		return notAgain;
	}

	/**
	 * @brief Processes the end of a received Signal.
	 *
	 * Returns the time it wants to handle the signal again
	 * (most probably notAgain).
	 *
	 * Default implementation just decides every signal as correct and passes it
	 * to the upper layer.
	 */
	virtual simtime_t processSignalEnd(AirFrame* frame);

	/**
	 * @brief Processes any Signal for which no state could be found.
	 * (is an error case).
	 */
	virtual simtime_t processUnknownSignal(AirFrame* frame) {
		opp_error("Unknown state for the AirFrame with ID %d", frame->getId());
		return notAgain;
	}

	/**
	 * @brief Returns the SignalState for the passed AirFrame.
	 *
	 * The default implementation checks if the passed AirFrame
	 * is the "currentSignal" and returns its state or if not
	 * "NEW".
	 */
	virtual int getSignalState(AirFrame* frame);

	/**
	 * @brief Handles a new incoming ChannelSenseRequest and returns the next
	 * (or latest) time to handle the request again.
	 */
	virtual simtime_t handleNewSenseRequest(ChannelSenseRequest* request);

	/**
	 * @brief Handles the timeout of a ChannelSenseRequest by calculating the
	 * ChannelState and returning the request to the mac layer.
	 */
	virtual void handleSenseRequestTimeout(CSRInfo& requestInfo);

	/**
	 * @brief Changes the "isIdle"-status to the passed value.
	 *
	 * This method further checks if there are any ChannelSenseRequests
	 * which can be answered because of the idle state changed.
	 */
	virtual void setChannelIdleStatus(bool isIdle);

	/**
	 * @brief Returns true if the ChannelSenseRequest of the passed CSRInfo can be answered
	 * (e.g. because channel state changed or timeout is reached).
	 */
	virtual bool canAnswerCSR(const CSRInfo& requestInfo);

	/**
	 * @brief Calculates the RSSI value for the passed ChannelSenseRequest.
	 *
	 * This method is called by BaseDecider when it answers a ChannelSenseRequest
	 * and can be overridden by sub classing Deciders.
	 *
	 * Default implementation gets start of the request from the passed CSRInfo
	 * and end of the request from the current simulation time (because the request
	 * is answered now) and returns the maximum RSSI value inside that
	 * maximum.
	 */
	virtual double calcChannelSenseRSSI(CSRInfo& requestInfo);

	/**
	 * @brief Answers the ChannelSenseRequest (CSR) from the passed CSRInfo.
	 *
	 * Calculates the rssi value and the channel idle state and sends the CSR
	 * together with the result back to the mac layer.
	 */
	virtual void answerCSR(CSRInfo& requestInfo);

	//------Utility methods------------

	/**
	 * @brief Calculates a SNR-Mapping for a Signal.
	 *
	 * A Noise-Strength-Mapping is calculated (by using the
	 * "calculateRSSIMapping()"-method) for the time-interval
	 * of the Signal and the Signal-Strength-Mapping is divided by the
	 * Noise-Strength-Mapping.
	 *
	 * Note: 'divided' means here the special element-wise operation on mappings.
	 *
	 */
	virtual Mapping* calculateSnrMapping(AirFrame* frame);

	/**
	 * @brief Calculates a RSSI-Mapping (or Noise-Strength-Mapping) for a Signal.
	 *
	 * This method can be used to calculate a RSSI-Mapping in case the parameter
	 * exclude is omitted OR to calculate a Noise-Strength-Mapping in case the
	 * AirFrame of the received Signal is passed as parameter exclude.
	 */
	virtual Mapping* calculateRSSIMapping(	simtime_t start,
											simtime_t end,
											AirFrame* exclude = 0);
};

#endif /* BASEDECIDER_H_ */
