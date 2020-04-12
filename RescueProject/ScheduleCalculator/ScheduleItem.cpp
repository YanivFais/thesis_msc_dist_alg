#include "stdafx.h"
#include "ScheduleItem.h"	

ScheduleItem::ScheduleItem()
{
	edgeNum = -1;
	requestNum = -1;
	freqNum = -1;
	vertexNum = -1;
	timeSlot = -1;
	sender = false;
	totalFlow = 0.;
	apr = 0.;
}

ScheduleItem::ScheduleItem(int i_vertexNum, int i_edgeNum, int i_requestNum, int i_freqNum, int i_TimeSlot, bool i_sender, double i_totalFlow, double i_apr)
{
	edgeNum = i_edgeNum;
	vertexNum = i_vertexNum;
	requestNum = i_requestNum;
	freqNum = i_freqNum;
	timeSlot = i_TimeSlot;
	sender = i_sender;
	totalFlow = i_totalFlow; 
	apr = i_apr;
}

ScheduleItem::~ScheduleItem(void)
{}

double ScheduleItem::GetTotalFlow() const
{
	return totalFlow;
}

double ScheduleItem::GetAPR () const
{
	return apr;
}

int ScheduleItem::GetEdgeNum () const
{
	return edgeNum;
}

int ScheduleItem::GetRequestNum () const
{
	return requestNum;
}

int ScheduleItem::GetFreqNum () const
{
	return freqNum;
}

int ScheduleItem::GetVertexNum () const
{
	return vertexNum;
}

int ScheduleItem::GetTimeSlot () const
{
	return timeSlot;
}

bool ScheduleItem::isSender () const
{
	return sender;
}

