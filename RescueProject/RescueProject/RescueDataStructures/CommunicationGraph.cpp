#include "CommunicationGraph.h"

#include "CommunicationEdge.h"
#include "DynamicVertex.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

// false means undirected graph 
CommunicationGraph::CommunicationGraph() :RescueGraph(true)
{
	mReverseEdgeOffset = -1;
}

void CommunicationGraph::SetReverseEdgesOffset(int reverseEdgeOffset)
{
	mReverseEdgeOffset = reverseEdgeOffset;
}

bool CommunicationGraph::InitFromFiles(const char * commGraphFileName,const char * coordinatesFileName)
{
	if (ReadCoordinatesFile(coordinatesFileName) == false)
		return false;

	return ReadCommunicationGraphFile(commGraphFileName);
}

bool CommunicationGraph::WriteToFiles(const char * commGraphFileName,const char * coordinatesFileName, double T) const
{
	if (WriteCoordinatesFile(coordinatesFileName,T) == false)
		return false;

	return WriteCommunicationGraphFile(commGraphFileName,T);
}

bool CommunicationGraph::WriteCommunicationGraphFile (const char * commGraphFileName, double T) const
{
	if (commGraphFileName == NULL)
		return true;

	// this condition must be after checking for NULL name
	double MY_EPS = 1e-6;
	if ( (T > MY_EPS) || (T < -MY_EPS) )
	{
		cout << "Supporting only T = 0"  << endl;
        return false;
    }

	ofstream commGraphFile;
	commGraphFile.open(commGraphFileName,ios_base::trunc);
    if (!commGraphFile) 
	{
		cout << "Unable to open file" << commGraphFileName << endl;
        return false;
    }
	// first line - the edges number
//	commGraphFile << GetNumEdges() << endl;
	commGraphFile << mReverseEdgeOffset << endl;

	map<int,RescueEdge *>::const_iterator edgesIter;
	for (edgesIter = mEdges.begin();edgesIter != mEdges.end(); edgesIter++)
	{
		CommunicationEdge * pEdge = (CommunicationEdge *) edgesIter->second;  

		commGraphFile << pEdge->GetEdgeNum() << "," << pEdge->GetFrom() << "," 
			<< pEdge->GetTo()  << "," << pEdge->GetCapacity() << "," 
			<< pEdge->GetPER() << "," << pEdge->GetMCS() << endl;
	}

	commGraphFile.close();
	return true;
}

bool CommunicationGraph::WriteCoordinatesFile (const char * coordinatesFileName, double T) const
{
	if (coordinatesFileName == NULL)
		return true;

	ofstream coordinatesFile;
	coordinatesFile.open(coordinatesFileName,ios_base::trunc);
    if (!coordinatesFile) 
	{
		cout << "Unable to open file" << coordinatesFileName << endl;
        return false;
    }
 
	coordinatesFile << mVertices.size() << endl;
	map<int,RescueVertex *>::const_iterator verticesIter;
	for (verticesIter = mVertices.begin();verticesIter != mVertices.end(); verticesIter++)
	{
		DynamicVertex * pVertex = (DynamicVertex *) verticesIter->second;  
		DblPoint locationAtT = pVertex->GetLocationAtT(T);
		DblPoint velocity = pVertex->GetVelocity();

		coordinatesFile << pVertex->GetVertexNum() << "," << locationAtT.x << "," << locationAtT.y
			<< "," << velocity.x << "," << velocity.y << endl;
	}

	coordinatesFile.close();
	return true;
}


bool CommunicationGraph::ReadCoordinatesFile(const char *coordinatesFileName)
{
	ifstream coordinatesFile;
    coordinatesFile.open(coordinatesFileName);
    if (!coordinatesFile) 
	{
		cout << "Unable to open file" << coordinatesFileName << endl;
        return false;
    }
 
	bool firstLine = true;
	int nVertices;
	string line;
    while (getline(coordinatesFile,line)) 
    {
		if ( (line.size() == 0) || 
             ((line.size() >= 1) && (line[0] == '#') ))
        {
			continue;  // comment
        } 
        else 
        {
			stringstream ss (line);
			string token;
            if (firstLine) // header line - number of edges
            {
				getline (ss,token,',');
				nVertices = atoi(token.c_str());
                firstLine = false;
                continue;
            }
 
            getline(ss, token, ',');
            int vertexNum = atoi(token.c_str());
            getline(ss, token, ',');
            double x = atof(token.c_str());
            getline(ss, token, ',');
			double y = atof(token.c_str());	
            getline(ss, token, ',');
            double Vx = atof(token.c_str());
            getline(ss, token, ',');
			double Vy = atof(token.c_str());	
			DblPoint X0; 
			X0.x = x;
			X0.y = y;
			DblPoint V;
			V.x = Vx;
			V.y = Vy;
			DynamicVertex * pVertex = new DynamicVertex(vertexNum, X0, V);			
			AddVertex(pVertex);
		}
	}

	coordinatesFile.close();
	return true;
}

bool CommunicationGraph::ReadCommunicationGraphFile(const char * commGraphFileName)
{
	ifstream communicationFile;
    communicationFile.open(commGraphFileName);
    if (!communicationFile) 
	{
		cout << "Unable to open file" << commGraphFileName << endl;
        return false;
    }
 
    bool firstLine = true; 
    string  line; 
	while (getline(communicationFile,line)) 
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
            if (firstLine) // header line - number of edges
            {
				getline (ss,token,',');
				mReverseEdgeOffset = atoi(token.c_str());
                firstLine = false;
                continue;
            }
 
            getline(ss, token, ',');
            int edgeNum = atoi(token.c_str());
            getline(ss, token, ',');
            int from = atoi(token.c_str());
            getline(ss, token, ',');
			int to = atoi(token.c_str());
            getline(ss, token, ',');
			double capacity = atof(token.c_str());
            getline(ss, token, ',');
            double PER = atof(token.c_str());
            getline(ss, token, ',');
			int  MCS = atoi(token.c_str());
			CommunicationEdge * pEdgeOneWay = 
				new CommunicationEdge(edgeNum,from, to, capacity, PER, MCS);
            AddEdge  (pEdgeOneWay);
/*			The reverse edge is written to the file, so no need to add it now
			CommunicationEdge * pEdgeReverse = 
				new CommunicationEdge(mReverseEdgeOffset + edgeNum,to, from, capacity, PER, MCS);
			AddEdge  (pEdgeReverse); */
		}
	}
 
    communicationFile.close();
    return true;
}



