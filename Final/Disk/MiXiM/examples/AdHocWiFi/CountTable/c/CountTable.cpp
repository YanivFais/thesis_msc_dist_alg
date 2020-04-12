#include <ScheduleItemCollection.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;

int count(vector<int>& v,int & scheduled,int & total,int & joined)
{
	for (int i=0;i<v.size();i++) {
		if (v[i]!=0)
			scheduled++;
		if (v[i]>1)
			joined++;
		total += v[i];
	}
}

int main(int argc,char * argv[])
{
  ScheduleItemCollection sic;
  sic.readFromFile(argv[1]);

  vector<int> c1(1000),c2(1000),c3(1000);
  for (int i =0; i < atoi(argv[2]); i++) {
    ScheduleItemCollection::Stream2SchedulesMap smap = sic.getSenderSchedule(i);
    for (ScheduleItemCollection::Stream2SchedulesMap::iterator it = smap.begin(); it != smap.end(); it++) {
    	for (ScheduleItemCollection::NodeSchedules::iterator scIter = (*it).second.begin(); scIter != (*it).second.end(); scIter++) {
    		int t = ((*scIter)->GetTimeSlot());
    		int c = (*scIter)->GetFreqNum();
    		if (c==1)
    			c1[t]++;
    		else if (c==2)
    			c2[t]++;
			else if (c==3)
				c3[t]++;
    	}
    }
  }

  int scheduled=0,total=0,joined=0;
  count(c1,scheduled,total,joined);
  cout << "Total scheduled " << scheduled << " with total " << total  << " sends and joined " << joined << endl;
  count(c2,scheduled,total,joined);
  cout << "Total scheduled " << scheduled << " with total " << total  << " sends and joined " << joined << endl;
  count(c3,scheduled,total,joined);
  cout << "Total scheduled " << scheduled << " with total " << total  << " sends and joined " << joined << endl;
}
