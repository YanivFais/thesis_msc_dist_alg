#pragma once
#include "CommunicationRequest.h"
#include <vector>

class RequestsCollection
{
public:
	RequestsCollection(void);
	~RequestsCollection(void);
	void AddRequest (CommunicationRequest * pRequest);
	std::vector<CommunicationRequest *> GetRequests();

	bool InitFromFile (const char * requestsFileName); 
	bool WriteToFile  (const char * requestsFileName) const; 

private:
	std::vector<CommunicationRequest *> mRequests;
};
