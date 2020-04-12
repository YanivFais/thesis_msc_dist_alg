#include "GraphGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h> 
#include <tuple>

using namespace std; 

GraphGenerator::GraphGenerator(GenerationParams & params, const RateCalculator & rateCalculator, unsigned int seed) :
	mParams(params), mRateCalculator(rateCalculator), 
	mDebug(true), mDebugFileName("debug_comm_graph.csv")
{
	if (mDebug)
	{
		mDebugFile.open(mDebugFileName.c_str(),ios_base::trunc);
	}
	if (seed == 0)
		mSeed = (unsigned int) time(NULL);
	else
		mSeed = seed;

	cout << "Generation seed is: " << mSeed << endl;
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

	srand (mSeed);  // initiate the seed

	// check if meta-data file exists
	string metadataFilename = "points_metadata.txt";
	struct stat stFileInfo;
	int intStat;

	intStat = stat(metadataFilename.c_str(),&stFileInfo);
	if(intStat == 0) // exists
	{
		cout << "Using metadata file for vertex generation" << endl;
		GenerateVerticesFromMetadata(metadataFilename, communicationGraph, interferenceGraph);
	} 
	else  // not exists (or inaccessible)
	{
		GenerateRandomVertices(communicationGraph, interferenceGraph);
	}
	
	AddEdgesToGraphs(communicationGraph,interferenceGraph); 

	if (communicationGraph->GetNumEdges() == 0)
	{
		cout << "Error - no edges in the graph" << endl;
		exit(1);
	}

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
	pGraph->SetReverseEdgesOffset(offset);
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

		DynamicVertex * pOtherVertex = new DynamicVertex(*pVertex);
		interferenceGraph->AddVertex(pOtherVertex);

	}
}

void GraphGenerator::GenerateVerticesFromMetadata(string & metadataFilename, CommunicationGraph * pGraph,InterferenceGraph * pInterferenceGraph)
{
	vector<int> nVerticesInCluster;
	vector<vector<tr1::tuple<double, double, double>>> clustersTracks;
	string errorStr;

	if (ReadMetadataFile(metadataFilename, nVerticesInCluster,
				clustersTracks,	errorStr) == false)
	{
		cout << errorStr << endl;
		GenerateRandomVertices(pGraph, pInterferenceGraph);
		return;
	}

	int nClusters = clustersTracks.size();
	int vertexNumOffset = 1;
	for (int i=0;i<nClusters;i++)
	{
		vector<tr1::tuple<double, double, double>> & currCluster = clustersTracks[i];
		int currNVertices = nVerticesInCluster[i];
		GenerateCluster(currCluster, currNVertices, pGraph, pInterferenceGraph, vertexNumOffset);
		vertexNumOffset += currNVertices;
	}
}

void GraphGenerator::GenerateCluster(vector<tr1::tuple<double, double, double>> & cluster, 
									 int nVertices, CommunicationGraph * pGraph,
									 InterferenceGraph * pInterferenceGraph,
									 int vertexNumOffset)
{
// 1. For each vertex add noise to the leader's locations.
// 2. Find the velocities these noisy locations defines.

	for (int i=0;i<nVertices; i++)
	{
		vector<DblPoint> locationAtT;
		vector<double> startTofV;
		for (int j=0;j<(int) cluster.size();j++)
		{
			DblPoint X;
			X.x = tr1::get<1>(cluster[j]) + (((double) rand()) / RAND_MAX - .5)*mParams.CLUSTER_RADIUS;
			X.y = tr1::get<2>(cluster[j]) + (((double) rand()) / RAND_MAX - .5)*mParams.CLUSTER_RADIUS;
			if (X.x > 1.) 
				X.x = 1.;
			if (X.x < 0.)
				X.x = 0.;
			if (X.y > 1.) 
				X.y = 1.;
			if (X.y < 0.)
				X.y = 0.;
			X.x *= mParams.SCALE_IN_METERS;
			X.y *= mParams.SCALE_IN_METERS;
			locationAtT.push_back(X);
			startTofV.push_back(tr1::get<0>(cluster[j]));
		}
		
		DblPoint X0 = locationAtT[0];
		vector<DblPoint> V;
		
		for (int j=1;j<(int)locationAtT.size();j++)
		{
			DblPoint currV; 
			double dT = (startTofV[j] - startTofV[j-1]);
			currV.x = (locationAtT[j].x - locationAtT[j-1].x) / dT;
			currV.y = (locationAtT[j].y - locationAtT[j-1].y) / dT;

			V.push_back(currV);
		}
		startTofV.pop_back();  // delete the last time (the vertex go on moving in the last direction)

		DynamicVertex * pVertex = new DynamicVertex (i + vertexNumOffset, X0, V, startTofV);
		pGraph->AddVertex(pVertex);

		DynamicVertex * pOtherVertex = new DynamicVertex(*pVertex);
		pInterferenceGraph->AddVertex(pOtherVertex);
	}
}

