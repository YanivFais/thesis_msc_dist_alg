/*********************************************************
 * File:        AdHocWiFiConnectionManager.cpp
 *
 * Author:      Yaniv Fais
 *
 *
 ***************************************************************************
 * description: manages connections in the graph
**************************************************************************/


#include "AdHocWiFiConnectionManager.h"
#include <iostream>
using namespace std;
#include <stdlib.h>

Define_Module( AdHocWiFiConnectionManager );


AdHocWiFiConnectionManager::AdHocWiFiConnectionManager() {

}

AdHocWiFiConnectionManager::~AdHocWiFiConnectionManager() {
}

void AdHocWiFiConnectionManager::checkConnections()
{
#ifdef DEBUG_CONNECTIVITY
	FOR_EACH_C (NicEntries,nicIter,nics) {

		int id = nicIter->first;
		NicEntry* nic = nicIter->second;
		Coord cell = (*nicIter->second).pos;

		FOR_EACH_C (NicEntries,nicDestIter,nics) {
			 NicEntry* nic_i = nicDestIter->second;

			 // no recursive connections
			 if ( nic_i->nicId == id ) continue;
			 if (reported[nic_i->hostId][nic->hostId]==true) continue;

			 double distance;

			 if(useTorus)
			 {
			   distance = nic->pos.sqrTorusDist(nic_i->pos, playgroundSize);
			 } else {
			   distance = nic->pos.sqrdist(nic_i->pos);
			 }

			 bool inRange = (distance <= maxDistSquared);
			 bool connected = nic->isConnected(nic_i);

			 reported[nic_i->hostId][nic->hostId] = true;
			 cerr << "host #" << nic->hostId << "-> " << nic_i->hostId << " in range=" << inRange << " connected=" << connected << endl;
			 if ( inRange && !connected ){
				 // nodes within communication range: connect
				 // nodes within communication range && not yet connected
				 cerr << "host #" << id << " and #" << nic_i->nicId << " are in range" << endl;
			 }
			 else if ( !inRange && connected ){
				 // out of range: disconnect
				 // out of range, and still connected
				 cerr << "nic #" << id << " and #" << nic_i->nicId << " are NOT in range" << endl;
			 }
			 else if (inRange && connected) {
				 cerr << "nic #" << id << " and #" << nic_i->nicId << " are connected and in range" << endl;
			 }
			 else if (!inRange && !connected) {
				 cerr << "nic #" << id << " and #" << nic_i->nicId << " are NOT connected and in NOT range" << endl;
			 }
		 }
	}
#endif
 }


double AdHocWiFiConnectionManager::calcInterfDist()
{
  return 2000;
}
