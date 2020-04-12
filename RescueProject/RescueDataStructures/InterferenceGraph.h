#pragma once
#include "rescuegraph.h"

class InterferenceGraph : public RescueGraph
{
public:
	InterferenceGraph(void);
	~InterferenceGraph(void);

	void SetInterferenceRadius (double radius);
	double GetInterferenceRadius() const;

	// serialize and de-serialize
	bool InitFromFile (const char * graphFileName); 
	bool WriteToFile  (const char * graphFileName) const; 
private:
	double mInterferenceRadius;
};
