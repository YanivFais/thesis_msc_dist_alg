#pragma once
#include "rescuegraph.h"

class InterferenceGraph : public RescueGraph
{
public:
	InterferenceGraph(void);

	void SetInterferenceRadius (double radius);
	double GetInterferenceRadius() const;

	// serialize and de-serialize
	bool InitFromFiles(const char * graphFileName,const char * coordinatesFileName, char * addedExtension, double T = 0.); 
	bool WriteToFile  (const char * graphFileName) const; 
private:
	bool ReadCoordinatesFile         (const char * coordinatesFileName, double T, char * addedExtension);
	void calcNameTimeExtension       (double T, std::string & fnameAtT, char * addedExtension) const;
	double mInterferenceRadius;
};
