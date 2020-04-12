#pragma once

#include "RescueGraph.h"
#include <string>

class CommunicationGraph : public RescueGraph
{
public:
	CommunicationGraph  (void);

	void SetReverseEdgesOffset(int reverseEdgeOffset);
	// serialize and de-serialize
	bool InitFromFiles (const char * commGraphFileName,const char * coordinatesFileName, double T = 0.); 
	bool WriteToFiles  (const char * commGraphFileName,const char * coordinatesFileName, double T) const; 
	bool WriteCoordinatesFile (const char * coordinatesFileName, double T) const;

protected:

	bool ReadCommunicationGraphFile  (const char * commGraphFileName);
	bool ReadCoordinatesFile         (const char * coordinatesFileName, double T);
	bool WriteCommunicationGraphFile (const char * commGraphFileName, double T) const;
	void calcNameTimeExtension       (double T, std::string & fnameAtT) const;
	int  mReverseEdgeOffset;
};
