/*********************************************************
 * File:        AdHocWiFiPathLossModel.cpp
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: signal path loss model implementation
 **************************************************************************/


#include "AdHocWiFiPathLossModel.h"
#include "AdHocWiFiMac.h"
#include <math.h>
#include <stdio.h>
using namespace std;

AdHocWiFiPathLossModel::AdHocWiFiPathLossModel(Move* hostMove,double alphaP,double betaP,
			double cP,double htrP,double hrcP,
			Model modelP):
		hostMove(hostMove),alpha(alphaP),beta(betaP),elbit_c(cP),
		elbit_htr(htrP),elbit_hrc(hrcP),model(modelP)
{
///	double Ploss = (44.9-6.55*LOG10(elbit_htr))*LOG10(distance/1000)+45.5+(35.46-1.1*elbit_hrc)*LOG10(2.4*10^3)-13.82*LOG10(elbit_htr)+0.7*elbit_hrc;

	if (model==ELBIT) {
		elbit_additive = 45.5+(35.46-1.1*elbit_hrc)*log10(2400)-13.82*log10(elbit_htr)+0.7*elbit_hrc;
		elbit_dist_gain = (44.9-6.55*log10(elbit_htr));
	}
}

void AdHocWiFiPathLossModel::filterSignal(Signal& s)
{

	simtime_t start = s.getSignalStart();
	simtime_t end = start + s.getSignalLength();

	const Move& senderMove = s.getMove();
	// assuming our host/sender don't move
	double distance = senderMove.startPos.distance(hostMove->startPos);

	//add a new AdHocWiFiPathLossModelMapping to the signals attenuation list
	s.addAttenuation(new AdHocWiFiPathLossModelMapping(this,distance,s,
											  Argument(start),
											  Argument(end)));
//  add another model ? -	s.addAttenuation(...);
}

#include <iostream>
DimensionSet AdHocWiFiPathLossModelMapping::dimensions(Dimension::time);

double AdHocWiFiPathLossModelMapping::getValue(const Argument& pos) const {


	long double att;
	double Ploss;
	if (model->model==AdHocWiFiPathLossModel::ALPHA_BETA) {
		// implementing Path Loss Model as is appendix A according to the formula:
		Ploss = model->beta + model->alpha*log10(distance);
	}
	else {
		// implementing according to Elbit model (Macrocell formula as computed by Sasha)
		assert(model->model==AdHocWiFiPathLossModel::ELBIT);
		Ploss = distance ? model->elbit_dist_gain*log10(distance/1000) : 0;
		Ploss += model->elbit_additive;
	}
	// Ploss is in dB
	att = 1/(pow(10,Ploss/10));


	//	cout << __FUNCTION__ << " distance :" << distance << "; att=" << att << " Ploss=" << Ploss << endl;

	return att;
}
