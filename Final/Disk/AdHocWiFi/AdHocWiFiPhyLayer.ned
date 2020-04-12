//****************************************************************************
// * file:        AdHocWiFiPhyLayer.ned
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

import org.mixim.modules.phy.PhyLayer;

simple AdHocWiFiPhyLayer extends PhyLayer
{
    parameters:
        @class("AdHocWiFiPhyLayer");

	double centerFrequency  @unit(Hz);
	
	double pathloss_alpha;
	double pathloss_beta;
	double pathloss_c;
	double pathloss_htr;
	double pathloss_hrc;
	string pathloss_model;
	double external_PER;
}
