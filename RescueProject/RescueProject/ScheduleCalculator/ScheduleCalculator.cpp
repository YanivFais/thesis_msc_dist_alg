// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.

#include <CoinPackedMatrix.hpp>
/*#include "ClpSimplex.hpp"
#include "ClpPrimalColumnSteepest.hpp"
#include "ClpPrimalColumnDantzig.hpp"
#include "UnicastList.h"
#include "RescueScheduler.h"
*/
using namespace std;
#include <time.h>
#include <iostream>
#include "SchedulingParams.h"
#include "CommunicationGraph.h"
#include "InterferenceGraph.h"
#include "RequestsCollection.h"
#include "LPInterface.h"

bool ReadInputs(CommunicationGraph & commGraph, InterferenceGraph & interGraph, RequestsCollection & requests, SchedulingParams & params)
{	
	if (! commGraph.InitFromFiles (params.COMMUNICATION_GRAPH_FILENAME,params.COORDS_FILENAME))
	{
		cout << "Error reading communication graph: " << params.COMMUNICATION_GRAPH_FILENAME 
			 << " , " << params.COORDS_FILENAME << endl;
		return false;
	}

	if (! interGraph.InitFromFiles(params.INTERFERENCE_GRAPH_FILENAME,params.COORDS_FILENAME))
	{
		cout << "Error reading interference graph: " << params.INTERFERENCE_GRAPH_FILENAME << endl;
		return false;
	}

	if (! requests.InitFromFile(params.REQUESTS_FILENAME))
	{
		cout << "Error reading requests: " << params.REQUESTS_FILENAME << endl;
		return false;
	}
	return true;
}

int main (int argc, const char *argv[])
{
	if (argc != 2)
	{
		cout << "Wrong number of parameters" << endl;
		cout << "ScheduleCalculator <config file name>" << endl;
		return 1;
	}

	// read configuration file
	SchedulingParams params(argv[1]);
	if (params.mIsValid == false)
	{
		cout << "Error reading parameters file" << endl;
		return 1;
	}

    CommunicationGraph commGraph;
    InterferenceGraph interGraph;
	RequestsCollection  requests;
	if (ReadInputs(commGraph, interGraph, requests, params) == false)
	{
		cout << "Error reading input files" << endl;
		return 1;
	}

	double * solution;
	LPInterface lpSolver;
	lpSolver.SolveLP(commGraph, interGraph, requests, params, solution);

	return 0;
}  


/*
	//Gloabl Parameters of the Algorithm
    int ncols_lp, nrows_lp;
	double value_lp;
    CommunicationGraph comm_graph_lp;

	char graphFullFileName[MAX_FULL_FILENAME];
	strcpy(graphFullFileName,examplesPath);
	strcat(graphFullFileName,graphFileName);
	comm_graph_lp.initFromFile(graphFullFileName); //Read the Communication Graph

    CoinPackedMatrix matrix;
	UnicastList req;

//Fastest for Random Graph
//    ncols_lp = 25;
//    nrows_lp = 100;

    ncols_lp = 170000;
    nrows_lp = 30000;

	int * NCOLS;
	NCOLS = new int [1]; //A dynamic memory allocation of LP matrix dimensions
	*NCOLS = ncols_lp;
	int *NROWS;
	NROWS = new int [1]; //A dynamic memory allocation of LP matrix dimensions
	*NROWS = nrows_lp;


	matrix.setDimensions(*NROWS,*NCOLS); //have to ve set before the first element is loaded to the matrix!!! How can I know the dimesions before I read the files?!

	double * collb = new double [*NCOLS]; 
	double * colub = new double [*NCOLS]; 
	double * obj   = new double [*NCOLS];
	double * rowlb = new double [*NROWS];
	double * rowub = new double [*NROWS];


	char requestsFullFileName[MAX_FULL_FILENAME];
	strcpy(requestsFullFileName,examplesPath);
	strcat(requestsFullFileName,requestsFileName);
	req.initFromFile(requestsFullFileName, ncols_lp, nrows_lp, *collb, *colub, *obj, *rowlb, *rowub,  
		matrix, comm_graph_lp); //Read the Requirements Graph and Return the CLP Matrix


	time_t preloadTime;
	time(&preloadTime);
	cout << "End of preload: " << preloadTime << endl;

//ClpPrimalColumnSteepest steep(0); // 0 devex, 1 steepest
//ClpPrimalColumnDantzig steep;

//ClpPrimalColumnPivot steep(1);
	ClpSimplex  model;

//    ClpPrimalColumnSteepest steep(1); // 0 devex, 1 steepest
 //   model.setPrimalColumnPivotAlgorithm(steep);


	model.loadProblem (matrix, collb, colub, obj, rowlb, rowub); 
	model.primal();

	int numberRows = model.numberRows();
	double * rowPrimal = model.primalRowSolution();
*/
	/*
	for (int iRow=0;iRow<numberRows;iRow++) 	
		printf("Row %d, primal %g\n",iRow, rowPrimal[iRow]);
*/
		
