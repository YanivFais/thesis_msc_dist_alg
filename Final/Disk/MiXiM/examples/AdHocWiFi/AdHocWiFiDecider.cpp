/*********************************************************
 * File:        AdHocWiFiDecider.cpp
 *
 * Author:      Yaniv Fais
 *
 *
 ***************************************************************************
 * description: decider whether a package transmission is succesful or not
 **************************************************************************/

#include "AdHocWiFiDecider.h"
#include "DeciderResult80211.h"
#include "AdHocWiFiApplLayer.h"
#include "VideoStreamAppl.h"
#include "AdHocWiFiPhyLayer.h"
#include "AdHocWiFiMac.h"

simtime_t AdHocWiFiDecider::processNewSignal(AirFrame* frame) {
	if(currentSignal.first != 0) {
		debugEV << "Already receiving another AirFrame!" << endl;
		return notAgain;
	}

	// get the receiving power of the Signal at start-time and center frequency
	Signal& signal = frame->getSignal();
	Argument start(DimensionSet::timeFreqDomain);
	start.setTime(signal.getSignalStart());
	start.setArgValue(Dimension::frequency_static(), centerFrequency);

	double recvPower = signal.getReceivingPower()->getValue(start);

	cPacket * msgPhy = frame->getEncapsulatedPacket();
	assert (msgPhy);
	bool RTS = (string(msgPhy->getFullName())==string("wlan-rts")), rtsPower=false;

	if (recvPower<sensitivity) { // not suppose to normally receive
		if ((recvPower > (sensitivity/RtsSensitivityRatio)) && (RTS)) {
			debugEV << simTime() << " Signal is strong RTS (" << recvPower << " > " << sensitivity/RtsSensitivityRatio << ") " << myIndex << endl;
			AHphy->getMac()->delayMsg(3); // TODO - MCS?
			rtsPower=true;
		}
	}

	// check whether signal is strong enough to receive
	if ((recvPower < sensitivity) && !rtsPower)
	{
		if (RTS)
			debugEV << "Signal is too weak RTS (" << recvPower << " < " << sensitivity/RtsSensitivityRatio
					<< ") -> do not receive. " << myIndex << endl;
		else
		   debugEV << "Signal is too weak (" << recvPower << " < " << sensitivity
				<< ") -> do not receive." << endl;
		// Signal too weak, we can't receive it, tell PhyLayer that we don't want it again
		return notAgain;
	}

	// Signal is strong enough, receive this Signal and schedule it
	debugEV << "Signal is strong enough (" << recvPower << " > " << sensitivity
			<< ") -> Trying to receive AirFrame." << endl;

	currentSignal.first = frame;
	currentSignal.second = EXPECT_END;

	//channel turned busy
	setChannelIdleStatus(false);

	return ( signal.getSignalStart() + signal.getSignalLength() );
}

DeciderResult* AdHocWiFiDecider::checkIfSignalOk(Mapping* snrMap, AirFrame* frame)
{
	DeciderResult80211 * dr = dynamic_cast<DeciderResult80211*>(checkIfSignalOkBase(snrMap,frame));

	cPacket * msgPhy = frame->getEncapsulatedPacket();
	assert (msgPhy);
	cPacket * msgMac = msgPhy->getEncapsulatedPacket();
	if (msgMac==NULL) {
		return dr;
	}
	assert (msgMac);
	cPacket * msgApp = msgMac->getEncapsulatedPacket();
	assert(msgApp);
	ApplPkt *msg = dynamic_cast<ApplPkt *>(msgApp);
	assert (msg && msg->getKind() == AdHocWiFiApplLayer::MSG_VID_STREAM);
	
#ifdef DECIDER_DEBUG
	if (myIndex==msg->getDestAddr())
	  DEBUG_OUT << "Decider" << msg->getSrcAddr() << "->" << msg->getDestAddr() << " - " << dr->isSignalCorrect() << " SINR=" << dr->getSnr() << " bitrate=" << dr->getBitrate() << " index=" << myIndex <<  endl;
#endif
	
	if (myIndex==msg->getDestAddr())
	  VideoStreamAppl::addSINR(msg,dr);
	
	return dr;
}

