#pragma once
#include <string>
#include "RescueEdge.h"

class CommunicationEdge : public RescueEdge 
{
public:
	CommunicationEdge(int edgeNum, int from, int to, double capacity, 
		double PER, int MCS);

	double               GetPER           () const;
	int                  GetMCS           () const;


private:
	double       mPER;
	int          mMCS;
};
