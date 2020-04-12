#pragma once

class RescueEdge
{
public:
	 RescueEdge(int i_edgeNum,int i_from, int i_to, bool i_isDirected, double i_capacity);
    
	int    GetEdgeNum    () const;
	double GetCapacity   () const;
	int    GetFrom       () const;
	int    GetTo         () const;
	bool   GetIsDirected () const;

	void   SetCapacity   (double newCapacity);

protected:
	int     mEdgeNum;
	double  mCapacity;
	int		mFrom;
	int		mTo;
	bool    mIsDirected;
};