bool GraphGenerator::ReadMetadataFile(string & metadataFilename, 
									  vector<int> & nVerticesInCluster,
									  vector<vector<tr1::tuple<double, double, double>>> & clustersTracks, 
									  string & errorStr)
{
	ifstream metadataFile;
	metadataFile.open(metadataFilename.c_str(),ios::in);

	if (! metadataFile.is_open()) 
	{
		errorStr = "Error: problem opening metadata file - using simple model";
		return false;
	}

	char lineBuf[512];
	lineBuf[511] = 0x00;

	vector<tr1::tuple<double, double, double>> currTrack;

	enum States {BEFORE, IN};
	States STATE               = BEFORE;
	int nVertices              = -1;
	int nTimePoints            = -1;
	while (! metadataFile.eof()) 
	{
		metadataFile.getline(lineBuf,511);
		string line = lineBuf;
		TrimSpaces(line);
		if (line.size() == 0)
			continue;

		if (*line.c_str() == '/') // remark
			continue; 

		stringstream ss (line);
		string token;
		if (currTrack.size() == nTimePoints)
		{
			STATE = BEFORE;
		}

		if (STATE == BEFORE)   // read the track "header" line
		{
			// finish the previous track (if any)
			if (currTrack.size() > 0)
			{
				clustersTracks.push_back(currTrack);
				nVerticesInCluster.push_back(nVertices);
				currTrack.clear(); 
			}

			getline (ss,token,',');
			int trackNum = atoi(token.c_str());
			if (trackNum != clustersTracks.size())
			{
				errorStr = "Error: inconsistent number of tracks - using simple model";
				return false;
			}
			getline (ss,token,',');
			nVertices = atoi(token.c_str());
			if (nVertices <= 0) 
			{
				errorStr = "Error: zero vertices in track - using simple model";
				return false;
			}

			getline (ss,token,',');
			nTimePoints = atoi(token.c_str());
			if (nTimePoints <= 0)
			{
				errorStr = "Error: zero time points in track - using simple model";
				return false;
			}
			STATE = IN;
			continue;
		}

		getline (ss,token,',');
		int pointNum = atoi(token.c_str());
		if (pointNum != currTrack.size())
		{
			errorStr = "Error: inconsistent number of point in track - using simple model";
			return false;
		}
		getline (ss,token,',');
		int currTime = atoi(token.c_str());
		if (currTime < 0) 
		{
			errorStr = "Error: negative time point - using simple model";
			return false;
		}
		int nPointsRead = currTrack.size();
		if (nPointsRead > 0)
		{
			tr1::tuple<double,double,double> & prev = currTrack[nPointsRead-1];
			if (tr1::get<0>(prev) >= currTime)
			{
				errorStr = "Error: time points are not in ascending order - using simple model";
				return false;
			}
		}


		getline (ss,token,',');
		double X = atof(token.c_str());
		if ( (X < 0) || (X > 1) )
		{
			errorStr = "Error: X-coordinate out of range - using simple model";
			return false;
		}

		getline (ss,token,',');
		double Y = atof(token.c_str());
		if ( (Y < 0) || (Y > 1) )
		{
			errorStr = "Error: Y-coordinate out of range - using simple model";
			return false;
		}
		tr1::tuple<double, double, double> timePoint(currTime, X, Y); 
		currTrack.push_back(timePoint); 

	}
	// finish the last track (if wasn't finished before)
	if (currTrack.size() > 0) 
	{
		if (currTrack.size() == nTimePoints) 
		{
			clustersTracks.push_back(currTrack);
			nVerticesInCluster.push_back(nVertices);
			currTrack.clear(); 
		}
		else
		{
			errorStr = "Error: wrong number of time points in track - using simple model";
			return false;
		}
	}

	metadataFile.close();
	return true;
}

