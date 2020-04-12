/*
#pragma once

#include <map>
#include <vector>
#include "PER_FOR_MCS.h"

class PER2SNRTable
{
public:
	PER2SNRTable(const char * per2SNRTableFileName);
	~PER2SNRTable(void);
	std::vector<PER_FOR_MCS *> * GetMCSAndPER(double SNR) const;

	bool mIsValid;
private:
	std::map<int, std::vector<PER_FOR_MCS *>> mcsMapOfSNR2PER;

	double CalcPERFromSNRTable(const std::vector<PER_FOR_MCS *> & vec, double SNR) const;
};
*/
