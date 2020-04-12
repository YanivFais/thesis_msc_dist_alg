#
# OMNeT++/OMNEST Makefile for MiXiM
#
# This file was generated with the command:
#  opp_makemake -f -O out -I../../modules/phy -I../../modules/messages -I../../base/modules -I../../base/messages -I../../base/phyLayer -I../../modules/analogueModel -I../../modules/mac -I../../base/connectionManager -I../../base/utils -I../../modules/utility -L../../out/gcc-debug/tests/testUtils -L../../out/gcc-debug/base -L../../out/gcc-debug/tests/power/utils -L../../out/gcc-debug/modules -lmiximbase -lmiximmodules
#

# Name of target to be created (-o option)
TARGET = MiXiM$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../modules/phy \
    -I../../modules/messages \
    -I../../base/modules \
    -I../../base/messages \
    -I../../base/phyLayer \
    -I../../modules/analogueModel \
    -I../../modules/mac \
    -I../../base/connectionManager \
    -I../../base/utils \
    -I../../modules/utility \
    -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../out/gcc-debug/tests/testUtils -L../../out/gcc-debug/base -L../../out/gcc-debug/tests/power/utils -L../../out/gcc-debug/modules  -lmiximbase -lmiximmodules
LIBS += -Wl,-rpath,`abspath ../../out/gcc-debug/tests/testUtils` -Wl,-rpath,`abspath ../../out/gcc-debug/base` -Wl,-rpath,`abspath ../../out/gcc-debug/tests/power/utils` -Wl,-rpath,`abspath ../../out/gcc-debug/modules`

# Output directory
PROJECT_OUTPUT_DIR = ../../out
PROJECTRELATIVE_PATH = /examples/AdHocWiFi
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cpp and .msg files
OBJS = \
    $O/AdHocWiFiApplLayer.o \
    $O/AdHocWiFiConnectionManager.o \
    $O/AdHocWiFiDecider.o \
    $O/AdHocWiFiMac.o \
    $O/AdHocWiFiMobility.o \
    $O/AdHocWiFiPathLossModel.o \
    $O/AdHocWiFiPhyLayer.o \
    $O/CommunicationRequest.o \
    $O/Mac80211g.o \
    $O/PER2SNRTable.o \
    $O/PER_FOR_MCS.o \
    $O/rd.o \
    $O/RequestsCollection.o \
    $O/RtpData.o \
    $O/ScheduleItem.o \
    $O/ScheduleItemCollection.o \
    $O/VideoStreamAppl.o

# Message files
MSGFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY:

.SUFFIXES: .cpp

