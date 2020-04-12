#include "Rescuevertex.h"

using namespace std;

RescueVertex::RescueVertex () 
{
	mVertexNum = -1;
}

RescueVertex::RescueVertex (int i_vertexNum)
{
	mVertexNum = i_vertexNum;
}

RescueVertex::RescueVertex (const RescueVertex & other) :
	mInEdges(other.mInEdges), mOutEdges(other.mOutEdges)
{
	mVertexNum = other.mVertexNum;
}


std::vector<int>  & RescueVertex::GetInEdges () 
{
	return mInEdges;
}

std::vector<int>  & RescueVertex::GetOutEdges() 
{
	return mOutEdges;
}

int RescueVertex::GetVertexNum  () const
{
	return mVertexNum;
}

int RescueVertex::GetNumInEdges () const
{
	return mInEdges.size();
}
int RescueVertex::GetNumOutEdges() const
{
	return mOutEdges.size();
}

void RescueVertex::AddInEdge(int edgeNum)
{
	mInEdges.push_back(edgeNum);
}

void RescueVertex::AddOutEdge(int edgeNum)
{
	mOutEdges.push_back(edgeNum);
}