DeciderResult* AdHocWiFiDecider::checkIfSignalOkBase(Mapping* snrMap, AirFrame* frame)
{
	assert(snrMap);

	bool OK = true;

	if (external_PER>0)
		OK = (rand() % 1000)+1 > (external_PER*1000);

	Signal& s = frame->getSignal();
	simtime_t start = s.getSignalStart();
	simtime_t end = start + s.getSignalLength();
	DeciderResult80211* result = 0;

	ConstMappingIterator* bitrateIt = s.getBitrate()->createConstIterator();
	bitrateIt->next(); //iterate to payload bitrate indicator
	double payloadBitrate = bitrateIt->getValue();
	delete bitrateIt;
	double sinrMin;
	if (deciderMode!=MinPayload) {
		start = start + RED_PHY_HEADER_DURATION; //its ok if the phy header is received only
											 //partly - TOD0: maybe solve this nicer (Sorin)
		Argument min(DimensionSet::timeFreqDomain);
		min.setTime(start);
		min.setArgValue(Dimension::frequency_static(), centerFrequency - 11e6);
		Argument max(DimensionSet::timeFreqDomain);
		max.setTime(end);
		max.setArgValue(Dimension::frequency_static(), centerFrequency + 11e6);

		sinrMin = MappingUtils::findMin(*snrMap, min, max);

		EV << " snrMin: " << sinrMin << endl;
		if (sinrMin > snrThreshold) {
			bool berPacket = false;
/*			cPacket * msgPhy = frame->getEncapsulatedMsg();
		    int dest = 0;
		    int source = 0;
			if (msgPhy) {
				cPacket * msgMac = msgPhy->getEncapsulatedMsg();
				if (msgMac) {
					cPacket * msgApp = msgMac->getEncapsulatedMsg();
					if (msgApp) {
						ApplPkt *msg = dynamic_cast<ApplPkt *>(msgApp);
						dest = msg->getDestAddr();
						source = msg->getSrcAddr();
					}
				}
			}*/
			if (deciderMode==Sorin)
				berPacket = packetOkSorin(sinrMin, frame->getBitLength() - (int)PHY_HEADER_LENGTH, payloadBitrate);
			else if (deciderMode==Default)
				berPacket = packetOk(sinrMin, frame->getBitLength() - (int)PHY_HEADER_LENGTH, payloadBitrate);
			if (berPacket) {
				result = new DeciderResult80211(OK, payloadBitrate, sinrMin);
			} else {
				EV << "Packet has BIT ERRORS! It is lost!\n";
				result = new DeciderResult80211(false, payloadBitrate, sinrMin);
			}
		}
	}
	else { // three sections : header and FCS-any bit bad all bad,for payload bad-bits/total-bits < p then ok
		Argument min(DimensionSet::timeFreqDomain);
		min.setTime(start);
		min.setArgValue(Dimension::frequency_static(), centerFrequency - 11e6);
		Argument max(DimensionSet::timeFreqDomain);
		max.setTime(start+RED_PHY_HEADER_DURATION+16/payloadBitrate); // header+service
		max.setArgValue(Dimension::frequency_static(), centerFrequency + 11e6);

		sinrMin = MappingUtils::findMin(*snrMap, min, max);
		if (sinrMin > snrThreshold) { // header good
			min.setTime(end-6/payloadBitrate); // trailer
			max.setTime(end);
			sinrMin = MappingUtils::findMin(*snrMap, min, max);
			if (sinrMin > snrThreshold) { // trailer good
				min.setTime(start+RED_PHY_HEADER_DURATION+16/payloadBitrate); // payload
				max.setTime(end-6/payloadBitrate);
				if (countBelowThBer(*snrMap, min, max,snrThreshold*3,payloadBitrate)<payloadBerThreshold)
					result = new DeciderResult80211(OK, payloadBitrate, sinrMin);
				else
					result = new DeciderResult80211(false, payloadBitrate, sinrMin);
			}
		}
	}

	if (result==NULL) {
		EV << "Packet has ERRORS! It is lost! " << frame->getDisplayString() << " " << frame->getId() << endl;
		result = new DeciderResult80211(false, payloadBitrate, sinrMin);
	}

	return result;
}

