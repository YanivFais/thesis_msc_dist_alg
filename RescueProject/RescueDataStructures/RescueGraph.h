#pragma once

#include <map>
#include <vector>
#include "RescueEdge.h"
#include "RescueVertex.h"


class RescueGraph
{
public:
	RescueGraph  (bool i_isDirected);
	~RescueGraph (void);

	int                            GetNumVertices() const;
	int                            GetNumEdges   () const;
	std::map <int,RescueEdge*>   & GetEdges      ();
	std::map <int,RescueVertex*> & GetVertices   (); 
	
	RescueVertex                 * GetVertex           (int vertexNum);
	RescueEdge                   * GetEdge             (int edgeNum);

	bool						   AddVertex(RescueVertex * pVertex);
	bool                           AddEdge  (RescueEdge * pEdge);

protected:

	std::map<int,RescueVertex*> mVertices;
	std::map<int,RescueEdge*> mEdges;

	bool     mIsDirected;
};
