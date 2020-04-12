#include "CommunicationEdge.h"
using namespace std;

// the communication graph is undirected - therefore the flag is false.
CommunicationEdge::CommunicationEdge(int edgeNum, int from, int to, double capacity, 
									 double PER, int MCS) :
				 RescueEdge(edgeNum, from, to, false, capacity) 
{
	mPER = PER;
	mMCS = MCS;
}

double CommunicationEdge::GetPER() const
{
	return mPER;
}

int CommunicationEdge::GetMCS() const
{
	return mMCS;
}

