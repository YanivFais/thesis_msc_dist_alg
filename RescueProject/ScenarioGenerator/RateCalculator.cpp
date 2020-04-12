#include <math.h>

#include "RateCalculator.h"

using namespace std;

RateCalculator::RateCalculator(const GenerationParams & params, const PER2SNRTable * PER2SNRTable) :
	mParams(params), mPER2SNRTable(PER2SNRTable)
{}

RateCalculator::~RateCalculator(void)
{}


// distance are meters.
double RateCalculator::GetPathlossFromDist(double dist) const
{
	double L; 
	if (dist <= mParams.D_BREAKPOINT)
	{
		double SF = mParams.NUM_STD_FOR_DECAY*mParams.SF_BEFORE_BREAKPOINT; // in db
		// the constants are given assuming the distance is in meters and the 
		// frequency in Hz (in the config file the frequency is in GHz). 
		double L_FS = 20*log10(dist) + 20*log10(mParams.FREQUENCY*1e9) - 147.5;
		L = L_FS + SF;

	}
	else
	{
		double SF = mParams.NUM_STD_FOR_DECAY*mParams.SF_AFTER_BREAKPOINT;
		// the constants are given assuming the distance is in meters and the 
		// frequency in Hz (in the config file the frequency is in GHz). 
		double L_FS = 20*log10(mParams.D_BREAKPOINT) + 20*log10(mParams.FREQUENCY*1e9) - 147.5;
		L = L_FS + 35*log10(dist/mParams.D_BREAKPOINT) + SF;
	}

	return L;
}

double RateCalculator::GetSNR(double pathLoss) const
{
	double receptionPowerInMilliWatts = 
		10*log10(mParams.TRANSMISSION_POWER_IN_MILI_WATTS) - pathLoss;  
	return (receptionPowerInMilliWatts - mParams.NOISE_IN_DBM); 
}

// PERS_FOR_MCS (Modulation, Code, PER, DataRate)  // should multiply in code rate from table)
vector<PER_FOR_MCS *> * RateCalculator::GetPERFromSNR(double SNR) const
{
	return mPER2SNRTable->GetMCSAndPER(SNR); 
}

PER_FOR_MCS RateCalculator::GetCommunicationParameters(double dist) const
{

	double pathLoss = GetPathlossFromDist(dist);
	double SNR = GetSNR(pathLoss);
	 // note this vector is allocated in the class, and should be freed here
	vector<PER_FOR_MCS *> * persData = GetPERFromSNR(SNR);
	int nPers = (int) persData->size(); 

	PER_FOR_MCS bestCommunicationParameters(*((*persData)[0]));
	// find the best net rate (among all possible MCS)
	for (int i=0;i<nPers; i++)
	{
		PER_FOR_MCS * currMCS = (*persData)[i];
		double netRate = CalcNetRateForMCS(*currMCS);

		if (netRate > bestCommunicationParameters.mNetRate)
		{
			bestCommunicationParameters = *currMCS;
			bestCommunicationParameters.mNetRate = netRate;
		}

		// no longer needed - allocaed in the caller
		delete currMCS;
	}
	delete persData;

	return bestCommunicationParameters;
}

double RateCalculator::CalcNetRateForMCS(PER_FOR_MCS & perForMCS) const
{
	// the report notations
	double macO = mParams.MAC_HEADER_LEN + mParams.MAC_FCS_LEN;

	// 8000 is kbyte to bits factor
	double macPsduSize = mParams.PAYLOAD*8000 + macO;
	double Tphy = mParams.PREAMBLE_TIME + mParams.HEADER_TIME + 
		mParams.MAC_SERVICE_TRAILER_TIME + (macPsduSize / perForMCS.DATA_RATE);

	double ackTime = mParams.PREAMBLE_TIME + mParams.HEADER_TIME +
		mParams.MAC_SERVICE_TRAILER_TIME + (mParams.MAC_ACK_LEN / perForMCS.DATA_RATE); 
	double rtsTime = mParams.PREAMBLE_TIME + mParams.HEADER_TIME +
		mParams.MAC_SERVICE_TRAILER_TIME + (mParams.MAC_RTS_LEN / perForMCS.DATA_RATE); 
	double ctsTime = mParams.PREAMBLE_TIME + mParams.HEADER_TIME +
		mParams.MAC_SERVICE_TRAILER_TIME + (mParams.MAC_CTS_LEN / perForMCS.DATA_RATE); 

	double overallTime = (mParams.DIFS_TIME + mParams.CW_TIME + 3*mParams.SIFS_TIME +
			Tphy + rtsTime + ctsTime + ackTime); 

	// 8000 is kbyte to bits factor
	double netDataSizeAfterPer = mParams.PAYLOAD * 8000 * (1. - perForMCS.PER);

	return netDataSizeAfterPer / overallTime;
}




