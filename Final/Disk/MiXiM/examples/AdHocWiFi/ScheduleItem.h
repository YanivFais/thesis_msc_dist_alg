#pragma once

#include <string>

class ScheduleItem
{
public:
	ScheduleItem();
	ScheduleItem(int i_vertexNum1, int i_vertexNum2, int i_edgeNum, int i_requestNum, int i_freqNum, 
					int i_timeSlot, int i_MCS, double i_flowInSlot);
	~ScheduleItem(void);

	std::string ToString() const;
	int GetEdgeNum () const;
	int GetRequestNum () const;
	int GetFreqNum () const;
	int GetSender  () const;
	int GetReciever () const;
	int GetTimeSlot () const;
	int GetOrigMCS() const { return origMCS; }
	int GetMCS() const;
	void SetMCS(int newMCS) { MCS = newMCS; }
	double GetFlowInSlot() const;
	void SetFlowInSlot(double f) { flowInSlot = f; }

	double GetWeight() const { return weight; }
	void SetWeight(double w) { weight=w; }

protected:
	int edgeNum;
	int requestNum;
	int freqNum;
	int vertexNum1;
	int vertexNum2;
	int timeSlot;
	int MCS;
	int origMCS;
	double flowInSlot;
	double weight;
};



