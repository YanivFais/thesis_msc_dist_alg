/*********************************************************
 * File:        AdHocWiFiMobiliy.h
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: handles nodes mobility according to scenario generated files
 **************************************************************************/


#ifndef ADHOCWIFIMOBILITY_H_
#define ADHOCWIFIMOBILITY_H_

#include "BaseMobility.h"
#include "AdHocWiFi.h"
using namespace std;

class AdHocWiFiMobility: public BaseMobility {
public:
	AdHocWiFiMobility();
	virtual ~AdHocWiFiMobility();

public:

  /** @brief Initializes mobility model parameters.*/
  virtual void initialize(int stage);

  /**
   * @return global update interval (schedule round repeat time)
   */
  static double getUpdateInterval() { return sUpdateInterval; }

protected:

  virtual void fixIfHostGetsOutside();

  virtual void makeMove();

private:

	// index in timed positions
	int index;

	// ID of host
	int ID;
	
	/**
	 * position on surface and speed
	 */
	struct Position {
		double x,y,vx,vy;
	};

	/**
	 * positions data
	 */
	struct Positions {
	  // node id to position
		map<int,Position> data;
	  
	  /**
	   * read from a file
	   * @param fileName name of file to read
	   */
	  bool readFromFile(const string& fileName);

	  /**
	   * 
	   */
	  static bool readFromFiles(const string& fileName,int number,vector<Positions>& timedPositions);
	};

	typedef vector<Positions> TimedPositions;

	static TimedPositions timedPositions;

	/**
	 * global update interval - schedule time
	 */
	static double sUpdateInterval;

};

#endif /* ADHOCWIFIMOBILITY_H_ */
