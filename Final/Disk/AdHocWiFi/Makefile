#
# OMNeT++/OMNEST Makefile for MiXiM
#
# This file was generated with the command:
#  opp_makemake -f -O out -I../../modules/phy -I../../modules/messages -I../../base/modules -I../../base/messages -I../../base/phyLayer -I../../modules/analogueModel -I../../modules/mac -I../../base/connectionManager -I../../base/utils -I../../modules/utility -L../../out/gcc-release/tests/testUtils -L../../out/gcc-release/base -L../../out/gcc-debug/tests/power/utils -L../../out/gcc-release/modules -lmiximbase -lmiximmodules
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
LIBS = -L../../out/gcc-release/tests/testUtils -L../../out/gcc-release/base -L../../out/gcc-debug/tests/power/utils -L../../out/gcc-release/modules  -lmiximbase -lmiximmodules
LIBS += -Wl,-rpath,`abspath ../../out/gcc-release/tests/testUtils` -Wl,-rpath,`abspath ../../out/gcc-release/base` -Wl,-rpath,`abspath ../../out/gcc-debug/tests/power/utils` -Wl,-rpath,`abspath ../../out/gcc-release/modules`

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
  ./AdHocWiFi.h \
  ../../base/utils/HostState.h \
  ./CommunicationRequest.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../modules/phy/DeciderResult80211.h \
  ./Mac80211g.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/messages/MacPkt_m.h \
  ../../modules/phy/Decider80211.h \
  ./AdHocWiFiConnectionManager.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../base/connectionManager/NicEntry.h \
  ./types.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/connectionManager/ConnectionManager.h \
  ../../modules/mac/Mac80211.h \
  ../../base/modules/BaseMacLayer.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/Move.h \
  ./ScheduleItem.h \
  ./AdHocWiFiMac.h \
  ./AdHocWiFiDecider.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/ImNotifiable.h \
  ./VideoStreamAppl.h \
  ./AdHocWiFiApplLayer.h \
  ../../base/utils/SimpleAddress.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ./RequestsCollection.h \
  ../../base/phyLayer/MappingBase.h \
  ./ScheduleItemCollection.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h \
  ../../base/phyLayer/Mapping.h \
  ./rtpdump.h \
  ../../base/messages/ApplPkt_m.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/modules/BaseArp.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/utils/PassedMessage.h \
  ./RtpData.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/utils/NetwControlInfo.h \
  ../../base/modules/BaseApplLayer.h \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BaseModule.h \
  ../../base/phyLayer/Decider.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../modules/utility/Consts80211.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/phyLayer/BaseDecider.h \
  ../../base/modules/BaseLayer.h
$O/AdHocWiFiConnectionManager.o: AdHocWiFiConnectionManager.cpp \
  ./AdHocWiFi.h \
  ./AdHocWiFiConnectionManager.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/connectionManager/ConnectionManager.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h \
  ../../base/connectionManager/BaseConnectionManager.h
$O/AdHocWiFiDecider.o: AdHocWiFiDecider.cpp \
  ./AdHocWiFi.h \
  ../../base/utils/HostState.h \
  ./CommunicationRequest.h \
  ../../modules/phy/DeciderResult80211.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../modules/phy/Decider80211.h \
  ./types.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/utils/FindModule.h \
  ./ScheduleItem.h \
  ../../base/utils/Move.h \
  ./AdHocWiFiDecider.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/ImNotifiable.h \
  ./VideoStreamAppl.h \
  ./AdHocWiFiApplLayer.h \
  ./RequestsCollection.h \
  ../../base/phyLayer/MappingBase.h \
  ./ScheduleItemCollection.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h \
  ../../base/phyLayer/Mapping.h \
  ./rtpdump.h \
  ../../base/messages/ApplPkt_m.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/utils/PassedMessage.h \
  ./RtpData.h \
  ../../base/modules/BaseApplLayer.h \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/Decider.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../modules/utility/Consts80211.h \
  ../../base/phyLayer/BaseDecider.h \
  ../../base/modules/BaseLayer.h
$O/AdHocWiFiMac.o: AdHocWiFiMac.cpp \
  ./AdHocWiFi.h \
  ../../base/utils/HostState.h \
  ./CommunicationRequest.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../modules/phy/DeciderResult80211.h \
  ./Mac80211g.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/messages/MacPkt_m.h \
  ../../modules/phy/Decider80211.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../base/modules/BatteryAccess.h \
  ../../modules/mac/Mac80211.h \
  ../../base/modules/BaseMacLayer.h \
  ../../base/utils/FindModule.h \
  ./ScheduleItem.h \
  ../../base/utils/Move.h \
  ./AdHocWiFiMac.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/ImNotifiable.h \
  ./AdHocWiFiApplLayer.h \
  ../../base/utils/SimpleAddress.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ./RequestsCollection.h \
  ../../base/phyLayer/MappingBase.h \
  ./ScheduleItemCollection.h \
  ../../base/utils/FWMath.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/Coord.h \
  ../../base/phyLayer/PhyToMacControlInfo.h \
  ../../base/phyLayer/MacToPhyControlInfo.h \
  ../../base/messages/ApplPkt_m.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/modules/BaseArp.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/utils/PassedMessage.h \
  ../../base/modules/BaseApplLayer.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/utils/NetwControlInfo.h \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/Decider.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../modules/utility/Consts80211.h \
  ../../base/phyLayer/BaseDecider.h \
  ../../base/modules/BaseLayer.h
