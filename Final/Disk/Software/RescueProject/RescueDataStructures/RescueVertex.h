#pragma once
#include <vector>
#include "RescueEdge.h"

class RescueVertex
{
public:
	RescueVertex ();
	RescueVertex (int i_vertexNum);
	RescueVertex (const RescueVertex & other);

	std::vector<int>  & GetInEdges ();
	std::vector<int>  & GetOutEdges();

	int                  GetVertexNum  () const;
	int                  GetNumInEdges () const;
	int                  GetNumOutEdges() const;

	void                 AddInEdge     (int edgeNum);
	void                 AddOutEdge    (int edgeNum);

//private:
protected:
	
	std::vector<int> mInEdges;
	std::vector<int> mOutEdges;
	int              mVertexNum;
};

