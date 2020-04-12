#pragma once

#include "RescueGraph.h"

class CommunicationGraph : public RescueGraph
{
public:
	CommunicationGraph  (void);
	~CommunicationGraph (void);

	// serialize and de-serialize
	bool InitFromFiles (const char * commGraphFileName,const char * coordinatesFileName); 
	bool WriteToFiles  (const char * commGraphFileName,const char * coordinatesFileName, double T) const; 

protected:

	bool ReadCommunicationGraphFile  (const char * commGraphFileName);
	bool ReadCoordinatesFile         (const char * coordinatesFileName);
	bool WriteCommunicationGraphFile (const char * commGraphFileName, double T) const;
	bool WriteCoordinatesFile        (const char * coordinatesFileName, double T) const;

};
