#include "GraphGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>
#include <iostream>


using namespace std; 

GraphGenerator::GraphGenerator(GenerationParams & params, const RateCalculator & rateCalculator) :
	mParams(params), mRateCalculator(rateCalculator), 
	mDebug(true), mDebugFileName("debug_comm_graph.csv")
{
	if (mDebug)
	{
		mDebugFile.open(mDebugFileName.c_str(),ios_base::trunc);
	}
}

GraphGenerator::~GraphGenerator()
{
	if (mDebugFile && mDebug)
		mDebugFile.close();
}

void GraphGenerator::GenerateGraphs(CommunicationGraph *& communicationGraph, InterferenceGraph *& interferenceGraph)
{
	communicationGraph = new CommunicationGraph();
	interferenceGraph = new InterferenceGraph();

	GenerateRandomVertices(communicationGraph, interferenceGraph);
	AddEdgesToGraphs(communicationGraph,interferenceGraph); 
	return;
}

void GraphGenerator::AddEdgesToGraphs(CommunicationGraph * pGraph,InterferenceGraph * pInterferenceGraph)
{
	int nVertices = pGraph->GetNumVertices();

	// first calculate the edges in one direction (from -> to)
	vector<CommunicationEdge*> communicationEdgesToAdd;
	vector<RescueEdge*> interferenceEdgesToAdd;
	for (int i=0;i<(nVertices-1);i++)
	{
		DynamicVertex * pVertex_i = 
			(DynamicVertex *) pGraph->GetVertex(i+1);  // the vertex numbers are 1 to N and not 0 to N-1

		for (int j=(i+1);j<nVertices;j++)
		{
			DynamicVertex * pVertex_j = (DynamicVertex *) pGraph->GetVertex(j+1);
			pair<CommunicationEdge *, RescueEdge *> edges = 
				CalcEdgeBetweenNodes(pVertex_i,pVertex_j, 
//				communicationEdgesToAdd.size(), interferenceEdgesToAdd.size());
// this is not an error - I would like to have the interference edges and the 
// communication edges the same indices (so there may be "holes" in the 
// communication edges indices).
				interferenceEdgesToAdd.size(), interferenceEdgesToAdd.size());
			if (edges.first != NULL)
			{
				communicationEdgesToAdd.push_back(edges.first);
			}
			if (edges.second != NULL)
			{
				interferenceEdgesToAdd.push_back(edges.second);
			}
		}
	}

	// now add the edges and their reverse to the communication graph
	int nEdges = communicationEdgesToAdd.size();
// again this is because I want the interference and communication 
// edges have the same indices. 
	int offset = interferenceEdgesToAdd.size();
	for (int i=0;i<nEdges;i++)
	{
		CommunicationEdge * pEdge = communicationEdgesToAdd[i];
		CommunicationEdge * pReverseEdge = 
			new CommunicationEdge(pEdge->GetEdgeNum()+offset, pEdge->GetTo(),pEdge->GetFrom(),
			pEdge->GetCapacity(),pEdge->GetPER(),pEdge->GetMCS());
		pGraph->AddEdge(pEdge);
		pGraph->AddEdge(pReverseEdge);
	}
	// do the same for the interference graph
	nEdges = interferenceEdgesToAdd.size();
	for (int i=0;i<nEdges;i++)
	{
		RescueEdge * pEdge = interferenceEdgesToAdd[i];
		RescueEdge * pReverseEdge = 
			new RescueEdge(pEdge->GetEdgeNum()+nEdges, pEdge->GetTo(),pEdge->GetFrom(),
			pEdge->GetIsDirected(), pEdge->GetCapacity());
		pInterferenceGraph->AddEdge(pEdge);
		pInterferenceGraph->AddEdge(pReverseEdge);
	}
}

pair<CommunicationEdge *,RescueEdge *> GraphGenerator::CalcEdgeBetweenNodes(DynamicVertex * pFrom, DynamicVertex * pTo, int nEdgesAddedToCommunication, int nEdgesAddedToInterference)
{
	pair<CommunicationEdge *,RescueEdge *> edges;
	edges.first = NULL;
	edges.second = NULL;
	double generationTime = 0.;
	DblPoint PFrom = pFrom->GetLocationAtT(generationTime);
	DblPoint PTo = pTo->GetLocationAtT(generationTime);

	double dist = sqrt(PFrom.calcSqaureDist(PTo));
	PER_FOR_MCS communicationParams = mRateCalculator.GetCommunicationParameters(dist);
	
	if (mDebug)
	{
		mDebugFile << pFrom->GetVertexNum() << "," << pTo->GetVertexNum() << "," 
			<< PFrom.x << "," << PFrom.y << "," << PTo.x << "," << PTo.y << ","
			<< dist << "," << communicationParams.SNR << "," << communicationParams.PER 
			<< "," << communicationParams.MCS_INDEX << endl;
	}

	if (communicationParams.SNR > mParams.MIN_SNR_FOR_INTERFERENCE)
	{
		double dummyCapacity = -1.;
		bool dummyDirected = true;
		RescueEdge * pInterferenceEdge = 
			new RescueEdge((nEdgesAddedToInterference+1), pFrom->GetVertexNum(), pTo->GetVertexNum(),
							dummyDirected,dummyCapacity);
		edges.second = pInterferenceEdge;
	}

	if (communicationParams.SNR < mParams.MIN_SNR_FOR_COMMUNICATION)
		return edges;

	double capacity = communicationParams.mNetRate;
	double PER = communicationParams.PER;
	int MCS = communicationParams.MCS_INDEX;

	// edges are numbered from 1 to M
	CommunicationEdge * pEdge = new CommunicationEdge((nEdgesAddedToCommunication+1), pFrom->GetVertexNum(), pTo->GetVertexNum(), 
		capacity, PER, MCS);
	edges.first = pEdge;
	return edges;
}

