#include "CommunicationRequest.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

CommunicationRequest::CommunicationRequest(int requestNum, int source, int destination, double demand, double rawRate) 
{
	mRequestNum = requestNum;
	mSource = source;
	mDestinations.push_back(destination);
	mDemand = demand;
	mRawRate = rawRate;
}

CommunicationRequest::CommunicationRequest(int requestNum, int source, std::vector<int> destinations, double demand, double rawRate)
{
	mRequestNum = requestNum;
	mSource = source;
	mDestinations = destinations;
	mDemand = demand;
	mRawRate = rawRate;
}

CommunicationRequest::CommunicationRequest(const CommunicationRequest & otherRequest)
{
	mRequestNum = otherRequest.mRequestNum;
	mSource = otherRequest.mSource;
	mDestinations = otherRequest.mDestinations;
	mDemand = otherRequest.mDemand;
	mRawRate = otherRequest.mRawRate;
}

CommunicationRequest::~CommunicationRequest(void)
{}

int    CommunicationRequest::GetRequestNum () const
{
	return mRequestNum;
}

int    CommunicationRequest::GetSource () const
{
	return mSource;
}

double CommunicationRequest::GetDemand () const
{
	return mDemand;
}

double CommunicationRequest::GetRawRate() const
{
	return mRawRate;
}

const std::vector<int> & CommunicationRequest::GetDestinations() const
{
	return mDestinations;
}

int    CommunicationRequest::GetFirstDestination () const
{
	return mDestinations[0];
}


string CommunicationRequest::WriteRequestAsString() const
{
	char Line[256];
	int nDest = mDestinations.size();
	sprintf(Line,"%d,%d,%8.3f,%d", mRequestNum, mSource, mDemand, nDest);

	for (int i=0;i<nDest;i++)
	{
		sprintf(Line,"%s,%d",Line, mDestinations[i]);
	}
	string req(Line);

	return req;
}

CommunicationRequest * CommunicationRequest::InitFromString(string requestStr) 
{
	stringstream ss (requestStr);
	string token;
    getline(ss, token, ',');
    int requestNum = atoi(token.c_str());
    getline(ss, token, ',');
    int source = atoi(token.c_str());
    getline(ss, token, ',');
	double demand = atof(token.c_str());	
    getline(ss, token, ',');
    int nDest = atoi(token.c_str());
    getline(ss, token, ',');
	int dest = atoi(token.c_str());	
	CommunicationRequest * pRequest = new CommunicationRequest(requestNum, source,dest,demand,1.);
	return pRequest;
}

