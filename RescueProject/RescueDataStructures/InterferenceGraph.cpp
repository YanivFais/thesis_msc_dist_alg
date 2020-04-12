#include "InterferenceGraph.h"
#include "DynamicVertex.h"
#include "RescueEdge.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

InterferenceGraph::InterferenceGraph(void) : 
	RescueGraph(true)
{
	mInterferenceRadius = -1.;
}

void InterferenceGraph::SetInterferenceRadius (double radius)
{
	mInterferenceRadius = radius;
}

double InterferenceGraph::GetInterferenceRadius() const
{
	return mInterferenceRadius;
}

// serialize and de-serialize
bool InterferenceGraph::InitFromFile (const char * graphFileName)
{
	ifstream interferenceFile;
    interferenceFile.open(graphFileName);
    if (!interferenceFile) 
	{
		cout << "Unable to open file" << graphFileName << endl;
        return false;
    }
 
    int lineNum = 1; 
    string  line; 
	int nEdges;
    while (getline(interferenceFile,line)) 
    {
		if ( (line.size() == 0) || 
             ((line.size() >= 1) && (line[0] == '#') ))
        {
			continue;  // comment
        } 
        else 
        {
           // parse lines
			stringstream ss (line);
			string token;
            if (lineNum == 1) // header line - number of edges
            {
				getline (ss,token,',');
				nEdges = atoi(token.c_str());
                lineNum++;
                continue;
            }
 
            getline(ss, token, ',');
            int edgeNum = atoi(token.c_str());
            getline(ss, token, ',');
            int from = atoi(token.c_str());
            getline(ss, token, ',');
			int to = atoi(token.c_str());
            getline(ss, token, ',');
			double dummyCapacity = -1.;
			bool isDirected = true;
			RescueEdge * pEdgeOneWay = 
				new RescueEdge(edgeNum,from, to, isDirected, dummyCapacity);
            AddEdge  (pEdgeOneWay);
			RescueEdge * pEdgeReverse = 
				new RescueEdge(nEdges + edgeNum,to, from, isDirected, dummyCapacity);
			AddEdge  (pEdgeReverse);
		}
	}
 
    interferenceFile.close();
    return true;
}

bool InterferenceGraph::WriteToFile  (const char * graphFileName) const
{
	if (graphFileName == NULL)
		return true;

	ofstream graphFile;
	graphFile.open(graphFileName,ios_base::trunc);
    if (!graphFile) 
	{
		cout << "Unable to open file" << graphFileName << endl;
        return false;
    }
	// first line - the edges number
	graphFile << GetNumEdges() << endl;

	map<int,RescueEdge *>::const_iterator edgesIter;
	for (edgesIter = mEdges.begin();edgesIter != mEdges.end(); edgesIter++)
	{
		RescueEdge * pEdge = edgesIter->second;  

		graphFile << pEdge->GetEdgeNum() << "," << pEdge->GetFrom() << "," 
			<< pEdge->GetTo() << endl;
	}

	graphFile.close();
	return true;
}

