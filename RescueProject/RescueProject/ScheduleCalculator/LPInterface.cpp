#include "LPInterface.h"

#include <CoinPackedMatrix.hpp>
#include "ClpSimplex.hpp"
#include "ClpPrimalColumnSteepest.hpp"
#include "ClpPrimalColumnDantzig.hpp"

#include <vector>
using namespace std;

LPInterface::LPInterface(void)
{}

bool LPInterface::SolveLP(CommunicationGraph & commGraph, InterferenceGraph & interGraph, RequestsCollection & requests, 
							SchedulingParams & params, double *& solution)
{
	// calculate the number of variables: #freq * # requests * #directed edges
	int nFreq = params.N_FREQS_FOR_SCHEDULE;
	vector<CommunicationRequest*> requestsArray = requests.GetRequests();
	int nReq = (int) requestsArray.size();
	int nVertices = commGraph.GetNumVertices();
	int nCommEdges = commGraph.GetNumEdges();

	int nVars = nFreq * nReq * nCommEdges + 1; 

	// each variable is a triplet (edge #, request #, freq) denoted f_{i,j}(e) (i-request, j-freq).
	// the numbering of the variables is 0...nVars-1 in the following order:
	//  (e=1,r=1,f=1), (e=1,r=1,f=2)...(e=1,r=1,f=nFreq), 
	//  (e=1,r=2,f=1), (e=1,r=2,f=2)...(e=1,r=2,f=nFreq), 
	//   ... 
	//  (e=1,r=nReq,f=1), (e=1,r=nReq,f=2)...(e=1,r=nReq,f=nFreq), 
	//  ...
	//  ...
	//  (e=nInterEdges,r=1,f=1), (e=nInterEdges,r=1,f=2)...(e=nInterEdges,r=1,f=nFreq), 
	//   ... 
	//  (e=nInterEdges,r=nReq,f=1), (e=nInterEdges,r=nReq,f=2)...(e=nInterEdges,r=nReq,f=nFreq)
	//   \rho  (this is variable number nVars-1)


	// the constraints excluding the upper/lower bound on the variables are:
	// type I - flow conservation \forall i,v 
	//    \Sum_{j, e\in in(v)} f_{i,j} (e) - \Sum_{j, e\in out(v)} f_{i,j} (e) = 0
	int nTypeOneConstraints = (nVertices-2)*nReq;

	// type II - maximizing the flow leaving s_i \forall i 
	// \Sum_{j, e\in in(s_i)} f_{i,j} (e) - \Sum_{j, e\in out(s_i)} f_{i,j} (e) >= \rho d_i
	int nTypeTwoConstraints = nReq;

	// type III - a single transmitter-reciever in each node \forall v 
	// \Sum_{u \in N(v),i,j} (f_{i,j}(e)+f_{i,j}(e^R)) / c(e) \le 1, 
	// where N(v) are the neighbors of v; e=(u,v) and e^R=(v,u) are communication edges
	int nTypeThreeConstraints = commGraph.GetNumVertices();

	// type IV - interference within a frequency  \forall e, j
	// \Sum_{i, e' \in conf(e) \cup \{e\}} (f_{i,j}(e') + f_{i,j}(e'^R)) / c(e') \<= 1
	// where e is an edge in the communication graph, e' are edges in the communication
	// graph that interferes e, i.e., if e=(u,v) and e'=(w,x) than either w or x equals u or v, 
	// or one of the edge (u,w), (u,x), (v,w), (v,x) is in the interference graph
	int nTypeFourConstraints = commGraph.GetNumEdges()*nFreq;

	int nAllConstraints = nTypeOneConstraints + nTypeTwoConstraints +
		nTypeThreeConstraints + nTypeFourConstraints;

	// set lower and upper bounds for the variables and set the objective.
	int i;
	solution = new double[nVars];
	double * collb = new double[nVars]; 
	double * colub = new double[nVars]; 
	double * obj   = new double [nVars];
	for (i=0;i<nVars;i++)
	{
		collb[i] = 0.;
		colub[i] = 1.;
		obj  [i] = 0.;
	}
	obj[nVars-1] = -1;  // rho

	
	
	//void setOptimizationDirection(double value)
//double optimizationDirection()  These methods set and get the objective sense. The parameter value should be +1 to minimize, -1 to maximize, and 0 to ignore.  



/*	int NCOLS = nVars; // matrix columns corresponds to variables.
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

*/
	return true;
}
