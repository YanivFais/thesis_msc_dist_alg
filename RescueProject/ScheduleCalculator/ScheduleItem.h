#pragma once


class ScheduleItem
{
public:
	ScheduleItem();
	ScheduleItem(int i_vertexNum, int i_edgeNum, int i_requestNum, int i_freqNum, 
					int i_timeSlot, bool i_sender, double i_totalFlow, double i_apr);
	~ScheduleItem(void);

	int GetEdgeNum () const;
	int GetRequestNum () const;
	int GetFreqNum () const;
	int GetVertexNum () const;
	int GetTimeSlot () const;
	double GetTotalFlow() const;
	double GetAPR () const;

	bool isSender () const;

protected:
	int edgeNum;
	int requestNum;
	int freqNum;
	int vertexNum;
	int timeSlot;
	bool sender;
	double totalFlow; 
	double apr;
};



