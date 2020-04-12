#include "PER2SNRTable.h"

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

PER2SNRTable::PER2SNRTable(const char * per2SNRTableFileName)
{
	mIsValid = false;
	ifstream myFile;
	myFile.open(per2SNRTableFileName);
	if (! myFile.is_open()) 
	{
		string tmp(per2SNRTableFileName);
		cout << "Error opening file: " << tmp.c_str() << endl;
		return;
	}
	int prevMCS = -1;
	vector<PER_FOR_MCS *> currMCSVector;
	int currPerForMCS;
	string line;
	while (! myFile.eof() )
	{
		getline(myFile,line);
		if (line.empty())
			continue;
		if(line[0]=='/')
			continue;
		char *dup = _strdup(line.c_str());
		char *tok = strtok(dup,",");

		string mcs(tok);
		PER_FOR_MCS * perForMCS = new PER_FOR_MCS;
		currPerForMCS = atoi(mcs.c_str());
		if (currPerForMCS != prevMCS)
		{
			if (prevMCS != -1)
			{
				mcsMapOfSNR2PER[prevMCS] = currMCSVector;
				currMCSVector.clear();
			}
			prevMCS = currPerForMCS;
		}
		perForMCS->MCS_INDEX = currPerForMCS;
		

		tok = strtok(NULL,",");
		string modulation(tok);
		perForMCS->MODULATION = modulation;

		tok = strtok(NULL,",");
		string codeRate(tok);
		perForMCS->CODE_RATE = atof(codeRate.c_str());

		tok = strtok(NULL,",");
		string dataRate(tok);
		perForMCS->DATA_RATE = atof(dataRate.c_str());

		tok = strtok(NULL,",");
		string snr(tok);
		perForMCS->SNR = atof(snr.c_str());

		tok = strtok(NULL,",");
		string per(tok);
		perForMCS->PER = atof(per.c_str());

		free(dup);
		currMCSVector.push_back(perForMCS);

	}
	// last one
	mcsMapOfSNR2PER[currPerForMCS] = currMCSVector;
	currMCSVector.clear();

	myFile.close();
	mIsValid = true;
}

PER2SNRTable::~PER2SNRTable(void)
{
	// delete the allocated pointers
	map<int,vector<PER_FOR_MCS *> >::iterator tableIter;
	for (tableIter = mcsMapOfSNR2PER.begin();tableIter != mcsMapOfSNR2PER.end(); tableIter++)
	{
		vector<PER_FOR_MCS *> & vec = tableIter->second;
		for (int i=0;i<(int) vec.size(); i++)
		{
			delete vec[i];
			vec[i] = NULL;
		}
	}
}

vector<PER_FOR_MCS *> * PER2SNRTable::GetMCSAndPER(double SNR) const
{
	vector<PER_FOR_MCS *> * pVector = new vector<PER_FOR_MCS *>;
	map<int,vector<PER_FOR_MCS *> >::const_iterator tableIter;
	for (tableIter = mcsMapOfSNR2PER.begin();tableIter != mcsMapOfSNR2PER.end(); tableIter++)
	{
		int mcs = tableIter->first;
		const vector<PER_FOR_MCS *> & vec = tableIter->second;
		double per = CalcPERFromSNRTable(vec, SNR);
		PER_FOR_MCS * perForMCS = new PER_FOR_MCS(*(vec[0]));
		perForMCS->SNR = SNR;
		perForMCS->PER = per;
		pVector->push_back(perForMCS);
	}

	return pVector;
}

double PER2SNRTable::CalcPERFromSNRTable(const vector<PER_FOR_MCS *> & vec, double SNR) const
{
	float myEPS = 1e-2;
	// too small vector
	if (vec.size() < 2)
		return 1.;

	// exterpolate - not very smart, can we do better?
	if ((vec[0]->SNR+myEPS) >= SNR)
		return vec[0]->PER;

	if ((vec[vec.size()-1]->SNR-myEPS) <= SNR)
		return vec[vec.size()-1]->PER;

	// SNR is inside the region - linear interpolation
	for (int i=0;i<(int) vec.size();i++)
	{
		if (fabsf(vec[i]->SNR - SNR) <= myEPS)
			return vec[i]->PER;

		if (vec[i]->SNR > SNR)  // note that i > 0 
		{
			double interpolatedPER;
			double diff_below = SNR - vec[i-1]->SNR;
			double per_below = vec[i-1]->PER;
			double diff_above = vec[i]->SNR - SNR;
			double per_above = vec[i]->PER;

			interpolatedPER = (diff_above*per_below + diff_below*per_above)/(diff_below + diff_above);
			return interpolatedPER;
		}
	}
	return 1.;  // I will never get here
}
