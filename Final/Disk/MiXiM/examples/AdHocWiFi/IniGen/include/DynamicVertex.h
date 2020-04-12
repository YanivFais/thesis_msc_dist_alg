#pragma once

#include "DblPoint.h"
#include "RescueVertex.h"
#include <vector>

class DynamicVertex : public RescueVertex
{
public:
	DynamicVertex(int i_vertexNum, DblPoint X0, DblPoint V);
	DynamicVertex(int i_vertexNum, DblPoint X0, std::vector<DblPoint> & V, std::vector<double> & startTofV);
	DynamicVertex(const DynamicVertex & other);
	DynamicVertex(int i_vertexNum);

	void      SetLocationAtZero(DblPoint X0);

	DblPoint  GetLocationAtT(double t) const;
	DblPoint  GetVelocityAtT(double t) const;

private:
	DblPoint                mX0;
	std::vector<DblPoint>   mVelocity;
	std::vector<double>     mStartTimesOfVelocity;
};
