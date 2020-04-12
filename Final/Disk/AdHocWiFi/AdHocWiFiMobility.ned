//****************************************************************************
// * file:        AdHocWiFiApplMobility.ned
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


import org.mixim.base.modules.BaseMobility;

//
// This is a mobility model from file of x,y,vx,vy
simple AdHocWiFiMobility extends BaseMobility
{
    parameters:
        @class(AdHocWiFiMobility);
        bool debug; // debug switch
	string coords_file;
	int coords_number;
        double updateInterval @unit(s); // time interval to update the hosts position (seconds)
}

