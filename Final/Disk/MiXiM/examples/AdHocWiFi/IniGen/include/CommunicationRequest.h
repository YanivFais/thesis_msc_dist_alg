#pragma once

#include <vector>
#include <string>

class CommunicationRequest
{
public:
	CommunicationRequest(int requestNum, int source, int destination, double demand, double rawRate);
	CommunicationRequest(int requestNum, int source, std::vector<int> destinations, double demand, double rawRate);
	CommunicationRequest(const CommunicationRequest & otherRequest);
	~CommunicationRequest(void);

	std::string WriteRequestAsString() const;
	static CommunicationRequest * InitFromString(std::string requestStr);


	int    GetRequestNum () const;
	int    GetSource () const;
	double GetDemand () const; 
	double GetRawRate() const; 

	const std::vector<int> & GetDestinations() const;
	int    GetFirstDestination () const;

private:
	int mRequestNum;
	int mSource;
	std::vector<int> mDestinations;
	double mDemand;
	double mRawRate;
};