/*	int numberColumns = model.numberColumns();
	double * columnPrimal = model.primalColumnSolution();
*/
/*
The location of variables as follows:
[0,#number of edges] - by numbering which appears in graph.txt by direction from (from->to)
[#number of edges+1,2*#number of edges] - opposite direction of the edges (i+N).
Frequencies: for a first frequency, for a second, etc...
[frequency number * edge number]
Commodities:
Multipliing similarly for frequencies - external blocks.
[commodity number * frequency number * edge number]
*/

/*

	for (int iColumn=0;iColumn<numberColumns;iColumn++) 	{
		if (columnPrimal[iColumn] < 0)
			printf("Column %d, primal %g\n",iColumn,columnPrimal[iColumn]);
	}

	time_t simplexTime;
	time(&simplexTime);

	cout << "End of execution: " << simplexTime << endl;

	RescueScheduler scheduler(&comm_graph_lp,req.GetNumFreqs(), req.GetNumRequests());
	if (scheduler.IsOK() == false)
	{
		cout << "Problem initializing the scheduler" << endl;
	}
	else
	{
		if (scheduler.CalcSchedule(columnPrimal, numberColumns) == false)
		{
			cout << "Problem in scheduler " << endl;
		}
		else
		{
			char scheduleFullFileName[MAX_FULL_FILENAME];
			strcpy(scheduleFullFileName,examplesPath);
			strcat(scheduleFullFileName,scheduleFilename);
			if (scheduler.WriteSchedule(scheduleFullFileName) == false)
			{
				cout << "Problems writing the schedule" << endl;
			}
			else
			{
				cout << "Finished scheduling the flow" << endl;
			}

		}
	}
	time_t scheduleTime;
	time(&scheduleTime);

	cout << "End of schedule: " << scheduleTime << endl;

	return 0;
*/



/* @@@@@@@@@@@@@
#include <CoinPackedMatrix.hpp>
#include "ClpSimplex.hpp"

int main (int argc, const char *argv[])
{

	
	const int NCOLS = 3;
	const int NROWS = 3;
	const double HUGE_NUM = 1e8;
	CoinPackedMatrix matrix;
	matrix.setDimensions(NROWS,NCOLS);
	matrix.modifyCoefficient(0,0,1.);
	matrix.modifyCoefficient(0,1,1.);
	matrix.modifyCoefficient(1,1,1.);
	matrix.modifyCoefficient(1,2,1.);
	matrix.modifyCoefficient(2,0,1.);
	matrix.modifyCoefficient(2,2,1.);
 

	int i;
	double * collb = new double[NCOLS]; 
	double * colub = new double[NCOLS]; 
	double * obj   = new double [NCOLS];
	for (i=0;i<NCOLS;i++)
	{
		collb[i] = 0.;
		colub[i] = 1.;
		obj  [i] = 1.;
	}
	double * rowlb = new double [NROWS];
	double * rowub = new double [NROWS];
	for (i=0;i<NROWS;i++)
	{
		rowlb[i] = 1.;
		rowub[i] = 10.;
	}
	ClpSimplex  model;
	model.loadProblem (matrix, collb, colub, obj, rowlb, rowub); 
	model.primal();

	int numberRows = model.numberRows();
	double * rowPrimal = model.primalRowSolution();

	int iRow;

	for (iRow=0;iRow<numberRows;iRow++) 	
		printf("Row %d, primal %g\n",iRow, rowPrimal[iRow]);
	
	int numberColumns = model.numberColumns();
	double * columnPrimal = model.primalColumnSolution();

	int iColumn;

	for (iColumn=0;iColumn<numberColumns;iColumn++) 	
			printf("Column %d, primal %g\n",iColumn,columnPrimal[iColumn]);

	return 0;
}  
*/