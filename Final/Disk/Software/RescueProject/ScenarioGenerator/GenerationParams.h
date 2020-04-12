#pragma once

class GenerationParams
{
public:
	GenerationParams(const char * configFileName);
	~GenerationParams();

	int    SEED_FOR_GENERATOR;
	double CLUSTER_RADIUS;
	int    N_CLIENTS;
	int    TIME_PERIOD;
	int    TIME_RESOLUTION;
	int    N_NEAR_REQUEST;
	int    N_FAR_REQUEST;
	double FAR_THRESHOLD;
	int    SCALE_IN_METERS;
	double MEAN_VELOCITY;
	double STD_VELOCITY;
	double MAX_VELOCITY;
	double TRANSMISSION_POWER_IN_MILI_WATTS;
	int    PATHLOSS_MODEL;
	double PATHLOSS_ALPHA;
	double PATHLOSS_BETA;
	double PATHLOSS_ELBIT_C;
	double PATHLOSS_ELBIT_H_TR;
	double PATHLOSS_ELBIT_H_RC;

	double NOISE_IN_DBM;
	char * SNR_TO_PER_FILE_NAME;
	double D_BREAKPOINT;
	double SF_BEFORE_BREAKPOINT;
	double SF_AFTER_BREAKPOINT;
	double NUM_STD_FOR_DECAY;
	double FREQUENCY;

	double MIN_SNR_FOR_COMMUNICATION;
	double MIN_SNR_FOR_INTERFERENCE;

	double HEADER_TIME;
	double PREAMBLE_TIME;
	double MAC_SERVICE_TRAILER_TIME;
	double MAC_HEADER_LEN;
	double MAC_FCS_LEN; 
	double MAC_RTS_LEN; 
	double MAC_CTS_LEN; 
	double MAC_ACK_LEN; 
	double DIFS_TIME;
	double SIFS_TIME;
	double CW_TIME;
	double PAYLOAD;

	bool   mIsValid;

	char * COMMUNICATION_GRAPH_FILENAME;
	char * INTERFERENCE_GRAPH_FILENAME;
	char * COORDS_FILENAME;
	char * REQUESTS_FILENAME;

	double DEMAND_SIZE;

private:
	static const int DEFAULT_SEED_FOR_GENERATOR;
	static const double DEFAULT_CLUSTER_RADIUS;

	static const int DEFAULT_TIME_PERIOD;
	static const int DEFAULT_TIME_RESOLUTION;
// number of clients
	static const int DEFAULT_N_CLIENTS;
// number of requests from each type
	static const int DEFAULT_N_NEAR_REQUEST;
	static const int DEFAULT_N_FAR_REQUEST;
// definition of when a request is considered far
	static const double DEFAULT_FAR_THRESHOLD;
// 30 km square - the program select points in the unit square,
// and then scales them up
	static const int DEFAULT_SCALE_IN_METERS;

// dynamic model (velocities are normalized to unit square)
// so 0.2 for a square of 30km means 6 km / h
	static const double DEFAULT_MEAN_VELOCITY;  
	static const double DEFAULT_STD_VELOCITY;
	static const double DEFAULT_MAX_VELOCITY;

	// sensitivity parameters in db
	static const double DEFAULT_MIN_SNR_FOR_COMMUNICATION;
	static const double DEFAULT_MIN_SNR_FOR_INTERFERENCE;

// communication parameters
	static const double DEFAULT_TRANSMISSION_POWER_IN_MILI_WATTS;
	static const int    DEFAULT_PATHLOSS_MODEL;
	static const double DEFAULT_PATHLOSS_ALPHA;
	static const double DEFAULT_PATHLOSS_BETA;
	static const double DEFAULT_PATHLOSS_ELBIT_C;
	static const double DEFAULT_PATHLOSS_ELBIT_H_TR;
	static const double DEFAULT_PATHLOSS_ELBIT_H_RC;


	static const double DEFAULT_NOISE_IN_DBM;
	static const char * DEFAULT_SNR_TO_PER_FILE_NAME;
// In meters (different model before and after)
	static const double DEFAULT_D_BREAKPOINT;
// STD in DB
	static const double DEFAULT_SF_BEFORE_BREAKPOINT;
	static const double DEFAULT_SF_AFTER_BREAKPOINT;
// Arbitrary - to get yes no for and edge instead of distribution
	static const double DEFAULT_NUM_STD_FOR_DECAY;
// in GHZ
	static const double DEFAULT_FREQUENCY;
// in micro-second
	static const double DEFAULT_HEADER_TIME;
	static const double DEFAULT_PREAMBLE_TIME;
	static const double DEFAULT_MAC_SERVICE_TRAILER_TIME; 
	static const double DEFAULT_MAC_HEADER_LEN; 
	static const double DEFAULT_MAC_FCS_LEN; 
	static const double DEFAULT_MAC_RTS_LEN; 
	static const double DEFAULT_MAC_CTS_LEN; 
	static const double DEFAULT_MAC_ACK_LEN; 
	static const double DEFAULT_DIFS_TIME;
	static const double DEFAULT_SIFS_TIME;
	static const double DEFAULT_CW_TIME;
// in mega bit per second
	static const double DEFAULT_DEMAND_SIZE;
// in kilo-byte
	static const double DEFAULT_PAYLOAD;

// names used for output
	static const char * DEFAULT_COMMUNICATION_GRAPH_FILENAME;
	static const char * DEFAULT_INTERFERENCE_GRAPH_FILENAME;
	static const char * DEFAULT_COORDS_FILENAME;
	static const char * DEFAULT_REQUESTS_FILENAME;
};
