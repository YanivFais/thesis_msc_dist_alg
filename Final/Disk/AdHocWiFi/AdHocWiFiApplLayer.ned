//****************************************************************************
// * file:        AdHocWiFiApplLayer.ned
// *
// * author:      Yaniv Fais
// *
// *              This program is free software; you can redistribute it 
// *              and/or modify it under the terms of the GNU General Public 
// *              License as published by the Free Software Foundation; either
// *              version 2 of the License, or (at your option) any later 
// *              version.
// *              For further information see file COPYING 
// *              in the top level directory
// ***************************************************************************
// ***************************************************************************/
package org.mixim.examples.AdHocWiFi;

import org.mixim.base.modules.IBaseApplLayer;

// Application layer to test lower layer implementations
//
// This application layer does exactly the same as the
// TestApplLayer. The only difference is that is sends a burst of
// broadcast messages instead of just one. The burst size is specified
// in burstSize and can be set in omnetpp.ini
//
// @sa TestApplALyer
// @author Yaniv Fais
simple AdHocWiFiApplLayer like IBaseApplLayer
{
    parameters:
	// if true then collect statistics on RTP
        bool rtpStatistics;

	// if 1 enable arbitration between queues when sending
        bool enableArbitration;

	bool one_slot; // if true would run in one slot

	bool adjustRoundTime; // if true adjust the round time as max slots*expected slot time

        // debug switch
        bool debug; // debug switch
        // length of the application message header (in bits)
        // length of the application message header (in bits)
        int headerLength @unit("bit"); // length of the application message header (in bits)
        // number of rounds for doing all slots
        // number of rounds for doing all slots
        int rounds; // number of rounds for doing all slots
        // number of slots
        int slots; // number of slots
        // time of round
        double slotMaxTime @unit(s); // time of round
        // name of schedule file
        string schedule_file; // name of schedule file
        // name of requests file
        string requests_file; // name of requests file
        // number of channels per video node
        int channels; // number of channels per video node
        // base name of RTP file
        string rtpName; // base name of RTP file
        // number of RTP files per rate
        int rtpNumber; // number of RTP files per rate
        // mode of data - video/raw
        string mode; // mode of data - video/raw
        // mode of statistics - part/full
        string statisticsMode;

    gates:
        input lowerGateIn; // from network layer
        output lowerGateOut; // to network layer
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer
}