$O/AdHocWiFiMobility.o: AdHocWiFiMobility.cpp \
  ../../base/modules/BaseWorldUtility.h \
  ./AdHocWiFi.h \
  ../../base/utils/HostState.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BaseUtility.h \
  ../../base/modules/BaseMobility.h \
  ./AdHocWiFiMobility.h
$O/AdHocWiFiPathLossModel.o: AdHocWiFiPathLossModel.cpp \
  ../../base/utils/HostState.h \
  ../../base/phyLayer/AnalogueModel.h \
  ./Mac80211g.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/messages/MacPkt_m.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../base/modules/BatteryAccess.h \
  ../../modules/mac/Mac80211.h \
  ../../base/modules/BaseMacLayer.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/Move.h \
  ./AdHocWiFiMac.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/Coord.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/modules/BaseArp.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/utils/PassedMessage.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BaseUtility.h \
  ./AdHocWiFiPathLossModel.h \
  ../../modules/utility/Consts80211.h \
  ../../base/modules/BaseLayer.h
$O/AdHocWiFiPhyLayer.o: AdHocWiFiPhyLayer.cpp \
  ../../base/modules/BaseWorldUtility.h \
  ./AdHocWiFi.h \
  ../../base/utils/HostState.h \
  ./CommunicationRequest.h \
  ../../base/phyLayer/AnalogueModel.h \
  ./AdHocWiFiPhyLayer.h \
  ./Mac80211g.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../modules/phy/Decider80211.h \
  ../../base/messages/MacPkt_m.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/modules/BatteryAccess.h \
  ../../modules/mac/Mac80211.h \
  ../../base/modules/BaseMacLayer.h \
  ../../base/utils/FindModule.h \
  ./ScheduleItem.h \
  ../../base/utils/Move.h \
  ./AdHocWiFiMac.h \
  ./AdHocWiFiDecider.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/ImNotifiable.h \
  ./AdHocWiFiApplLayer.h \
  ../../modules/phy/PhyLayer.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ./RequestsCollection.h \
  ../../base/phyLayer/MappingBase.h \
  ./ScheduleItemCollection.h \
  ../../base/utils/FWMath.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/Coord.h \
  ../../base/messages/ApplPkt_m.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/modules/BaseArp.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/utils/PassedMessage.h \
  ../../base/modules/BaseApplLayer.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/Decider.h \
  ../../base/phyLayer/BasePhyLayer.h \
  ../../base/connectionManager/ChannelAccess.h \
  ../../base/modules/BaseUtility.h \
  ./AdHocWiFiPathLossModel.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../base/phyLayer/ChannelInfo.h \
  ../../modules/utility/Consts80211.h \
  ../../base/phyLayer/BaseDecider.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/modules/BaseLayer.h
$O/CommunicationRequest.o: CommunicationRequest.cpp \
  ./CommunicationRequest.h
$O/Mac80211g.o: Mac80211g.cpp \
  ../../base/utils/HostState.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/phyLayer/MappingUtils.h \
  ./Mac80211g.h \
  ../../base/messages/MacPkt_m.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../base/modules/BatteryAccess.h \
  ../../modules/mac/Mac80211.h \
  ../../base/modules/BaseMacLayer.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/modules/BaseArp.h \
  ../../base/utils/PassedMessage.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseUtility.h \
  ../../modules/utility/Consts80211.h \
  ../../base/modules/BaseLayer.h
$O/RequestsCollection.o: RequestsCollection.cpp \
  ./CommunicationRequest.h \
  ./RequestsCollection.h
$O/RtpData.o: RtpData.cpp \
  ./types.h \
  ./ansi.h \
  ./rtpdump.h \
  ./rtp.h \
  ./RtpData.h
$O/ScheduleItem.o: ScheduleItem.cpp \
  ./ScheduleItem.h
$O/ScheduleItemCollection.o: ScheduleItemCollection.cpp \
  ./ScheduleItem.h \
  ./ScheduleItemCollection.h
$O/VideoStreamAppl.o: VideoStreamAppl.cpp \
  ../../base/modules/BaseWorldUtility.h \
  ./AdHocWiFi.h \
  ../../base/utils/HostState.h \
  ./CommunicationRequest.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../modules/phy/DeciderResult80211.h \
  ./Mac80211g.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/messages/MacPkt_m.h \
  ../../modules/phy/Decider80211.h \
  ../../modules/messages/Mac80211Pkt_m.h \
  ../../base/modules/BatteryAccess.h \
  ./types.h \
  ../../modules/mac/Mac80211.h \
  ../../base/modules/BaseMacLayer.h \
  ../../base/utils/FindModule.h \
  ./ScheduleItem.h \
  ../../base/utils/Move.h \
  ./AdHocWiFiMac.h \
  ./AdHocWiFiDecider.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/ImNotifiable.h \
  ./AdHocWiFiApplLayer.h \
  ./VideoStreamAppl.h \
  ../../base/utils/SimpleAddress.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../base/phyLayer/MappingBase.h \
  ./RequestsCollection.h \
  ./ScheduleItemCollection.h \
  ../../base/utils/FWMath.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/Coord.h \
  ./rtpdump.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/messages/ApplPkt_m.h \
  ../../base/modules/BaseArp.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/utils/PassedMessage.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/modules/BaseApplLayer.h \
  ./RtpData.h \
  ../../base/utils/NetwControlInfo.h \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/Decider.h \
  ../../base/modules/BaseUtility.h \
  ../../base/modules/BaseMobility.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ./AdHocWiFiMobility.h \
  ../../modules/utility/Consts80211.h \
  ../../base/phyLayer/BaseDecider.h \
  ../../base/modules/BaseLayer.h
$O/rd.o: rd.cpp \
  ./types.h \
  ./rtpdump.h
