//
// Copyright (C) 2006 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//


import
    "MobileHost2",
    "ChannelControl";


module Net80211_2
    parameters:
        numHosts: numeric const,
        playgroundSizeX: numeric const,
        playgroundSizeY: numeric const;
    submodules:
        host1: MobileHost2;
	    parameters:
		IPForward=false,
		routingFile="mchost1.mrt";
	    display: "i=device/pocketpc_s;r=,,#707070";
        host2: MobileHost2;
	    parameters:
		IPForward=false,
		routingFile="mchost2.mrt";
	    display: "i=device/pocketpc_s;r=,,#707070";
//        host3: MobileHost2;
//	    parameters:
//		IPForward=false,
//		routingFile="mchost3.mrt";
//	    display: "i=device/pocketpc_s;r=,,#707070";



        channelcontrol: ChannelControl;
            parameters:
                playgroundSizeX = playgroundSizeX,
                playgroundSizeY = playgroundSizeY;
            display: "p=0,0;i=misc/sun";
//        configurator: FlatNetworkConfigurator;
//            parameters:
//                moduleTypes = "MobileHost2",
//                nonIPModuleTypes = "",
//                networkAddress = "145.236.0.0",
//                netmask = "255.255.0.0";
//            display: "p=140,50;i=block/cogwheel_s";
    connections nocheck:
endmodule

network net80211_2 : Net80211_2
endnetwork

