#include <iostream>
#include "SchedulingParams.h"
#include "ConfigReader.h"

#include <string.h>

using namespace std;

const char * SchedulingParams::DEFAULT_COMMUNICATION_GRAPH_FILENAME = "CommGraph.txt";
const char * SchedulingParams::DEFAULT_INTERFERENCE_GRAPH_FILENAME = "InterfGraph.txt";
const char * SchedulingParams::DEFAULT_COORDS_FILENAME = "Coords.txt";
const char * SchedulingParams::DEFAULT_REQUESTS_FILENAME = "Requests.txt";

const int    SchedulingParams::DEFAULT_N_FREQS_FOR_SCHEDULE = 3;

SchedulingParams::SchedulingParams(const char * configFileName)
{
	ConfigReader params(configFileName);
	if (params.hasFoundFile() == false)
	{
		cout << "Problem reading the scenario parameters file" << configFileName << endl;
		return;
	}

	N_FREQS_FOR_SCHEDULE = params.findInt("N_FREQS_FOR_SCHEDULE",DEFAULT_N_FREQS_FOR_SCHEDULE);

	string DEFAULT_COMMUNICATION_GRAPH_FILENAME_(DEFAULT_COMMUNICATION_GRAPH_FILENAME);
	string helper = params.findString("COMMUNICATION_GRAPH_FILENAME",DEFAULT_COMMUNICATION_GRAPH_FILENAME_);
	COMMUNICATION_GRAPH_FILENAME  = new char[helper.size()+1];
	strcpy(COMMUNICATION_GRAPH_FILENAME, helper.c_str()); 

	string DEFAULT_INTERFERENCE_GRAPH_FILENAME_(DEFAULT_INTERFERENCE_GRAPH_FILENAME);
	helper = params.findString("INTERFERENCE_GRAPH_FILENAME",DEFAULT_INTERFERENCE_GRAPH_FILENAME_);
	INTERFERENCE_GRAPH_FILENAME  = new char[helper.size()+1];
	strcpy(INTERFERENCE_GRAPH_FILENAME, helper.c_str()); 

	string DEFAULT_COORDS_FILENAME_(DEFAULT_COORDS_FILENAME);
	helper = params.findString("COORDS_FILENAME",DEFAULT_COORDS_FILENAME_);
	COORDS_FILENAME = new char[helper.size()+1];
	strcpy(COORDS_FILENAME,  helper.c_str()); 

	string DEFAULT_REQUESTS_FILENAME_(DEFAULT_REQUESTS_FILENAME);
	helper = params.findString("REQUESTS_FILENAME",DEFAULT_REQUESTS_FILENAME_);
	REQUESTS_FILENAME = new char[helper.size()+1];
	strcpy(REQUESTS_FILENAME,  helper.c_str()); 

	mIsValid = true;

}

SchedulingParams::~SchedulingParams(void)
{
	if (COORDS_FILENAME != NULL)
	{
		delete [] COORDS_FILENAME;
	}
	if (COMMUNICATION_GRAPH_FILENAME != NULL)
	{
		delete [] COMMUNICATION_GRAPH_FILENAME;
	}
	if (INTERFERENCE_GRAPH_FILENAME != NULL)
	{
		delete [] INTERFERENCE_GRAPH_FILENAME;
	}
	if (REQUESTS_FILENAME != NULL)
	{
		delete [] REQUESTS_FILENAME;
	}
}
