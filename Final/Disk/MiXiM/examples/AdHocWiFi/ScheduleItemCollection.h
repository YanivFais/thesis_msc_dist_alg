/*
 * ScheduleItemCollection.h
 *
 *  Created on: Jun 23, 2010
 *      Author: yaniv fais <yaniv.fais@gmail.com>
 */

#ifndef SCHEDULE_ITEM_COLLECTION_H_
#define SCHEDULE_ITEM_COLLECTION_H_

#include "ScheduleItem.h"
#include <tr1/memory>
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

	/**
	 * @return number of maximum slot used in schedule
	 */
	int getMaxSlot();

	void print();

private:

	Node2StreamMap schedule;

};

#endif /* SCHEDULE_ITEM_COLLECTION_H_ */
