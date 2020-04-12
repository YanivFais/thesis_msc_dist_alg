#pragma once
#include <string>

class PER_FOR_MCS
{
public:
	PER_FOR_MCS(void);
	PER_FOR_MCS(const PER_FOR_MCS & other);
	PER_FOR_MCS operator=(const PER_FOR_MCS &other);
	~PER_FOR_MCS(void);

	// key
	int MCS_INDEX;
	// informative fields
	std::string MODULATION;
	double CODE_RATE;
	double DATA_RATE;
	// input - in db
	double SNR;

	// result
	double PER;

	double mNetRate;  // -1 if not calculated.
};
