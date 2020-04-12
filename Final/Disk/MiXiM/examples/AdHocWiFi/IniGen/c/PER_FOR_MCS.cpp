#include "PER_FOR_MCS.h"

PER_FOR_MCS::PER_FOR_MCS(void)
{
	mNetRate = -1.;  // NOT CALCULATED YET 
}

PER_FOR_MCS::PER_FOR_MCS(const PER_FOR_MCS & other) :
	MODULATION(other.MODULATION)
{
	MCS_INDEX = other.MCS_INDEX;
	CODE_RATE = other.CODE_RATE;
	DATA_RATE = other.DATA_RATE;
	SNR = other.SNR;
	PER = other.PER;
	mNetRate = other.mNetRate; 
}

PER_FOR_MCS PER_FOR_MCS::operator=(const PER_FOR_MCS &other)
{
	MODULATION.clear();
	MODULATION = other.MODULATION;
	MCS_INDEX = other.MCS_INDEX;
	CODE_RATE = other.CODE_RATE;
	DATA_RATE = other.DATA_RATE;
	SNR = other.SNR;
	PER = other.PER;
	mNetRate = other.mNetRate; 
	return (*this);
}

PER_FOR_MCS::~PER_FOR_MCS(void)
{}
