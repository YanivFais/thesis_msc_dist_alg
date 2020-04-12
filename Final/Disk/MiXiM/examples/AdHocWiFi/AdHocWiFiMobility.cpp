/*********************************************************
 * File:        AdHocWiFiMobility.cc
 *
 * Author:      Yaniv Fais <yaniv.fais@gmail.com>
 *
 *
 ***************************************************************************
 * description: handle mobility of nodes from scenario generator files
 **************************************************************************/

#include "AdHocWiFiMobility.h"
#include <iomanip>
#include <sstream>
#include <fstream>
using namespace std;

#include <assert.h>

Define_Module(AdHocWiFiMobility);

// static
AdHocWiFiMobility::TimedPositions AdHocWiFiMobility::timedPositions;
double AdHocWiFiMobility::sUpdateInterval;

AdHocWiFiMobility::AdHocWiFiMobility() : index(0) {
}

AdHocWiFiMobility::~AdHocWiFiMobility() {
}

  /** @brief Initializes mobility model parameters.*/
void AdHocWiFiMobility::initialize(int stage)
{
	BaseMobility::initialize(stage);

	sUpdateInterval = SIMTIME_DBL(updateInterval);

	if (timedPositions.size()==0) {
		const char * fileNameBase = par("coords_file");
		int coordsNumber = par("coords_number");
		Positions::readFromFiles(fileNameBase,coordsNumber,timedPositions);
	}

	if (stage==0) {
		int channel;
		sscanf(hostPtr->getFullPath().c_str(),"sim.channel%d_node[%d]",&channel,&ID);
		if (ID==0) {
			if (channel==1) {
				cOutVector playground("PlaygroundSize");
				playground.recordWithTimestamp((simtime_t)1,playgroundSizeX());
			}
			ID = 1; // 0 is dummy and linked to 1
		}
		makeMove();
		index--;
	}

}

void AdHocWiFiMobility::makeMove()
{

    EV << "start makeMove " << move.info() << endl;

    move.startTime = simTime();
    Coord stepTarget;
    Positions positions;
	Position position;
	if (timedPositions.size()<(unsigned)index+1) {
		EV << "No more positions\n";
		positions = timedPositions[timedPositions.size()-1];
		position = positions.data[ID];
		position.x = move.startPos.getX() + (position.vx* updateInterval.dbl());
		position.y = move.startPos.getY() + (position.vy* updateInterval.dbl());
		stepTarget.setX(position.x);
		stepTarget.setY(position.y);
	}
	else {
		positions = timedPositions[index++];
		position = positions.data[ID];
	    Coord stepTarget1(position.x,position.y);
		stepTarget = stepTarget1;
	}
    move.startPos = stepTarget;


	move.speed = sqrt(position.vx*position.vx + position.vy*position.vy);
	if (move.speed==0)
	  move.speed = 1E-20; // just mark as moving

	EV << "Move:" << ID << " @" << SIMTIME_DBL(simTime()) << " " << move.startPos.getX() << "," << move.startPos.getY() << " " << move.speed << ":" << position.vx << "-" << position.vy << endl;

	Position nextPosition;
	if (timedPositions.size()>(unsigned)index+1) {
    	Positions nextPositions = timedPositions[index];
    	nextPosition = nextPositions.data[ID];
    }
	else {
		nextPosition.x = move.startPos.getX() + (position.vx* updateInterval.dbl());
		nextPosition.y = move.startPos.getY() + (position.vy* updateInterval.dbl());
	}
	if (nextPosition.x < 0)
		nextPosition.x = 1;
	if (nextPosition.y < 0)
		nextPosition.y = 1;
	if (nextPosition.x > playgroundSizeX())
		nextPosition.x = playgroundSizeX()-1;
	if (nextPosition.y > playgroundSizeY())
		nextPosition.y = playgroundSizeY()-1;
    stepTarget.setX(nextPosition.x);
    stepTarget.setY(nextPosition.y);
    move.setDirection(stepTarget);


    fixIfHostGetsOutside();

}

void AdHocWiFiMobility::fixIfHostGetsOutside()
{
	if ((move.direction.getX() > playgroundSizeX()) || (move.direction.getX()<0) ||
	    (move.direction.getY() > playgroundSizeY()) || (move.direction.getY()<0))  {
		if (move.direction.getX() < 0)
			move.direction.setX(1);
		if (move.direction.getY() < 0)
			move.direction.setY(1);
		if (move.direction.getX() > playgroundSizeX())
			move.direction.setX(playgroundSizeX()-1);
		if (move.direction.getY() > playgroundSizeY())
			move.direction.setY(playgroundSizeY()-1);
	}
}

bool AdHocWiFiMobility::Positions::readFromFiles(const string& fileName,int number,TimedPositions& timedPositions)
{
	Positions oldPositions;
	for (int i=0;i<number;i++) {
		Positions positions;
		stringstream coordsFileName;
		coordsFileName << fileName << "_" << setfill('0') << setw(3) << i << ".000.txt";
		if (!positions.readFromFile(coordsFileName.str())) {
			//positions = oldPositions;
			break;
		}
		if (i==0) {
			cOutVector x0("Initial_Positions_X");
			cOutVector y0("Initial_Positions_Y");
			int j=1;
			for (map<int,Position>::iterator it = positions.data.begin(); it != positions.data.end(); it++,j++) {
				x0.recordWithTimestamp((simtime_t)j,it->second.x);
				y0.recordWithTimestamp((simtime_t)j,it->second.y);
			}
		}
		timedPositions.push_back(positions);
		oldPositions = positions;
	}
	return true;
}

bool AdHocWiFiMobility::Positions::readFromFile(const string& fileName)
{
	ifstream coordsFile;
	coordsFile.open(fileName.c_str());
    if (!coordsFile)
	{
		cerr << "Unable to open file " << fileName << endl;
        return false;
    }

	string line;
	bool first = true;
	int numElements;
	int item = 1;
    while (getline(coordsFile,line))
    {
		if ( (line.size() == 0) ||
             ((line.size() >= 1) && (line[0] == '#') ))
        {
			continue;  // comment
        }
        else
        {
			stringstream ss (line);
			string token;
            getline(ss, token, ',');
			if (first) {
				first = false;
				numElements = atoi(token.c_str());
			}
			else {
				// format: id,x,y,vx,vy
				int id = atoi(token.c_str());
				assert(id==item++);
				getline(ss, token, ',');
				Position p;
				p.x = atof(token.c_str());
				getline(ss, token, ',');
				p.y = atof(token.c_str());
				getline(ss, token, ',');
				p.vx = atof(token.c_str());
				getline(ss, token, ',');
				p.vy = atof(token.c_str());
				data[id] = p;
			}
        }
    }
    assert((unsigned)numElements==data.size());
    coordsFile.close();
	return true;
}

