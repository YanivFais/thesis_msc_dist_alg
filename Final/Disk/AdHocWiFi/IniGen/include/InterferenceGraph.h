#pragma once
#include "RescueGraph.h"
#include <stdlib.h>

class InterferenceGraph : public RescueGraph
{
public:
	InterferenceGraph(void);

	void SetInterferenceRadius (double radius);
	double GetInterferenceRadius() const;

	// serialize and de-serialize
	bool InitFromFiles(const char * graphFileName,const char * coordinatesFileName); 
	bool WriteToFile  (const char * graphFileName) const; 
private:
	bool ReadCoordinatesFile         (const char * coordinatesFileName);
	double mInterferenceRadius;
};
