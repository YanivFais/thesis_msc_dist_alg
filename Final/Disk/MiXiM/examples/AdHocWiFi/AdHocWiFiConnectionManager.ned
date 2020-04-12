//***************************************************************************
// * file:        AdHocWiFiConnectionManager.ned
// *
// *
// *              This program is free software; you can redistribute it
// *              and/or modify it under the terms of the GNU General Public
// *              License as published by the Free Software Foundation; either
// *              version 2 of the License, or (at your option) any later
// *              version.
// *              For further information see file COPYING
// *              in the top level directory
// **************************************************************************/


package org.mixim.examples.AdHocWiFi;

// Module to control all connection related stuff
//
// The central module that coordinates the connections between all
// nodes, and handles dynamic gate creation.  ConnectionManager therefore
// periodically communicates with the mobility module and
// ChannelAccess.
//
// The four parameters pMax, sat, alpha, and carrierFrequency are used
// to calculate the interference distance between nodes. The values used
// here in ConnectionManager are used to calculate the upper bound,
// i.e. they can be redifined in the analogue models, but never such
// that the maximal interference distance is exeeded.
//       
// @see BasicMobility
//
simple AdHocWiFiConnectionManager
{
    parameters:
        // debug switch for core framework
        bool coreDebug;
        // send directly to the node or create separate gates for every connection
        bool sendDirect;
        // maximum sending power used for this network [mW]
        double pMax @unit(mW);
        // minimum signal attenuation threshold [dBm]
        double sat @unit(dBm);
        // minimum path loss coefficient
        double alpha;
        // minimum carrier frequency of the channel [Hz]
        double carrierFrequency @unit(Hz);
}

