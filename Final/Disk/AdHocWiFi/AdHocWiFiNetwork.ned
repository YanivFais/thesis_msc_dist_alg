package org.mixim.examples.AdHocWiFi;

import org.mixim.base.connectionManager.ConnectionManager;
import org.mixim.base.modules.BaseWorldUtility;
import org.mixim.modules.mobility.LinearMobility;


module AdHocWiFiNetwork
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
	int    numNodes; // number of nodes in network

        @display("bgb=$playgroundSizeX,$playgroundSizeY,white,,;bgp=0,0");
    submodules:
        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                playgroundSizeZ = playgroundSizeZ;
                @display("p=30,0;i=misc/globe");
        }
        channel1: AdHocWiFiConnectionManager { 
         	parameters:
                @display("p=250,0;b=42,42,rect,green,,;i=abstract/multicast");   
        }
        channel2: AdHocWiFiConnectionManager {
         	parameters:
                @display("p=350,0;b=42,42,rect,red,,;i=abstract/multicast");   
        }
        channel3: AdHocWiFiConnectionManager {
         	parameters:
                @display("p=450,0;b=42,42,rect,blue,,;i=abstract/multicast");   
        }

        channel1_node[numNodes]: AdHocWiFiNode {
            parameters:
                @display("p=50,50;i=device/wifilaptop");
        }
        channel2_node[numNodes]: AdHocWiFiNode {
            parameters:
                @display("p=50,50;i=device/wifilaptop");
        }
        channel3_node[numNodes]: AdHocWiFiNode {
            parameters:
                @display("p=50,50;i=device/wifilaptop");
        }

    connections allowunconnected:

}



network sim extends AdHocWiFiNetwork
{
    parameters:
}

