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
	RescueEdge                   * GetReverseEdge      (int edgeNum);

	int                            GetEdgeNumByVertices(int from, int to);

	bool						   AddVertex(RescueVertex * pVertex);
	bool                           AddEdge  (RescueEdge * pEdge);

	void                           RemoveEdge(int edgeId);

	void						   FindBFSPath(int source, int target, std::vector<int> & path);

	void                           FindMaxBottleneckPath(int source, int target, std::vector<int> & path);

protected:

	std::map<int,RescueVertex*> mVertices;
	std::map<int,RescueEdge*> mEdges;
	std::map<int,int> mReverseEdges;

	bool     mIsDirected;


	void RemoveIdFromVec(int id, std::vector<int> & vec);

};
