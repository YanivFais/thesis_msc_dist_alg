// gen.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include "GenerationParams.h"
//#include "PER2SNRTable.h"
//#include "RateCalculator.h"
//#include "GraphGenerator.h"
#include "RequestsCollection.h"
#include "CommunicationGraph.h"
#include "InterferenceGraph.h"
#include "OmnetGraph.h"
#include "ScheduleItemCollection.h"
#include <assert.h>
#include "ConfigReader.h"

#ifndef __GNUC__
  #include <direct.h>
#else
  #include <unistd.h>
#endif
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#ifndef PATH_MAX
#define PATH_MAX 256
#endif

using namespace std;

string get_file_name(const string& in,const string& dir)
{
	string rval;
	if (((in.find('\\')==string::npos) && (in.find('/')==string::npos) && (in.find("..")==string::npos))) {
		rval = dir + in;
	}
	else if (in.length()>1 && in[1]==':') {
	  rval = dir + in.substr(in.rfind('\\')+1);
	}
	else rval = in;
	string::size_type pos = rval.find("\\");
	while (pos!=string::npos) {
		rval.replace(pos,1,"\\\\");
		pos = rval.find("\\",pos+2);
	}
	return rval;
}

string removePrefix(const string& str,const string& prefix)
{
  string s = str;
  if (s.find(prefix)==0)
    s = s.substr(prefix.length());
  return s;
}

int main(int argc, char * argv[])
{
  if ((argc != 2) && (argc!=3) && (argc!=4))
    {
      cout << "Wrong number of parameters" << endl;
      cout << "ScenarioGenerator <config file name> [base directory] [relative path]" << endl;
      return 1;
    }
  
  string prefix = (argc==4) ? argv[3] : "";
    
  // read configuration file
  GenerationParams params(string(prefix+argv[1]).c_str());
  if (params.mIsValid == false)
    return 1;
  

  ConfigReader rawParams(prefix+argv[1]);
  if (rawParams.hasFoundFile() == false)
    {
      cout << "Problem reading the scenario parameters file " << prefix+argv[1] << endl;
      return 2;
    }
  
  string dir;
  char cwd[PATH_MAX];
  getcwd(cwd,PATH_MAX);
  if (argc>=3) {
    dir = prefix+argv[2];
  }
  else dir = cwd;

	bool mapNodes = false;
	string scheduleFileName = get_file_name(rawParams.findString("SCHEDULE_FILENAME","schedule.txt"),dir);
	ScheduleItemCollection schedule;
	cout << "===IniGen 5.1===:Reading from schedule file " << scheduleFileName << endl;
	schedule.readFromFile(scheduleFileName);
	OmnetGraph communicationGraph;
	string coordsFileName = get_file_name(params.COORDS_FILENAME,dir).c_str();
	if (!communicationGraph.InitFromFiles(get_file_name(params.COMMUNICATION_GRAPH_FILENAME,dir).c_str(),
					 coordsFileName.c_str()))
		return 1;
	ostringstream str,mapping;
	str << "[General]\n";
	str << "sim.channel*node[*].appl.headerLength = " << params.PAYLOAD*8*1000 << "bit\n"; // kbytes->bits
	str << "sim.channel*node[*].nic.phy.pathloss_alpha = " << params.PATHLOSS_ALPHA << endl;
	str << "sim.channel*node[*].nic.phy.pathloss_beta = " << params.PATHLOSS_BETA << endl;
	str << "sim.channel*node[*].nic.phy.pathloss_c = " << params.PATHLOSS_ELBIT_C << endl;
	str << "sim.channel*node[*].nic.phy.pathloss_htr = " << params.PATHLOSS_ELBIT_H_TR << endl;
	str << "sim.channel*node[*].nic.phy.pathloss_hrc = " << params.PATHLOSS_ELBIT_H_RC << endl;
	string pathloss_model =  (params.PATHLOSS_MODEL==2) ? "\"Elbit\"" : "\"AlphaBeta\"";
	str << "sim.channel*node[*].nic.phy.pathloss_model = " << pathloss_model << endl;

	str << "sim.channel*.alpha = " << params.PATHLOSS_ALPHA/10 << endl;
	str << "**.phy.thermalNoise = " << params.NOISE_IN_DBM << "dBm\n";
	str << "**.mac.txPower = " << params.TRANSMISSION_POWER_IN_MILI_WATTS << "mW" << endl;
	int n_time_slots = rawParams.findInt("N_TIME_SLOTS",1000);

	str << "sim.channel*node[*].nic.mac.per2snr_filename = \"" << params.SNR_TO_PER_FILE_NAME << "\"" << endl;

	str << "sim.channel*node[*].appl.slots = " << n_time_slots << endl;

	double slotTime = (double(params.TIME_RESOLUTION))/n_time_slots;
	str << "sim.channel*node[*].appl.slotMaxTime = " << slotTime << "s" << endl;
	str << "sim.channel*node[*].mobility.updateInterval = 1s" << endl;
	str << "sim.channel*node[*].appl.schedule_file = \"" << removePrefix(scheduleFileName,prefix) << "\""  << endl;
	string requestsFile = get_file_name(params.REQUESTS_FILENAME,dir);
	str << "sim.channel*node[*].appl.requests_file = \"" << removePrefix(requestsFile,prefix) << "\"" << endl;
	int channels = rawParams.findInt("N_FREQS_FOR_SCHEDULE",3);
	str << "sim.channel*node[*].appl.channels = " << channels << endl;

	int min_snr_comm = params.MIN_SNR_FOR_COMMUNICATION;
	str << "**.mac.min_snr_comm = " << min_snr_comm << endl;
	int min_snr_intrf = params.MIN_SNR_FOR_INTERFERENCE;
	str << "**.mac.min_snr_intrf = " << min_snr_intrf << endl;
	str << "**.phy.sensitivity = " << min_snr_intrf-params.TRANSMISSION_POWER_IN_MILI_WATTS << "dBm" << endl;

	str << "sim.channel*node[*].mobility.coords_file = \"" << removePrefix(coordsFileName,prefix) << "\"" << endl;
	int time_period = rawParams.findInt("TIME_PERIOD",30);
	str << "sim.channel*node[*].mobility.coords_number = " << time_period << "" << endl;
	str << "sim.channel*node[*].appl.rounds = " << time_period << endl;

	int scale = params.SCALE_IN_METERS;
	str << "sim.playgroundSizeX = " << scale  << "m\n";
	str << "sim.playgroundSizeY = " << scale  << "m\n";
	str << endl << endl;
	communicationGraph.writeOmnetppIni(scale,str,schedule,mapping,mapNodes);

	cout << "Creating omnetpp.ini.include" << endl;
	ofstream f("omnetpp.ini.include");
	f << str.str();
	f.close();

	if (mapNodes) {
		ofstream m("mapping.txt");
		m << mapping.str();
		m.close();
	}

	return 0;
}




