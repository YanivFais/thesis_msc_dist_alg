#include "Rescueedge.h"

RescueEdge::RescueEdge(int i_edgeNum, int i_from, int i_to, bool i_isDirected, double i_capacity) 
{
	mEdgeNum = i_edgeNum;
	mFrom = i_from;
	mTo = i_to;
	mIsDirected = i_isDirected;
	mCapacity = i_capacity;
}

bool RescueEdge::GetIsDirected () const
{
	return mIsDirected;
}

int RescueEdge::GetEdgeNum() const
{
	return mEdgeNum;
}

double RescueEdge::GetCapacity() const
{
	return mCapacity;
}

int RescueEdge::GetFrom () const
{
	return mFrom;
}

int RescueEdge::GetTo   () const
{
	return mTo;
}

void RescueEdge::SetCapacity   (double newCapacity)
{
	mCapacity = newCapacity;
}


