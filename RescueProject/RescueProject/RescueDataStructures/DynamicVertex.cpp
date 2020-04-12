#include "DynamicVertex.h"

DynamicVertex::DynamicVertex(int i_vertexNum) : RescueVertex(i_vertexNum)
{}

DynamicVertex::DynamicVertex(int i_vertexNum, DblPoint X0, DblPoint V) : RescueVertex(i_vertexNum)
{
	mX0 = X0;
	mVelocity = V;
}

DynamicVertex::DynamicVertex(const DynamicVertex &other) :RescueVertex(other)
{
	mX0 = other.mX0;
	mVelocity = other.mVelocity;
}


void DynamicVertex::SetLocationAtZero(DblPoint X0)
{
	mX0 = X0;
}

void DynamicVertex::SetVelocity (DblPoint V)
{
	mVelocity = V;
}

DblPoint  DynamicVertex::GetLocationAtT(double t) const
{
	DblPoint result(mX0.x + t*mVelocity.x, mX0.y + t*mVelocity.y);
	return result;
}

DblPoint  DynamicVertex::GetVelocity() const
{
	return mVelocity;
}