double AdHocWiFiDecider::countBelowThBer(ConstMapping& m, const Argument& min, const Argument& max,double th,double payloadBitrate) {

	//the passed interval should define a value for every dimension
	//of the mapping.
	assert(min.getDimensions().isSubSet(m.getDimensionSet()));
	assert(max.getDimensions().isSubSet(m.getDimensionSet()));

	ConstMappingIterator* it = m.createConstIterator(min);

	double total = ((max.getTime() - min.getTime()).dbl());
	double err = 0;
	Argument prev = min;
	while(it->hasNext() && it->getNextPosition().compare(max, m.getDimensionSet()) < 0){
		it->next();
		Argument beg,end;
		const Argument& next = it->getPosition();
		EV << "count:" << min.getTime() <<" - " << max.getTime() << ":" << prev.getTime() << "-" << next.getTime() << endl;

		bool inRange = next.getTime() >= min.getTime() && next.getTime() <= max.getTime();
		if(inRange) {
			for(Argument::const_iterator itA = next.begin(); itA != next.end(); ++itA) {
				if(itA->second < min.getArgValue(itA->first) || itA->second > max.getArgValue(itA->first)) {
					inRange = false;
					break;
				}
			}
		}

		if(inRange) {
			double val = it->getValue();
			if(val < th) {
				//EV << " total is" << total << " err prev = " << err;
				err += (it->getPosition().getTime()-prev.getTime()).dbl();
				//EV << "err after = " << err << endl;
			}
		}
		prev = next;
	}
	it->iterateTo(max);
	double val = it->getValue();
	if(val < th) {
		//EV << " total is" << total << " err prev = " << err;
		err += (max.getTime() - prev.getTime()).dbl();
		//EV << "err after = " << err << endl;
	}
	delete it;
	return err/total;
}



//taken from Sorin code - http://vega.unitbv.ro/~sorin.cocorada/omnetpp/

