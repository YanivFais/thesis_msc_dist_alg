/*********************************************************
 * File:        AdHocWiFiConnectionManager.h
 *
 * Author:      Yaniv Fais
 *
 *
 ***************************************************************************
 * description: manages connections in the graph
**************************************************************************/

#ifndef ADHOCWIFICONNECTIONMANAGER_H_
#define ADHOCWIFICONNECTIONMANAGER_H_


#include "ConnectionManager.h"
#include "AdHocWiFi.h"

/**
 * connection manager for the "world" between different nodes
 * override to perform checks on model
 */
class AdHocWiFiConnectionManager: public ConnectionManager {
public:
	AdHocWiFiConnectionManager();
	virtual ~AdHocWiFiConnectionManager();

	void checkConnections();

	virtual double calcInterfDist();

private:
	map<int,map<int,bool> > reported;
};

#endif /* ADHOCWIFICONNECTIONMANAGER_H_ */
