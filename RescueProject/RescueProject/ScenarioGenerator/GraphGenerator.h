#pragma once

#include "CommunicationGraph.h"
#include "InterferenceGraph.h"
#include "DynamicVertex.h"
#include "CommunicationEdge.h"
#include "CommunicationRequest.h"
#include "RequestsCollection.h"

#include "GenerationParams.h"
#include "PER2SNRTable.h"
#include "RateCalculator.h"

#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


class GraphGenerator
{
public:
	GraphGenerator(GenerationParams & params, const RateCalculator & rateCalculator);
	~GraphGenerator();

	void GenerateGraphs(CommunicationGraph *& communicationGraph, InterferenceGraph *& interferenceGraph);
	RequestsCollection & GenerateRequests(CommunicationGraph & graph);
	void WriteRequestsFile(RequestsCollection & requests);
	void WriteGraphFiles(const CommunicationGraph & communicationGraph,const InterferenceGraph & interferenceGraph);

protected:
	GenerationParams     & mParams;
	const RateCalculator & mRateCalculator;

	void                GenerateRandomVertices(CommunicationGraph * pGraph, InterferenceGraph * interferenceGraph);
	void                AddEdgesToGraphs      (CommunicationGraph * pGraph, InterferenceGraph * pInterferenceGraph);
	std::pair<CommunicationEdge *,RescueEdge *> CalcEdgeBetweenNodes  (DynamicVertex * pFrom, DynamicVertex * pTo, int nEdgesAddedToCommunication, int nEdgesAddedToInterference);
	CommunicationRequest GenerateRequest(int requestNum, CommunicationGraph & graph, double farThreshold, bool isBelow);

protected:
	bool          mDebug;
	std::string   mDebugFileName;
	std::ofstream mDebugFile;
};
