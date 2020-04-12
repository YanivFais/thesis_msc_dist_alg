#include "DynamicVertex.h"

DynamicVertex::DynamicVertex(int i_vertexNum) : RescueVertex(i_vertexNum)
{}

DynamicVertex::DynamicVertex(int i_vertexNum, DblPoint X0, std::vector<DblPoint> & V, std::vector<double> & startTofV) 
	: RescueVertex(i_vertexNum)
{
	mX0 = X0;
	mVelocity = V;
	mStartTimesOfVelocity = startTofV;
}

DynamicVertex::DynamicVertex(int i_vertexNum, DblPoint X0, DblPoint V) : RescueVertex(i_vertexNum)
{
	mX0 = X0;
	mVelocity.push_back(V);
	mStartTimesOfVelocity.push_back(0.);
}

DynamicVertex::DynamicVertex(const DynamicVertex &other) :RescueVertex(other)
{
	mX0 = other.mX0;
	mVelocity = other.mVelocity;
	mStartTimesOfVelocity = other.mStartTimesOfVelocity;
}


void DynamicVertex::SetLocationAtZero(DblPoint X0)
{
	mX0 = X0;
}

DblPoint  DynamicVertex::GetLocationAtT(double t) const
{
	DblPoint X = mX0;
	if (mVelocity.size() == 0)
		return X;

	if (t < mStartTimesOfVelocity[0])
		return X;

	DblPoint V = mVelocity[0]; 
	int nLags = mVelocity.size();
	for (int i=1;i<nLags;i++)
	{
		if (t < mStartTimesOfVelocity[i])
		{
			X.x += ((t - mStartTimesOfVelocity[i-1])*V.x); 						
			X.y += ((t - mStartTimesOfVelocity[i-1])*V.y); 						
			return X;
		}

		X.x += ((mStartTimesOfVelocity[i] - mStartTimesOfVelocity[i-1])*V.x);
		X.y += ((mStartTimesOfVelocity[i] - mStartTimesOfVelocity[i-1])*V.y);
		V = mVelocity[i];
	}

	X.x += ((t - mStartTimesOfVelocity[nLags-1])*V.x); 						
	X.y += ((t - mStartTimesOfVelocity[nLags-1])*V.y); 						
	return X;
}

DblPoint  DynamicVertex::GetVelocityAtT(double t) const
{
	DblPoint V(0.,0.);
	if (mVelocity.size() == 0) 
		return V;

	int nLags = mVelocity.size();
	for (int i=0;i<nLags;i++)
	{
		if (t < mStartTimesOfVelocity[i])
			return V;
		V = mVelocity[i];
	}

	return V;
}
