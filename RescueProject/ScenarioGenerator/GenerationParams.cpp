#include <iostream>
#include "GenerationParams.h"
#include "ConfigReader.h"

#include <string.h>

const int GenerationParams::DEFAULT_TIME_PERIOD = 60;
const int GenerationParams::DEFAULT_TIME_RESOLUTION = 1;
const int GenerationParams::DEFAULT_N_CLIENTS = 10;
const int GenerationParams::DEFAULT_N_NEAR_REQUEST = 2;
const int GenerationParams::DEFAULT_N_FAR_REQUEST = 1;
const double GenerationParams::DEFAULT_FAR_THRESHOLD = 0.3;
const int GenerationParams::DEFAULT_SCALE_IN_METERS = 30000;
const double GenerationParams::DEFAULT_MEAN_VELOCITY = 0.2;  
const double GenerationParams::DEFAULT_STD_VELOCITY = 0.1;
const double GenerationParams::DEFAULT_MAX_VELOCITY = 0.4;

const double GenerationParams::DEFAULT_TRANSMISSION_POWER_IN_MILI_WATTS = 1.;
const double GenerationParams::DEFAULT_NOISE_IN_DBM = -101.7;
const char * GenerationParams::DEFAULT_SNR_TO_PER_FILE_NAME = "snr2per.txt";
const double GenerationParams::DEFAULT_D_BREAKPOINT = 20.;
const double GenerationParams::DEFAULT_SF_BEFORE_BREAKPOINT = 3.;
const double GenerationParams::DEFAULT_SF_AFTER_BREAKPOINT = 6.;
const double GenerationParams::DEFAULT_NUM_STD_FOR_DECAY = 3.;
const double GenerationParams::DEFAULT_FREQUENCY = 2.4;

const double GenerationParams::DEFAULT_MIN_SNR_FOR_COMMUNICATION = 20.;
const double GenerationParams::DEFAULT_MIN_SNR_FOR_INTERFERENCE = 5.;

const double GenerationParams::DEFAULT_HEADER_TIME = 4.;
const double GenerationParams::DEFAULT_PREAMBLE_TIME = 16.;
const double GenerationParams::DEFAULT_MAC_SERVICE_TRAILER_TIME = 8.;
const double GenerationParams::DEFAULT_MAC_HEADER_LEN = 240.;
const double GenerationParams::DEFAULT_MAC_FCS_LEN = 32.;
const double GenerationParams::DEFAULT_MAC_RTS_LEN = 160.;
const double GenerationParams::DEFAULT_MAC_CTS_LEN = 112.;
const double GenerationParams::DEFAULT_MAC_ACK_LEN = 112.;
const double GenerationParams::DEFAULT_DIFS_TIME = 28.;
const double GenerationParams::DEFAULT_SIFS_TIME = 10.;
const double GenerationParams::DEFAULT_CW_TIME = 135.;
// in kilo-byte
const double GenerationParams::DEFAULT_PAYLOAD = 2.;

const char * GenerationParams::DEFAULT_COMMUNICATION_GRAPH_FILENAME = "CommGraph.txt";
const char * GenerationParams::DEFAULT_INTERFERENCE_GRAPH_FILENAME = "InterfGraph.txt";
const char * GenerationParams::DEFAULT_COORDS_FILENAME = "Coords.txt";
const char * GenerationParams::DEFAULT_REQUESTS_FILENAME = "Requests.txt";


