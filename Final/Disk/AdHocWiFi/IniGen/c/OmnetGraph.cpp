/*
 * OmnetGraph.cpp
 *
 *  Created on: Jun 12, 2010
 *      Author: yaniv fais <yaniv.fais@gmail.com>
 */

#include "OmnetGraph.h"
#include "RescueVertex.h"
#include "DynamicVertex.h"
#include "ScheduleItemCollection.h"

#include <stdlib.h>
#include <iostream>
#include <math.h>

OmnetGraph::OmnetGraph() {
}

OmnetGraph::~OmnetGraph() {

}

void OmnetGraph::writeOmnetppIni(int scale,ostringstream& str,ScheduleItemCollection & schedule,ostringstream & mapping,bool performMap)
{
	map<int,RescueVertex*> vertices = GetVertices();
	int i = 1;
	int iMap = 1;
	int size = performMap ? schedule.getActiveSchedulers()+1 : (vertices.size()+1);
	str << "sim.numNodes = " << size << endl << endl;

	for (map<int,RescueVertex*>::const_iterator iter = vertices.begin(); iter != vertices.end(); iter++,i++) {
		int node = i;
		if (performMap) {
			if (schedule.getSenderSchedule(i).empty())
				continue;
			mapping << i << " -> " << iMap << endl;
			node = iMap++;
		}
		DynamicVertex * v = ((DynamicVertex*)iter->second);
		DblPoint p = v->GetLocationAtT(0);
		DblPoint velocity = v->GetVelocityAtT(0);
		double angle = atan2(velocity.y,velocity.x) * 180 / 3.14;
		if (angle<0)
			angle += 360;
		double speed = sqrt(velocity.x*velocity.x + velocity.y*velocity.y); 
		speed *= scale;
		speed /= 3600; // hour -> per second

		if (i==1) { // we put node 0 (dummy node just since in omnet the nodes start from 0) in same place as node 1
			str << "sim.channel*node[0].mobility.x = " << p.x << endl;
			str << "sim.channel*node[0].mobility.y = " << p.y << endl;
			str << "sim.channel*node[0].mobility.angle = " << (int)angle << "deg\n";
			str << "sim.channel*node[0].mobility.speed = " << speed << "mps\n\n\n\n";
		}
		str << "sim.channel*node[" << node << "].mobility.x = " << p.x << endl;
		str << "sim.channel*node[" << node << "].mobility.y = " << p.y << endl;
		str << "sim.channel*node[" << node << "].mobility.speed = " <<  speed << "mps" << endl << endl;
		str << "sim.channel*node[" << node << "].mobility.angle = " << (int) angle <<  "deg" << endl;

	}
}