$O/%.o: %.cpp
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cpp %_m.h: %.msg
	$(MSGC) -s _m.cpp $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f MiXiM MiXiM.exe libMiXiM.so libMiXiM.a libMiXiM.dll libMiXiM.dylib
	-rm -f ./*_m.cpp ./*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cpp

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/AdHocWiFiApplLayer.o: AdHocWiFiApplLayer.cpp \
  ./RequestsCollection.h \
  ../../base/phyLayer/ChannelState.h \
  ../../modules/phy/Decider80211.h \
  ./ScheduleItemCollection.h \
  ../../base/connectionManager/ConnectionManager.h \
  ../../base/modules/BaseUtility.h \
  ../../base/modules/BaseArp.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/HostState.h \
  ../../base/messages/ApplPkt_m.h \
  ./CommunicationRequest.h \
  ../../base/messages/MacPkt_m.h \
  ./AdHocWiFiConnectionManager.h \
  ../../base/modules/BaseLayer.h \
  ./ScheduleItem.h \
  ./types.h \
  ./PER_FOR_MCS.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../modules/mac/Mac80211.h \
  ./RtpData.h \
  ./VideoStreamAppl.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/SimpleAddress.h \
  ../../base/phyLayer/BaseDecider.h \
  ./AdHocWiFiMac.h \
  ../../base/utils/Coord.h \
  ../../base/utils/NetwControlInfo.h \
  ../../base/modules/BaseMacLayer.h \
  ./PER2SNRTable.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../base/modules/BaseModule.h \
  ../../modules/utility/Consts80211.h \
  ./AdHocWiFi.h \
  ../../modules/phy/DeciderResult80211.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/utils/Move.h \
  ../../base/phyLayer/Decider.h \
  ../../base/utils/ImNotifiable.h \
  ./Mac80211g.h \
  ./AdHocWiFiApplLayer.h \
  ./AdHocWiFiDecider.h \
  ../../base/modules/BaseApplLayer.h \
  ../../base/utils/PassedMessage.h \
  ./rtpdump.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/modules/BaseBattery.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h
$O/AdHocWiFiConnectionManager.o: AdHocWiFiConnectionManager.cpp \
  ../../base/connectionManager/ConnectionManager.h \
  ./AdHocWiFiConnectionManager.h \
  ../../base/utils/Coord.h \
  ./AdHocWiFi.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/utils/FWMath.h
$O/AdHocWiFiDecider.o: AdHocWiFiDecider.cpp \
  ./RequestsCollection.h \
  ../../base/phyLayer/ChannelState.h \
  ../../modules/phy/Decider80211.h \
  ./ScheduleItemCollection.h \
  ../../base/modules/BaseUtility.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/HostState.h \
  ../../base/messages/ApplPkt_m.h \
  ./CommunicationRequest.h \
  ../../base/modules/BaseLayer.h \
  ./ScheduleItem.h \
  ./types.h \
  ./RtpData.h \
  ./VideoStreamAppl.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/phyLayer/BaseDecider.h \
  ../../base/utils/Coord.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/modules/BaseModule.h \
  ../../modules/utility/Consts80211.h \
  ./AdHocWiFi.h \
  ../../modules/phy/DeciderResult80211.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/utils/Move.h \
  ../../base/phyLayer/Decider.h \
  ../../base/utils/ImNotifiable.h \
  ./AdHocWiFiApplLayer.h \
  ./AdHocWiFiDecider.h \
  ../../base/modules/BaseApplLayer.h \
  ../../base/utils/PassedMessage.h \
  ./rtpdump.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h
$O/AdHocWiFiMac.o: AdHocWiFiMac.cpp \
  ./RequestsCollection.h \
  ../../base/phyLayer/ChannelState.h \
  ../../modules/phy/Decider80211.h \
  ./ScheduleItemCollection.h \
  ../../base/modules/BaseUtility.h \
  ../../base/modules/BaseArp.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/HostState.h \
  ../../base/messages/ApplPkt_m.h \
  ./CommunicationRequest.h \
  ../../base/messages/MacPkt_m.h \
  ./ScheduleItem.h \
  ../../base/modules/BaseLayer.h \
  ./PER_FOR_MCS.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../modules/mac/Mac80211.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/SimpleAddress.h \
  ../../base/phyLayer/BaseDecider.h \
  ./AdHocWiFiMac.h \
  ../../base/utils/Coord.h \
  ../../base/modules/BaseMacLayer.h \
  ../../base/utils/NetwControlInfo.h \
  ./PER2SNRTable.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../modules/utility/Consts80211.h \
  ../../base/modules/BaseModule.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ./AdHocWiFi.h \
  ../../modules/phy/DeciderResult80211.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/MacToPhyControlInfo.h \
  ../../base/utils/Move.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/phyLayer/Decider.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/phyLayer/PhyToMacControlInfo.h \
  ./Mac80211g.h \
  ./AdHocWiFiApplLayer.h \
  ../../base/modules/BaseApplLayer.h \
  ../../base/utils/PassedMessage.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h
$O/AdHocWiFiMobility.o: AdHocWiFiMobility.cpp \
  ../../base/modules/BaseWorldUtility.h \
  ../../base/modules/BaseUtility.h \
  ../../base/utils/HostState.h \
  ../../base/modules/BaseMobility.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/Coord.h \
  ../../base/modules/BaseModule.h \
  ./AdHocWiFi.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/utils/FWMath.h \
  ./AdHocWiFiMobility.h
$O/AdHocWiFiPathLossModel.o: AdHocWiFiPathLossModel.cpp \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseUtility.h \
  ../../base/modules/BaseArp.h \
  ../../base/utils/HostState.h \
  ../../base/messages/MacPkt_m.h \
  ../../base/modules/BaseLayer.h \
  ./PER_FOR_MCS.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../modules/mac/Mac80211.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ./AdHocWiFiMac.h \
  ../../base/utils/Coord.h \
  ../../base/modules/BaseMacLayer.h \
  ./AdHocWiFiPathLossModel.h \
  ./PER2SNRTable.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../modules/utility/Consts80211.h \
  ../../base/modules/BaseModule.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ./Mac80211g.h \
  ../../base/utils/PassedMessage.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h
$O/AdHocWiFiPhyLayer.o: AdHocWiFiPhyLayer.cpp \
  ./RequestsCollection.h \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseWorldUtility.h \
  ../../modules/phy/Decider80211.h \
  ./ScheduleItemCollection.h \
  ../../base/modules/BaseUtility.h \
  ../../base/modules/BaseArp.h \
  ../../base/utils/HostState.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/messages/ApplPkt_m.h \
  ../../modules/phy/PhyLayer.h \
  ../../base/connectionManager/ChannelAccess.h \
  ../../base/phyLayer/ChannelInfo.h \
  ./CommunicationRequest.h \
  ../../base/messages/MacPkt_m.h \
  ../../base/modules/BaseLayer.h \
  ./ScheduleItem.h \
  ./PER_FOR_MCS.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../modules/mac/Mac80211.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../base/phyLayer/BasePhyLayer.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/phyLayer/BaseDecider.h \
  ./AdHocWiFiMac.h \
  ../../base/utils/Coord.h \
  ../../base/modules/BaseMacLayer.h \
  ./AdHocWiFiPathLossModel.h \
  ./PER2SNRTable.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../modules/utility/Consts80211.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../base/modules/BaseModule.h \
  ./AdHocWiFi.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/utils/Move.h \
  ../../base/phyLayer/Decider.h \
  ../../base/utils/ImNotifiable.h \
  ./Mac80211g.h \
  ./AdHocWiFiApplLayer.h \
  ./AdHocWiFiDecider.h \
  ../../base/modules/BaseApplLayer.h \
  ./AdHocWiFiPhyLayer.h \
  ../../base/utils/PassedMessage.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h
$O/CommunicationRequest.o: CommunicationRequest.cpp \
  ./CommunicationRequest.h
$O/Mac80211g.o: Mac80211g.cpp \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseUtility.h \
  ../../base/modules/BaseArp.h \
  ../../base/utils/HostState.h \
  ../../base/messages/MacPkt_m.h \
  ../../base/modules/BaseLayer.h \
  ./PER_FOR_MCS.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../modules/mac/Mac80211.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/Coord.h \
  ../../base/modules/BaseMacLayer.h \
  ./PER2SNRTable.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../base/modules/BaseModule.h \
  ../../modules/utility/Consts80211.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ./Mac80211g.h \
  ../../base/utils/PassedMessage.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h
$O/PER2SNRTable.o: PER2SNRTable.cpp \
  ./PER_FOR_MCS.h \
  ./PER2SNRTable.h
$O/PER_FOR_MCS.o: PER_FOR_MCS.cpp \
  ./PER_FOR_MCS.h
$O/RequestsCollection.o: RequestsCollection.cpp \
  ./RequestsCollection.h \
  ./CommunicationRequest.h
$O/RtpData.o: RtpData.cpp \
  ./types.h \
  ./RtpData.h \
  ./rtp.h \
  ./ansi.h \
  ./rtpdump.h
$O/ScheduleItem.o: ScheduleItem.cpp \
  ./ScheduleItem.h
$O/ScheduleItemCollection.o: ScheduleItemCollection.cpp \
  ./ScheduleItemCollection.h \
  ./ScheduleItem.h
$O/VideoStreamAppl.o: VideoStreamAppl.cpp \
  ./RequestsCollection.h \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseWorldUtility.h \
  ../../modules/phy/Decider80211.h \
  ./ScheduleItemCollection.h \
  ../../base/modules/BaseUtility.h \
  ../../base/modules/BaseArp.h \
  ../../base/utils/HostState.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/messages/ApplPkt_m.h \
  ./CommunicationRequest.h \
  ../../base/messages/MacPkt_m.h \
  ./ScheduleItem.h \
  ../../base/modules/BaseLayer.h \
  ./PER_FOR_MCS.h \
  ./types.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../modules/mac/Mac80211.h \
  ./RtpData.h \
  ./VideoStreamAppl.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../base/modules/BaseMobility.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/SimpleAddress.h \
  ../../base/phyLayer/BaseDecider.h \
  ./AdHocWiFiMac.h \
  ../../base/utils/Coord.h \
  ../../base/modules/BaseMacLayer.h \
  ../../base/utils/NetwControlInfo.h \
  ./PER2SNRTable.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../modules/utility/Consts80211.h \
  ../../base/modules/BaseModule.h \
  ./AdHocWiFi.h \
  ../../modules/phy/DeciderResult80211.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/utils/Move.h \
  ../../base/phyLayer/Decider.h \
  ../../base/utils/ImNotifiable.h \
  ./Mac80211g.h \
  ./AdHocWiFiApplLayer.h \
  ./AdHocWiFiDecider.h \
  ../../base/modules/BaseApplLayer.h \
  ../../base/utils/PassedMessage.h \
  ./rtpdump.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h \
  ./AdHocWiFiMobility.h
$O/rd.o: rd.cpp \
  ./types.h \
  ./rtpdump.h
