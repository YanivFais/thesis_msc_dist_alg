#include "RescueGraph.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include <tuple>
#include <math.h>

using namespace std;

RescueGraph::RescueGraph(bool i_isDirected) : mIsDirected(i_isDirected)
{}

RescueGraph::~RescueGraph(void)
{
	// delete all the pointers 
	map<int,RescueVertex *>::iterator verticesIter;
	for (verticesIter = mVertices.begin();verticesIter != mVertices.end(); verticesIter++)
	{
		delete verticesIter->second;  // delete the vertex
	}
	
	map<int,RescueEdge *>::iterator edgesIter;
	for (edgesIter = mEdges.begin();edgesIter != mEdges.end(); edgesIter++)
	{
		delete edgesIter->second;  // delete the edge
	}
}

int RescueGraph::GetNumVertices() const
{
	return mVertices.size();
}

int RescueGraph::GetNumEdges() const
{
	return mEdges.size(); 
}

std::map <int,RescueEdge*>   & RescueGraph::GetEdges      ()
{
	return mEdges;
}

std::map <int,RescueVertex*> & RescueGraph::GetVertices   ()
{
	return mVertices;
}
	
RescueVertex * RescueGraph::GetVertex (int vertexNum)
{
	map<int,RescueVertex *>::iterator found = mVertices.find(vertexNum);
	if (found != mVertices.end())
		return found->second;
	else
		return NULL; 
}

RescueEdge * RescueGraph::GetEdge (int edgeNum)
{
	map<int,RescueEdge *>::iterator found = mEdges.find(edgeNum);
	if (found != mEdges.end())
		return found->second;
	else
		return NULL; 
}

RescueEdge  * RescueGraph::GetReverseEdge (int edgeNum)
{
	map<int,int>::iterator found = mReverseEdges.find(edgeNum);
	if (found != mReverseEdges.end())
		return GetEdge (found->second);
	else
		return NULL; 
}

bool RescueGraph::AddVertex(RescueVertex * pVertex)
{
	int vertexNum = pVertex->GetVertexNum();
	map<int,RescueVertex *>::iterator found = mVertices.find(vertexNum);
	if (found != mVertices.end())
		return false;

	mVertices[vertexNum] = pVertex;
	return true;
}

bool RescueGraph::AddEdge  (RescueEdge * pEdge)
{
	int edgeNum = pEdge->GetEdgeNum();
	map<int,RescueEdge *>::iterator foundEdge = mEdges.find(edgeNum);
	if (foundEdge != mEdges.end())
		return false;

	// make sure both vertices exists.
	int from = pEdge->GetFrom();
	map<int,RescueVertex *>::iterator found = mVertices.find(from);
	if (found == mVertices.end())
		return false;
	
	int to = pEdge->GetTo();
	found = mVertices.find(to);
	if (found == mVertices.end())
		return false;

	mEdges[edgeNum] = pEdge;

	RescueVertex * pFrom = mVertices[from];
	pFrom->AddOutEdge(edgeNum);
	if (mIsDirected == false)
	{
		pFrom->AddInEdge(edgeNum);
	}
	RescueVertex * pTo = mVertices[to];
	pTo->AddInEdge(edgeNum);
	if (mIsDirected == false)
	{
		pTo->AddOutEdge(edgeNum);
	}

	// keep the reverse edge map
	if (mIsDirected == false)
	{
		mReverseEdges[edgeNum] = edgeNum;
	}
	else
	{
		mReverseEdges[edgeNum] = -1;
		RescueVertex * pReverseFrom = mVertices[to];
		const vector<int> & candidateEdges = pReverseFrom->GetOutEdges();
		for (int i=0;i<(int) candidateEdges.size();i++)
		{
			int candId = candidateEdges[i];
			RescueEdge * pCandEdge = GetEdge(candId);
			if (pCandEdge->GetTo() == from)
			{
				mReverseEdges[edgeNum] = candId;
				mReverseEdges[candId] = edgeNum;
				break;
			}
		}
	}

	return true;
}

void RescueGraph::RemoveEdge   (int edgeId)
{
	RescueEdge * pEdge = GetEdge (edgeId);
	if (pEdge == NULL)
		return;

	int from = pEdge->GetFrom();
	RescueVertex * pFrom = GetVertex(from); 
	int to = pEdge->GetTo();
	RescueVertex * pTo = GetVertex(to); 
	RemoveIdFromVec(edgeId, pFrom->GetOutEdges());
	RemoveIdFromVec(edgeId, pTo->GetInEdges());
	if (! mIsDirected)
	{
		RemoveIdFromVec(edgeId, pTo->GetOutEdges());
		RemoveIdFromVec(edgeId, pFrom->GetInEdges());
	}
	else
	{
		int reverseId = mReverseEdges[edgeId];
		if (reverseId != -1)  // there was a reverse edge but now we delete its pair.
			mReverseEdges[reverseId] = -1; 
	}
	mReverseEdges.erase(edgeId);  // in the non-directed case id is the reverse of itself

	mEdges.erase(edgeId);
	delete pEdge;
}

void RescueGraph::RemoveIdFromVec(int id, vector<int> & vec)
{
	vector<int> helper;
	for (int i=0;i<(int) vec.size();i++)
	{
		if (vec[i] != id)
			helper.push_back(vec[i]);
	}
	vec = helper;
}

