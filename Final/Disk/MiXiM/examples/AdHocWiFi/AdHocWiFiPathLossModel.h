/*********************************************************
 * File:        AdHocWiFiPathLossModel.h
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: signal path loss model implementation
 **************************************************************************/

#ifndef AD_HOC_WI_FI_PATH_LOSS_MODEL_H_
#define AD_HOC_WI_FI_PATH_LOSS_MODEL_H_

#include "AnalogueModel.h"

class AdHocWiFiPathLossModel;
class AdHocWiFiMac;

class AdHocWiFiPathLossModelMapping: public SimpleConstMapping {
protected:
	static DimensionSet dimensions;

	/** Pointer to the model to get parameters from */
	AdHocWiFiPathLossModel* model;

	double distance;

	const Signal& signal;

public:
	AdHocWiFiPathLossModelMapping(AdHocWiFiPathLossModel* model,
					   double distanceP,
					   const Signal& s,
					   const Argument& start,
					   const Argument& end):
		SimpleConstMapping(dimensions, start, end, Argument(0.01)), // last argument is interval to iterate over when calcluating attenuation
		model(model),distance(distanceP),signal(s)
	{}

	virtual double getValue(const Argument& pos) const;

	/**
	 * @brief creates a clone of this mapping.
	 *
	 * This method has to be implemented by every subclass.
	 * But most time the implementation will look like the
	 * implementation of this method (except of the class name).
	 */
	ConstMapping* constClone() const
	{
		return new AdHocWiFiPathLossModelMapping(*this);
	}
};

/**
 * Implements the project path loss model PathLossModel after.
 */
class AdHocWiFiPathLossModel: public AnalogueModel {
public:
	// type of model to use
	enum Model {ALPHA_BETA,ELBIT};

protected:
	friend class AdHocWiFiPathLossModelMapping;

	/** @brief HostMove of this analogue models host. */
	Move* hostMove;

	/* model parameters */
	double alpha,beta;

	// Elbit parameters
	double elbit_c,elbit_htr,elbit_hrc;

	Model model;

	// computation temporaries of Elbit macrocell formula
	double elbit_additive;
	double elbit_dist_gain;


public:

	/**
	 * path loss model
	 * alphaP - alpha parameter
	 * betaP - alpha parameter
	 * cP - Elbit c parameter
	 * htrP - Elbit htr parameter
	 * hrcP - Elbit hrc parameter
	 * model - model choose (alpha/beta or elbit)
	 */
	AdHocWiFiPathLossModel(Move* hostMove,double alphaP,double betaP,
			double cP,double htrP,double hrcP,
			Model modelP);

	virtual ~AdHocWiFiPathLossModel() {}

	/**
	 * filter signal according to path loss model
	 */
	virtual void filterSignal(Signal& s);
};

#endif
