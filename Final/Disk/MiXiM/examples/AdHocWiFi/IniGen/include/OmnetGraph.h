/*
 * OmnetGraph.h
 *
 *  Created on: Jun 12, 2010
 *      Author: Yaniv Fais <yaniv.fais@gmail.com>
 */

#ifndef OMNETGRAPH_H_
#define OMNETGRAPH_H_


#include <sstream>
using namespace std;

#include "ScheduleItemCollection.h"
#include "CommunicationGraph.h"

class OmnetGraph: public CommunicationGraph {
public:
	OmnetGraph();
	virtual ~OmnetGraph();

	void writeOmnetppIni(int scale,ostringstream& str,ScheduleItemCollection & schedule,
	                     ostringstream & mapping,bool performMap);
};

#endif /* OMNETGRAPH_H_ */