void RescueGraph::FindBFSPath   (int source, int target, vector<int> & path)
{
	path.clear();
	if (source == target)
		return;

	int maxVertexId = 0;
	map<int,RescueVertex*>::iterator iter = mVertices.begin();
	for (;iter != mVertices.end(); iter++)
	{
		if (maxVertexId < iter->first)
			maxVertexId = iter->first; 
	}

	vector<bool> processed(maxVertexId, false);
	map<int,pair<int,int>> fathers;
	pair<int,int> dummyPair(-1,-1);
	fathers[source] = dummyPair;  // no father
	queue<int> reached;
	reached.push(source);
	processed[source-1] = true;
	while (! reached.empty())
	{
		int currVertex = reached.front();
		if (currVertex == target)
			break;

		reached.pop();
		vector<int> & outEdges = GetVertex(currVertex)->GetOutEdges();
		for (int i=0;i<(int) outEdges.size(); i++)
		{
			RescueEdge * pEdge = GetEdge(outEdges[i]);
			int to = pEdge->GetTo();

			if (!processed[to-1])
			{
				pair<int,int> fatherAndEdge(currVertex, outEdges[i]);
				fathers[to] = fatherAndEdge;
				reached.push(to);
				processed[to-1] = true;
			}
		}
	}

	// reconstruct the path backwards
	vector<int> reversePath;
	map<int,pair<int,int>>::iterator found = fathers.find(target);
	if (found == fathers.end())
		return; 
	pair<int,int> fathersData = found->second;
	while (fathersData.first != -1)
	{
		reversePath.push_back(fathersData.second);
		found = fathers.find(fathersData.first);
		if (found == fathers.end())
			return;   // error? 
		fathersData = found->second;
	}

	for (int i=(int) reversePath.size()-1; i>=0; i--)
		path.push_back(reversePath[i]);
}

int RescueGraph::GetEdgeNumByVertices(int from, int to)
{
	int edgeId = -1;
	RescueVertex * pFrom = GetVertex(from); 
	RescueVertex * pTo = GetVertex(to); 
	if ( (pFrom == NULL) || (pTo == NULL) )
		return edgeId;
	vector<int> & edges = pFrom->GetOutEdges();
	int nEdgeFrom = (int) edges.size();
	for (int i=0;i<nEdgeFrom;i++)
	{
		if (GetEdge(edges[i])->GetTo() == to)
		{
			edgeId = edges[i];
			break;
		}
	}
	return edgeId;
}

void RescueGraph::FindMaxBottleneckPath   (int source, int target, vector<int> & path)
{
	path.clear();
	if (source == target)
		return;

	int maxVertexId = 0;
	map<int,RescueVertex*>::iterator iter = mVertices.begin();
	for (;iter != mVertices.end(); iter++)
	{
		if (maxVertexId < iter->first)
			maxVertexId = iter->first; 
	}

	int MY_INFINITY = 100000000;
	const int BEFORE = 0;
	const int IN_PROCESS = 1;
	const int FINAL = 2;
	vector<int> processingState(maxVertexId, BEFORE);
	vector<int> bottleneckValue(maxVertexId, -MY_INFINITY);
	vector<int> distance(maxVertexId, MY_INFINITY);
	map<int,pair<int,int>> fathers;

	pair<int,int> dummyPair(-1,-1);
	fathers[source] = dummyPair;  // no father
	bottleneckValue[source-1] = MY_INFINITY;
	distance[source-1] = 0;
	vector<tr1::tuple<int,int,int>> active;
	active.push_back(tr1::make_tuple(bottleneckValue[source-1],distance[source-1],source));
	processingState[source-1] = IN_PROCESS;

	while (! active.empty()) 
	{
		sort(active.begin(), active.end());
		int nActives = active.size();
		int currVertex = tr1::get<2>(active[nActives-1]);
		int currDist = -tr1::get<1>(active[nActives-1]);
		processingState[currVertex-1] = FINAL;
		if (currVertex == target)
			break;

		active.pop_back();

		vector<int> & outEdges = GetVertex(currVertex)->GetOutEdges();
		for (int i=0;i<(int) outEdges.size(); i++)
		{
			RescueEdge * pEdge = GetEdge(outEdges[i]);
			int to = pEdge->GetTo();

			if (processingState[to-1] != FINAL)
			{
				int candidateWeight = min(bottleneckValue[currVertex-1], 
							(int) (floor(pEdge->GetCapacity()*1000.)));
				int candidateDist = currDist+1;
				if (processingState[to-1] == BEFORE)
				{
					active.push_back(tr1::make_tuple(candidateWeight,-candidateDist,to));
					processingState[to-1] = IN_PROCESS;
				}
				if ( (bottleneckValue[to-1] < candidateWeight) ||
					 ( (bottleneckValue[to-1] == candidateWeight) &&
					   (distance[to-1] > candidateDist) ) )
				{
					fathers[to] = make_pair(currVertex, outEdges[i]);;
					bottleneckValue[to-1] = candidateWeight;
					distance[to-1] = candidateDist;
					for (int iActives=0; iActives < (int) active.size();iActives++)
					{
						if (tr1::get<2>(active[iActives]) == to)
						{
							tr1::get<1>(active[iActives]) = -candidateDist;
							tr1::get<0>(active[iActives]) = candidateWeight;
							break;
						}
					}
				}
			}
		}
	}

	// reconstruct the path backwards
	vector<int> reversePath;
	map<int,pair<int,int>>::iterator found = fathers.find(target);
	if (found == fathers.end())
		return; 
	pair<int,int> fathersData = found->second;
	while (fathersData.first != -1)
	{
		reversePath.push_back(fathersData.second);
		found = fathers.find(fathersData.first);
		if (found == fathers.end())
			return;   // error? 
		fathersData = found->second;
	}

	for (int i=(int) reversePath.size()-1; i>=0; i--)
		path.push_back(reversePath[i]);
}
