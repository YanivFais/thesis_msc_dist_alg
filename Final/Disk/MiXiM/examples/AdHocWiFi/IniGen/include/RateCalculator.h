/*TODO YF
#pragma once

#include "PER2SNRTable.h"
#include "GenerationParams.h"


class RateCalculator
{
public:
	RateCalculator(const GenerationParams & params, const PER2SNRTable * PER2SNRTable);
	~RateCalculator(void);

	PER_FOR_MCS GetCommunicationParameters(double dist) const;

protected:
	const GenerationParams & mParams;
	const PER2SNRTable * mPER2SNRTable;

	double GetPathlossFromDist(double dist) const;
	double GetSNR(double pathLoss) const;
	std::vector<PER_FOR_MCS *> * GetPERFromSNR(double SNR) const;
	double CalcNetRateForMCS(PER_FOR_MCS & perForMCS) const;

};
*/
