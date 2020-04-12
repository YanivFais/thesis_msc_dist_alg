/*
 * ScheduleItemCollection.h
 *
 *  Created on: Jun 23, 2010
 *      Author: yaniv fais <yaniv.fais@gmail.com>
 */

#ifndef SCHEDULE_ITEM_COLLECTION_H_
#define SCHEDULE_ITEM_COLLECTION_H_

#include "ScheduleItem.h"
#ifndef __GNUC__
  #include <memory>
#else
  #include <tr1/memory>
#endif
#include <string>
#include <list>
#include <map>

using namespace std;

typedef std::tr1::shared_ptr<ScheduleItem> ScheduleItemSPtr;

class ScheduleItemCollection {
public:
	typedef list<ScheduleItemSPtr> NodeSchedules;
	typedef map<int,NodeSchedules> Stream2SchedulesMap;
	typedef map<int,Stream2SchedulesMap> Node2StreamMap;

	ScheduleItemCollection();
	virtual ~ScheduleItemCollection();

	int readFromFile(const string& fileName);

	Stream2SchedulesMap getSenderSchedule(int sender);

	int getActiveSchedulers() { return schedule.size(); }

	void print();

private:

	Node2StreamMap schedule;

};

#endif /* SCHEDULE_ITEM_COLLECTION_H_ */
