#pragma once

class SchedulingParams
{
public:
	SchedulingParams(const char * configFileName);
	~SchedulingParams(void);

	bool   mIsValid;

	char * COMMUNICATION_GRAPH_FILENAME;
	char * INTERFERENCE_GRAPH_FILENAME;
	char * COORDS_FILENAME;
	char * REQUESTS_FILENAME;

	int    N_FREQS_FOR_SCHEDULE;

private:
	static const char * DEFAULT_COMMUNICATION_GRAPH_FILENAME;
	static const char * DEFAULT_INTERFERENCE_GRAPH_FILENAME;
	static const char * DEFAULT_COORDS_FILENAME;
	static const char * DEFAULT_REQUESTS_FILENAME;

	static const int    DEFAULT_N_FREQS_FOR_SCHEDULE;
};