void GraphGenerator::TrimSpaces( string& str)  
{  
// Trim Both leading and trailing spaces  
	size_t startpos = str.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces  
	size_t endpos = str.find_last_not_of(" \t"); // Find the first character position from reverse af  
	// if all spaces or empty return an empty string  
	if(( string::npos == startpos ) || ( string::npos == endpos))  
	{  
		str.clear();  
	}  
	else
	{
		str = str.substr( startpos, endpos-startpos+1 );  
	}
}  

RequestsCollection & GraphGenerator::GenerateRequests(CommunicationGraph & graph)
{
	vector<CommunicationRequest> v;
	int nNearRequests = mParams.N_NEAR_REQUEST;
	int nFarRequests = mParams.N_FAR_REQUEST;
	double farThreshold = mParams.FAR_THRESHOLD*mParams.SCALE_IN_METERS;

	double demand = mParams.DEMAND_SIZE;
	while (((int) v.size()) < (nNearRequests + nFarRequests) )
	{
		bool isBelow = ((int) v.size() < nNearRequests); 
		CommunicationRequest req = GenerateRequest((int) v.size()+1, graph, farThreshold, isBelow, demand);

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

CommunicationRequest GraphGenerator::GenerateRequest(int requestNum, CommunicationGraph & graph, double farThreshold, bool isBelow, double demand)
{
	double farThresholdSq = farThreshold * farThreshold;
	int nVertices = graph.GetNumVertices();
	bool goOn = true;
	int count = 0;
	while (goOn)
	{
		int randFirst = rand() % nVertices + 1;
		int randSecond = rand() % nVertices + 1;
		if (randFirst != randSecond)
		{
			DynamicVertex *p1 = (DynamicVertex *) graph.GetVertex(randFirst);
			DynamicVertex *p2 = (DynamicVertex *) graph.GetVertex(randSecond);
			DblPoint X1 = p1->GetLocationAtT(0.);
			DblPoint X2 = p2->GetLocationAtT(0.);
			double sqDist = X1.calcSqaureDist(X2); 
			
			if ( (    isBelow  && (sqDist <= farThresholdSq) ) ||
				 ( (! isBelow) && (sqDist >= farThresholdSq) ) )
			{
				// TODO:: make the demand and rate more realistic and random.
				CommunicationRequest req(requestNum, randFirst, randSecond,demand, 1.);
				return req;
			}
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
	interferenceGraph.WriteToFile(mParams.INTERFERENCE_GRAPH_FILENAME);
	communicationGraph.WriteToFiles(mParams.COMMUNICATION_GRAPH_FILENAME,mParams.COORDS_FILENAME, timeOfGeneration);

	// write coordinates file for different T's (used by the simulator)
	if (mParams.TIME_PERIOD > 0)
	{
		for (int i=mParams.TIME_RESOLUTION;i<=mParams.TIME_PERIOD;i++)
		{
			communicationGraph.WriteCoordinatesFile(mParams.COORDS_FILENAME, (double)i);
		}
	}

}