const unsigned short comb_calc[15][15]=
{
{    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
{    2,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
{    3,    3,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
{    4,    6,    4,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
{    5,   10,   10,    5,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
{    6,   15,   20,   15,    6,    1,    0,    0,    0,    0,    0,    0,    0,    0 },
{    7,   21,   35,   35,   21,    7,    1,    0,    0,    0,    0,    0,    0,    0 },
{    8,   28,   56,   70,   56,   28,    8,    1,    0,    0,    0,    0,    0,    0 },
{    9,   36,   84,  126,  126,   84,   36,    9,    1,    0,    0,    0,    0,    0 },
{   10,   45,  120,  210,  252,  210,  120,   45,   10,    1,    0,    0,    0,    0 },
{   11,   55,  165,  330,  462,  462,  330,  165,   55,   11,    1,    0,    0,    0 },
{   12,   66,  220,  495,  792,  924,  792,  495,  220,   66,   12,    1,    0,    0 },
{   13,   78,  286,  715, 1287, 1716, 1716, 1287,  715,  286,   78,   13,    1,    0 },
{   14,   91,  364, 1001, 2002, 3003, 3432, 3003, 2002, 1001,  364,   91,   14,    1 }
};

/*
unsigned long comb(int n,int k){
    unsigned long long p1=1, p2=1,i;
    for(i=0; i<=k-1; i++)
        p1=p1*(n-i);
    for(i=2; i<=k; i++)
        p2=p2*i;
    return p1/p2;
}
*/

unsigned short comb(int n,int k)
{
//    ASSERT(n>=1 && n<=14 k>=1 && k<=14);
    return comb_calc[n-1][k-1];
}



double Pd(int d, double Pb){

	long double sum=0.0;
	int k;
	if(d%2 == 1){
		for(k=(d+1)/2; k<=d; k++)
			sum+=comb(d,k)*pow(Pb,k)*pow(1.0-Pb, d-k);
	}
	else{
		sum=0.5*comb(d,d/2)*pow(Pb, d/2)*pow(1.0-Pb,d/2);
		for(k=d/2+1; k<=d; k++)
			sum+=comb(d,k)*pow(Pb,k)*pow(1.0-Pb, d-k);
	}
	//printf("prob=%f d=%d sum=%f \n",Pb,d, sum);
	return sum;

	//return pow(4*Pb*(1-Pb), d/2);
}


double Pb(int rate, double prob){

	long double probc;
	switch(rate){
	case 1:
		//11, 0, 38, 0, 193, 0, 1331, 0, 7275, 0, 40406, 0, 234969, 0, 1337714, 0, 7594819, 0, 433775588, 0,
		probc=11*Pd(10,prob) + 38*Pd(12,prob) + 193*Pd(14,prob);//+1331*Pd(16,prob);
		break;
	case 2:
		//1, 16, 48, 158, 642, 2435, 9174, 34701, 131533, 499312,
		probc=Pd(6,prob)+16*Pd(7,prob)+48*Pd(8,prob);//+158*Pd(9,prob)+642*Pd(10,prob)
//				+2435*Pd(11,prob)+ 9174*Pd(12,prob)+34701*Pd(13,prob)+131533*Pd(14,prob)+499312*Pd(15,prob);
		break;
	case 3:
		//(8, 31, 160, 892, 4512, 23297, 120976, 624304, 3229885, 16721329,
		probc=8*Pd(5,prob)+31*Pd(6,prob)+150*Pd(7,prob);//+892*Pd(8,prob)+4512*Pd(9,prob)
//			+23297*Pd(10,prob)+120976*Pd(11,prob)+624304*Pd(12,prob)+ 3229885*Pd(13,prob)+ 16721329*Pd(14,prob);
		break;
	default:
	;
	}

	return probc;
}


double ber_bpsk(double sinr, double bandwidth, double bitrate, char channelModel){
	double y=sinr*bandwidth/bitrate;
	if(channelModel=='r')//Rayleigh
		return 0.5*(1.0-sqrt(y/(1.0+y)));
	else
    	return 0.5*erfc(sqrt(y));//awgn
}

double ber_qpsk(double sinr, double bandwidth, double bitrate, char channelModel){
	double y=sinr*bandwidth/bitrate;
	if(channelModel=='r')//Rayleigh
		return 0.5*(1.0-sqrt(y/(1.0+y)));
	else
    	return 0.5*erfc(sqrt(y));//awgn
}

double ber_16qam(double sinr, double bandwidth, double bitrate, char channelModel){
	double y=sinr*bandwidth/bitrate;
	if(channelModel=='r')//Rayleigh
		return ( 5.0/8.0-3.0/8.0*sqrt(2.0*y/(5.0+2.0*y))-1.0/4.0*sqrt(18.0*y/(5.0+18.0*y)) );
	else
    	return ( 0.375*erfc(sqrt(0.4*y))+0.25*erfc(3.0*sqrt(0.4*y)) );//awgn
    }

double ber_64qam(double sinr, double bandwidth, double bitrate, char channelModel){
	double y=sinr*bandwidth/bitrate;
	if(channelModel=='r')//Rayleigh
		return ( 13.0/24.0-7.0/24.0*sqrt(y/(7.0+y))-1.0/4.0*sqrt(9.0*y/(7.0+9.0*y)) );
	else
    	return 7.0/24.0*erfc(sqrt(y/7.0))+0.25*erfc(3.0*sqrt(y/7.0));//awgn
    }



double AdHocWiFiDecider::calculateDuration(AirFrame *airframe,double bitrate)
{
    double duration;

    if(phyOpMode=='g')
	duration=4*ceil((16+airframe->getBitLength()+6)/(bitrate/1e6*4))*1e-6 + 26e-6;
    else
    // The physical layer header is sent with 1Mbit/s and the rest with the frame's bitrate
        duration=airframe->getBitLength()/bitrate + 192/BITRATE_HEADER;
    debugEV<<"Radio:frameDuration="<<duration*1e6<<"us("<<airframe->getBitLength()<<"bits)"<<endl;
    return duration;
}

bool AdHocWiFiDecider::packetOkSorin(double sinrMin, int lengthMPDU, double bitrate)
{
    double berHeader, berMPDU;

    berHeader = 0.5 * exp(-sinrMin * BANDWIDTH / BITRATE_HEADER);

    // if PSK modulation
    if (bitrate == 1E+6 || bitrate == 2E+6)
        berMPDU = 0.5 * exp(-sinrMin * BANDWIDTH / bitrate);
    // if CCK modulation (modeled with 16-QAM)
    else if (bitrate == 5.5E+6)
        berMPDU = 0.5 * (1 - 1 / sqrt(pow(2.0, 4))) * erfc(sinrMin * BANDWIDTH / bitrate);
    else if(bitrate == 11E+6)                        // CCK, modelled with 256-QAM
        berMPDU = 0.25 * (1 - 1 / sqrt(pow(2.0, 8))) * erfc(sinrMin * BANDWIDTH / bitrate);

	else{//802.11g rates
	  if (lengthMPDU<=160)
	    bitrate = 6E6;
	  //PLCP Header 24bits, BPSK, r=1/2, 6Mbps
	  berHeader=ber_bpsk(sinrMin, BANDWIDTH , 6E+6, channelModel);
	  berHeader=Pb(1, berHeader);
	  
	  
	  switch((int)bitrate){
		case (int)(6E+6)://6Mbps, r=1/2, BPSK
			berMPDU=ber_bpsk(sinrMin, BANDWIDTH , bitrate, channelModel);
			berMPDU=Pb(1, berMPDU);
		break;
		case (int)(9E+6)://9Mbps, r=3/4, BPSK
			berMPDU=ber_bpsk(sinrMin, BANDWIDTH , bitrate, channelModel);
			berMPDU=Pb(3, berMPDU);
		break;

		case (int)(12E+6)://12Mbps, r=1/2, QPSK
			berMPDU=ber_qpsk(sinrMin, BANDWIDTH , bitrate, channelModel);
			berMPDU=Pb(1, berMPDU);
		break;
		case (int)(18E+6)://18Mbps, r=3/4, QPSK
			berMPDU=ber_qpsk(sinrMin, BANDWIDTH , bitrate, channelModel);
			berMPDU=Pb(3, berMPDU);
		break;

		case (int)(24E+6)://24Mbps, r=1/2, 16QAM
			berMPDU=ber_16qam(sinrMin, BANDWIDTH , bitrate, channelModel);
			berMPDU=Pb(1, berMPDU);
		break;
		case (int)(36E+6)://36Mbps, r=3/4, 16QAM
			berMPDU=ber_16qam(sinrMin, BANDWIDTH , bitrate, channelModel);
			berMPDU=Pb(3, berMPDU);
		break;

		case (int)(48E+6)://48Mbps, r=2/3, 64QAM
			berMPDU=ber_64qam(sinrMin, BANDWIDTH , bitrate, channelModel);
			berMPDU=Pb(2, berMPDU);
		break;

		case (int)(54E+6)://54Mbps, r=3/4, 64QAM
			berMPDU=ber_64qam(sinrMin, BANDWIDTH , bitrate, channelModel);
			berMPDU=Pb(3, berMPDU);
		break;
		default:
			berMPDU=0;
		}


	}

	if((berHeader > 1.0 || berMPDU > 1.0))
	    return false;// error in MPDU
    // probability of no bit error in the PLCP header
	double headerNoError;
	if (phyOpMode=='g')
		headerNoError = pow(1.0 - berHeader, 24);//PLCP Header 24bit(without SERVICE), 6Mbps
	else
    	headerNoError = pow(1.0 - berHeader, HEADER_WITHOUT_PREAMBLE);

    // probability of no bit error in the MPDU
    double MpduNoError = pow(1.0 - berMPDU, lengthMPDU);

    double rand = dblrand();

    if (rand > headerNoError)
        return false; // error in header
    else if (dblrand() > MpduNoError)
        return false;  // error in MPDU
    else
        return true; // no error
}

double AdHocWiFiDecider::dB2fraction(double dB)
{
    return pow(10.0, (dB / 10));
}