void GraphGenerator::GenerateRandomVertices(CommunicationGraph * pGraph, InterferenceGraph * interferenceGraph)
{
	int nVertices = mParams.N_CLIENTS;

	srand ((unsigned int) time(NULL) );

	for (int i=0;i<nVertices;i++)
	{
		double x = (((double) rand()) / RAND_MAX)*mParams.SCALE_IN_METERS;  // uniform in [0,1]		
		double y = (((double) rand()) / RAND_MAX)*mParams.SCALE_IN_METERS;  // uniform in [0,1]
		DblPoint X0;
		X0.x = x;
		X0.y = y;
		double vX = (((double) rand()) / RAND_MAX - .5)*mParams.MAX_VELOCITY; // uniform in [-MAX_VELOCITY,MAX_VELOCITY]
		double vY = (((double) rand()) / RAND_MAX - .5)*mParams.MAX_VELOCITY;
		DblPoint V;
		V.x = vX;
		V.y = vY;
		// note that the vertex numbers are 1 to N and not 0 to N-1
		DynamicVertex * pVertex = new DynamicVertex (i+1, X0, V);
		pGraph->AddVertex(pVertex);

		DynamicVertex * pOtherVertex= new DynamicVertex(*pVertex);
		interferenceGraph->AddVertex(pOtherVertex);
	}
}

RequestsCollection & GraphGenerator::GenerateRequests(CommunicationGraph & graph)
{
	vector<CommunicationRequest> v;
	int nNearRequests = mParams.N_NEAR_REQUEST;
	int nFarRequests = mParams.N_FAR_REQUEST;
	double farThreshold = mParams.FAR_THRESHOLD*mParams.SCALE_IN_METERS;

	while (((int) v.size()) < (nNearRequests + nFarRequests) )
	{
		bool isBelow = ((int) v.size() <= nNearRequests); 
		CommunicationRequest req = GenerateRequest((int) v.size()+1, graph, farThreshold, isBelow);

		if (req.GetSource() == req.GetFirstDestination()) // bad request sorce = dest
			continue;

		// check if this request is the same as an old request (requests are randomly generated)
		bool alreadyExists = false;
		for (int i=0;i<(int) v.size();i++)
		{
			if ( (req.GetSource() == v[i].GetSource()) && 
				(req.GetFirstDestination() == v[i].GetFirstDestination()))
			{
				alreadyExists = true;
				break;
			}
			// this is not a must since the communication request is directed.
			// still this is unlikely, so I avoid it.
			if ( (req.GetFirstDestination() == v[i].GetSource()) && 
				(req.GetSource() == v[i].GetFirstDestination()))
			{
				alreadyExists = true;
				break;
			}
		}
		if (! alreadyExists)
		{
			v.push_back(req);
		}
	}

	RequestsCollection * pRequest = new RequestsCollection;
	for (int i=0;i<(int) v.size(); i++)
	{
		pRequest->AddRequest(new CommunicationRequest(v[i]));
	}

	return *pRequest;
}

CommunicationRequest GraphGenerator::GenerateRequest(int requestNum, CommunicationGraph & graph, double farThreshold, bool isBelow)
{
	double farThresholdSq = farThreshold * farThreshold;
	int nVertices = graph.GetNumVertices();
	bool goOn = true;
	int count = 0;
	while (goOn)
	{
		int randFirst = rand() % nVertices + 1;
		int randSecond = rand() % nVertices + 1;
		DynamicVertex *p1 = (DynamicVertex *) graph.GetVertex(randFirst);
		DynamicVertex *p2 = (DynamicVertex *) graph.GetVertex(randSecond);
		DblPoint X1 = p1->GetLocationAtT(0.);
		DblPoint X2 = p2->GetLocationAtT(0.);
		double sqDist = X1.calcSqaureDist(X2); 
		
		if ( (    isBelow  && (sqDist <= farThresholdSq) ) ||
			 ( (! isBelow) && (sqDist >= farThresholdSq) ) )
		{
			// TODO:: make the demand and rate more realistic and random.
			CommunicationRequest req(requestNum, randFirst, randSecond, 1., 1.);
			return req;
		}
		count++;
		if (count > 1000)
		{
			printf("Problem finding good pair for request isBelow = %d\n", isBelow);
			exit(1);
		}
	}
	// dummy request - will be filtered since src = dst
	CommunicationRequest req(requestNum, -1, -1, 1., 1.);
	return req;
}

void GraphGenerator::WriteRequestsFile(RequestsCollection & requests)
{
	requests.WriteToFile(mParams.REQUESTS_FILENAME);
}

void GraphGenerator::WriteGraphFiles(const CommunicationGraph & communicationGraph, const InterferenceGraph & interferenceGraph)
{
	// generate the graph and write it in time t = 0;
	double timeOfGeneration = 0.;
	communicationGraph.WriteToFiles(mParams.COMMUNICATION_GRAPH_FILENAME,mParams.COORDS_FILENAME, timeOfGeneration);
	interferenceGraph.WriteToFile(mParams.INTERFERENCE_GRAPH_FILENAME);
}

