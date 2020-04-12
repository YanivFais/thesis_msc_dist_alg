/*
 * Schedule.cpp
 *
 *  Created on: Jun 23, 2010
 *      Author: Yaniv Fais <yaniv.fais@gmail.com>
 */

#include "ScheduleItemCollection.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <stdlib.h>

ScheduleItemCollection::ScheduleItemCollection() {


}

ScheduleItemCollection::~ScheduleItemCollection() {

}

int ScheduleItemCollection::readFromFile(const string& fileName)
{
	ifstream schedFile;
	schedFile.open(fileName.c_str());

    if (!schedFile)
	{
		cout << "Unable to open file" << fileName << endl;
        return false;
    }

	string line;
    while (getline(schedFile,line))
    {
		if ( (line.size() == 0) ||
             ((line.size() >= 1) && (line[0] == '#') ))
        {
			continue;  // comment
        }
        else
        {
			stringstream ss (line);
			string token;
            getline(ss, token, ',');

            // format: timeSlot+1, freqNum, vertexNum1,requestNum,1,vertexNum2, MCS, flowInSlot
            int slot = atoi(token.c_str());
            getline(ss, token, ',');
            int freq = atoi(token.c_str());
            getline(ss, token, ',');
			int transmitter = atoi(token.c_str());
            getline(ss, token, ',');
            int streamNumber = atoi(token.c_str());
            getline(ss, token, ',');
			int recieversNumber = atoi(token.c_str());
			assert(recieversNumber==1);
            getline(ss, token, ',');
			int reciever = atoi(token.c_str());
            getline(ss, token, ',');
			int MCS = atoi(token.c_str());
            getline(ss, token, ',');
			double totalFlow = atof(token.c_str());


			ScheduleItemSPtr task(new ScheduleItem(transmitter,reciever,0,streamNumber,freq,slot,MCS,totalFlow));
			Node2StreamMap::iterator nIter = schedule.find(transmitter);
			if (nIter==schedule.end()) {
				schedule[transmitter] = Stream2SchedulesMap();
				nIter = schedule.find(transmitter);
			}

			Stream2SchedulesMap::iterator sIter = nIter->second.find(streamNumber);
			if (sIter == nIter->second.end()) {
				(nIter->second)[streamNumber] = NodeSchedules();
				sIter = nIter->second.find(streamNumber);
			}
			sIter->second.push_back(task);
		}
	}

    schedFile.close();
	return true;
}

ScheduleItemCollection::Stream2SchedulesMap ScheduleItemCollection::getSenderSchedule(int sender)
{
	Node2StreamMap::const_iterator it = schedule.find(sender);
	return (it==schedule.end()) ? Stream2SchedulesMap() : it->second;
}

void ScheduleItemCollection::print()
{
	for (Node2StreamMap::const_iterator it = schedule.begin(); it != schedule.end(); it++) {
		cout << " Node " << it->first << endl;
		for (Stream2SchedulesMap::const_iterator sit = it->second.begin(); sit != it->second.end(); sit++) {
			cout << "\tStream " << sit->first ;
			for (NodeSchedules::const_iterator nit = sit->second.begin(); nit != sit->second.end(); nit++) {
				cout << "@" << nit->get()->GetTimeSlot() << ":" << nit->get()->GetSender() << "(" << nit->get()->GetRequestNum() << ")";
			}
			cout << endl;
		}
	}
}
