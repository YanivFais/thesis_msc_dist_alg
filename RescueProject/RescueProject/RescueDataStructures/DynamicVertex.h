#pragma once

#include "DblPoint.h"
#include "RescueVertex.h"

class DynamicVertex : public RescueVertex
{
public:
	DynamicVertex(int i_vertexNum, DblPoint X0, DblPoint V);
	DynamicVertex(const DynamicVertex & other);
	DynamicVertex(int i_vertexNum);

	void      SetLocationAtZero(DblPoint X0);
	void      SetVelocity (DblPoint V);

	DblPoint  GetLocationAtT(double t) const;
	DblPoint  GetVelocity() const;

private:
	DblPoint   mX0;
	DblPoint   mVelocity;
};