using namespace std;
GenerationParams::GenerationParams(const char * configFileName)
{
	mIsValid = false;
	ConfigReader params(configFileName);
	if (params.hasFoundFile() == false)
	{
		cout << "Problem reading the scenario parameters file" << configFileName << endl;
		return;
	}

	N_CLIENTS                = params.findInt   ("N_CLIENTS",DEFAULT_N_CLIENTS);
	TIME_PERIOD              = params.findInt   ("TIME_PERIOD",DEFAULT_TIME_PERIOD);
	TIME_RESOLUTION          = params.findInt   ("TIME_RESOLUTION",DEFAULT_TIME_RESOLUTION);
	N_NEAR_REQUEST           = params.findInt   ("N_NEAR_REQUEST",DEFAULT_N_NEAR_REQUEST);
	N_FAR_REQUEST            = params.findInt   ("N_FAR_REQUEST",DEFAULT_N_FAR_REQUEST);
	FAR_THRESHOLD            = params.findDouble("FAR_THRESHOLD",DEFAULT_FAR_THRESHOLD);
	SCALE_IN_METERS          = params.findInt   ("SCALE_IN_METERS",DEFAULT_SCALE_IN_METERS);
	MEAN_VELOCITY            = params.findDouble("MEAN_VELOCITY",DEFAULT_MEAN_VELOCITY);
	STD_VELOCITY             = params.findDouble("STD_VELOCITY",DEFAULT_STD_VELOCITY);
	MAX_VELOCITY             = params.findDouble("MAX_VELOCITY",DEFAULT_MAX_VELOCITY);

	TRANSMISSION_POWER_IN_MILI_WATTS = params.findDouble("TRANSMISSION_POWER_IN_MILI_WATTS",DEFAULT_TRANSMISSION_POWER_IN_MILI_WATTS);
	NOISE_IN_DBM = params.findDouble("NOISE_IN_DBM",DEFAULT_NOISE_IN_DBM);
	D_BREAKPOINT = params.findDouble("D_BREAKPOINT",DEFAULT_D_BREAKPOINT);
	SF_BEFORE_BREAKPOINT = params.findDouble("SF_BEFORE_BREAKPOINT",DEFAULT_SF_BEFORE_BREAKPOINT);
	SF_AFTER_BREAKPOINT = params.findDouble("SF_AFTER_BREAKPOINT",DEFAULT_SF_AFTER_BREAKPOINT);
	NUM_STD_FOR_DECAY = params.findDouble("NUM_STD_FOR_DECAY", DEFAULT_NUM_STD_FOR_DECAY);
	FREQUENCY = params.findDouble("FREQUENCY", DEFAULT_FREQUENCY);

	MIN_SNR_FOR_COMMUNICATION = params.findDouble("MIN_SNR_FOR_COMMUNICATION",DEFAULT_MIN_SNR_FOR_COMMUNICATION);
	MIN_SNR_FOR_INTERFERENCE = params.findDouble("MIN_SNR_FOR_INTERFERENCE",DEFAULT_MIN_SNR_FOR_INTERFERENCE);

	HEADER_TIME = params.findDouble("HEADER_TIME",DEFAULT_HEADER_TIME);
	PREAMBLE_TIME = params.findDouble("PREAMBLE_TIME", DEFAULT_PREAMBLE_TIME);
	MAC_SERVICE_TRAILER_TIME = params.findDouble("MAC_SERVICE_TRAILER_TIME",DEFAULT_MAC_SERVICE_TRAILER_TIME);
	MAC_HEADER_LEN = params.findDouble("MAC_HEADER_LEN",DEFAULT_MAC_HEADER_LEN);
	MAC_FCS_LEN = params.findDouble("MAC_FCS_LEN",DEFAULT_MAC_FCS_LEN);
	MAC_RTS_LEN = params.findDouble("MAC_RTS_LEN",DEFAULT_MAC_RTS_LEN);
	MAC_CTS_LEN = params.findDouble("MAC_CTS_LEN",DEFAULT_MAC_CTS_LEN);
	MAC_ACK_LEN = params.findDouble("MAC_ACK_LEN",DEFAULT_MAC_ACK_LEN);
	DIFS_TIME = params.findDouble("DIFS_TIME", DEFAULT_DIFS_TIME);
	SIFS_TIME = params.findDouble("SIFS_TIME", DEFAULT_SIFS_TIME);
	CW_TIME = params.findDouble("CW_TIME", DEFAULT_CW_TIME);
	PAYLOAD = params.findDouble("PAYLOAD", DEFAULT_PAYLOAD);

	string DEFAULT_SNR_TO_PER_FILE_NAME_(DEFAULT_SNR_TO_PER_FILE_NAME);
	string helper = params.findString("SNR_TO_PER_FILE_NAME",DEFAULT_SNR_TO_PER_FILE_NAME_);
	SNR_TO_PER_FILE_NAME = new char[helper.size()+1];
	strcpy(SNR_TO_PER_FILE_NAME, helper.c_str());

	string DEFAULT_COMMUNICATION_GRAPH_FILENAME_(DEFAULT_COMMUNICATION_GRAPH_FILENAME);
	helper = params.findString("COMMUNICATION_GRAPH_FILENAME",DEFAULT_COMMUNICATION_GRAPH_FILENAME_);
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

GenerationParams::~GenerationParams()
{
	if (SNR_TO_PER_FILE_NAME != NULL)
	{
		delete [] SNR_TO_PER_FILE_NAME;
	}
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