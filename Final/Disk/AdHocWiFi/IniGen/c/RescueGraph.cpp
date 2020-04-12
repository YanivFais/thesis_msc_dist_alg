#include "RescueGraph.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

