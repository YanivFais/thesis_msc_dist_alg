#ifndef __AD_HOC_WI_FI
#define __AD_HOC_WI_FI
/*********************************************************
 * File:        AdHocWiFi.h
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: global definitions for the project
**************************************************************************/

// version definition
#define AD_HOC_WI_FI_MAJOR 5
#define AD_HOC_WI_FI_MINOR 1

#define FOR_EACH(type,iter,collection) for(type::iterator iter=(collection).begin();iter!=(collection).end();iter++)
#define FOR_EACH_C(type,iter,collection) for(type::const_iterator iter=(collection).begin();iter!=(collection).end();iter++)

#include <list>
#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;

typedef list<int> IntList;
typedef vector<int> IntVector;
typedef vector<double> DoubleVector;
typedef map<int,int> Int2IntMap;
typedef map<int,double> Int2RealMap;
typedef set<int> IntSet;
typedef map<int,IntSet> Int2IntSetMap;
typedef list<string> StringList;
typedef pair<int,int> IntPair;

#define PKT_IN_QUEUE 0.01
#define PKT_DROPPED -1.0
#define PKT_LOST 0
#define SLOT_Q_SAMPLE 5
#define DESTINATION_REQUEST_MULTIPLIER 1
#define NUMBER_OF_CHANNELS 3

#define DEBUG_OUT  EV
#define DEBUG_MOVE(x)
#define DEBUG_MSG(x) 
//cout << x
//EV << x

#endif // __AD_HOC_WI_FI
