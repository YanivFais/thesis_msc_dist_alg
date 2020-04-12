#pragma once
#include "SchedulingParams.h"
#include "CommunicationGraph.h"
#include "InterferenceGraph.h"
#include "RequestsCollection.h"

class LPInterface
{
public:
	LPInterface(void);
	bool SolveLP(CommunicationGraph & commGraph, InterferenceGraph & interGraph, RequestsCollection & requests, 
					SchedulingParams & params, double *& solution);
};
