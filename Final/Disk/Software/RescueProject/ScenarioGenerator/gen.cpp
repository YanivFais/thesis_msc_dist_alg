// gen.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include "GenerationParams.h"
#include "PER2SNRTable.h"
#include "RateCalculator.h"
#include "GraphGenerator.h"
#include "RequestsCollection.h"

using namespace std;

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		cout << "Wrong number of parameters" << endl;
		cout << "ScenarioGenerator <config file name>" << endl;
		return 1;
	}

	// read configuration file
	GenerationParams params(argv[1]);
	if (params.mIsValid == false)
		return 1;


	// read configuration file
	PER2SNRTable perTable(params.SNR_TO_PER_FILE_NAME);
	if (perTable.mIsValid == false)
	{
		cout << "Error reading PER to SNR table" << endl;
		return 1;
	}

	RateCalculator rateCalculator(params, &perTable);

	GraphGenerator generator(params, (const RateCalculator &) rateCalculator, params.SEED_FOR_GENERATOR);

	CommunicationGraph * communicationGraph;
	InterferenceGraph  * interferenceGraph;
	generator.GenerateGraphs(communicationGraph, interferenceGraph);
	RequestsCollection & requests = generator.GenerateRequests(*communicationGraph);
	
	generator.WriteGraphFiles(*communicationGraph,*interferenceGraph);
	generator.WriteRequestsFile(requests);

	return 0;
}




