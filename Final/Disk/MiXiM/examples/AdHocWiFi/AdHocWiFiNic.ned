package org.mixim.examples.AdHocWiFi;

import org.mixim.modules.mac.Mac80211;
import org.mixim.modules.phy.PhyLayer;

//
// This NIC implements an 802.11 network interface card with project phy/mac
//
// @see Mac80211, Decider80211
// @author Yaniv Fais
//
module AdHocWiFiNic 
{
    parameters:
        string connectionManagerName; 	//name of the ConnectionManager module

    gates:
        input upperGateIn; // to upper layers
        output upperGateOut; // from upper layers
        output upperControlOut; // control information 
        input upperControlIn; // control information 
		input radioIn; // radioIn gate for sendDirect

    submodules:
        mac: AdHocWiFiMac {
            @display("p=96,87;i=block/layer");
        }
        
        phy: AdHocWiFiPhyLayer {
            @display("p=106,157;i=block/process_s");
        }
		//radio: SingleChannelRadio;
	    // display: "p=200,30;b=30,25";

    connections:
        mac.upperGateOut --> { @display("ls=black;m=m,25,50,25,0"); } --> upperGateOut;
        mac.upperGateIn <-- { @display("ls=black;m=m,15,50,15,0"); } <-- upperGateIn;
        mac.upperControlOut --> { @display("ls=red;m=m,75,50,75,0"); } --> upperControlOut;
        mac.upperControlIn <-- { @display("ls=red;m=m,85,0,85,0"); } <-- upperControlIn;

        phy.upperGateOut --> { @display("ls=black;m=m,25,50,25,0"); } --> mac.lowerGateIn;
        phy.upperGateIn <-- { @display("ls=black;m=m,15,50,15,0"); } <-- mac.lowerGateOut;
        phy.upperControlOut --> { @display("ls=red;m=m,75,50,75,0"); } --> mac.lowerControlIn;
        phy.upperControlIn <-- { @display("ls=red;m=m,85,0,85,0"); } <-- mac.lowerControlOut;

        radioIn --> phy.radioIn;
}

