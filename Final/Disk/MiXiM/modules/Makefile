#
# OMNeT++/OMNEST Makefile for libmiximmodules
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -o miximmodules -O out -I../base/modules -I../base/messages -I../base/phyLayer -I../base/connectionManager -I../base/utils -L../out/gcc-debug/base -L../out/gcc-debug/tests/testUtils -lmiximbase
#

# Name of target to be created (-o option)
TARGET = libmiximmodules$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../base/modules \
    -I../base/messages \
    -I../base/phyLayer \
    -I../base/connectionManager \
    -I../base/utils \
    -I. \
    -IanalogueModel \
    -Iapplication \
    -IconnectionManager \
    -Imac \
    -Imessages \
    -Imobility \
    -Inic \
    -Iphy \
    -Ipower \
    -Ipower/battery \
    -Iutility

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../out/gcc-debug/base -L../out/gcc-debug/tests/testUtils  -lmiximbase
LIBS += -Wl,-rpath,`abspath ../out/gcc-debug/base` -Wl,-rpath,`abspath ../out/gcc-debug/tests/testUtils`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = /modules
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/analogueModel/JakesFading.o \
    $O/analogueModel/LogNormalShadowing.o \
    $O/analogueModel/SimplePathlossModel.o \
    $O/application/BurstApplLayer.o \
    $O/application/BurstApplLayerBattery.o \
    $O/connectionManager/UnitDisk.o \
    $O/mac/CSMAMacLayer.o \
    $O/mac/Mac80211.o \
    $O/mobility/ANSimMobility.o \
    $O/mobility/BonnMotionFileCache.o \
    $O/mobility/BonnMotionMobility.o \
    $O/mobility/CircleMobility.o \
    $O/mobility/ConstSpeedMobility.o \
    $O/mobility/LinearMobility.o \
    $O/mobility/LineSegmentsMobilityBase.o \
    $O/mobility/MassMobility.o \
    $O/mobility/RectangleMobility.o \
    $O/mobility/TractorMobility.o \
    $O/mobility/TurtleMobility.o \
    $O/phy/Decider80211.o \
    $O/phy/Decider80211Battery.o \
    $O/phy/PhyLayer.o \
    $O/phy/PhyLayerBattery.o \
    $O/phy/SNRThresholdDecider.o \
    $O/power/battery/BatteryStats.o \
    $O/power/battery/SimpleBattery.o \
    $O/messages/Mac80211Pkt_m.o

# Message files
MSGFILES = \
    messages/Mac80211Pkt.msg

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
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

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
	ln -s -f $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@mkdir -p $O
	$(SHLIB_LD) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(LIBS) $(OMNETPP_LIBS) $(LDFLAGS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f miximmodules miximmodules.exe libmiximmodules.so libmiximmodules.a libmiximmodules.dll libmiximmodules.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f analogueModel/*_m.cc analogueModel/*_m.h
	-rm -f application/*_m.cc application/*_m.h
	-rm -f connectionManager/*_m.cc connectionManager/*_m.h
	-rm -f mac/*_m.cc mac/*_m.h
	-rm -f messages/*_m.cc messages/*_m.h
	-rm -f mobility/*_m.cc mobility/*_m.h
	-rm -f nic/*_m.cc nic/*_m.h
	-rm -f phy/*_m.cc phy/*_m.h
	-rm -f power/*_m.cc power/*_m.h
	-rm -f power/battery/*_m.cc power/battery/*_m.h
	-rm -f utility/*_m.cc utility/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc analogueModel/*.cc application/*.cc connectionManager/*.cc mac/*.cc messages/*.cc mobility/*.cc nic/*.cc phy/*.cc power/*.cc power/battery/*.cc utility/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/analogueModel/JakesFading.o: analogueModel/JakesFading.cc \
  ../base/phyLayer/MappingUtils.h \
  ../base/phyLayer/MappingBase.h \
  analogueModel/JakesFading.h \
  ../base/phyLayer/Interpolation.h \
  ../base/phyLayer/Signal_.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/phyLayer/Mapping.h \
  ../base/utils/HostState.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/utils/ImNotifiable.h \
  ../base/phyLayer/AnalogueModel.h \
  ../base/modules/BaseUtility.h \
  ../base/utils/FWMath.h
$O/analogueModel/LogNormalShadowing.o: analogueModel/LogNormalShadowing.cc \
  ../base/phyLayer/MappingUtils.h \
  ../base/phyLayer/MappingBase.h \
  ../base/phyLayer/Interpolation.h \
  ../base/phyLayer/Signal_.h \
  ../base/phyLayer/Mapping.h \
  ../base/utils/HostState.h \
  analogueModel/LogNormalShadowing.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/utils/ImNotifiable.h \
  ../base/phyLayer/AnalogueModel.h \
  ../base/modules/BaseUtility.h \
  ../base/utils/FWMath.h
$O/analogueModel/SimplePathlossModel.o: analogueModel/SimplePathlossModel.cc \
  ../base/phyLayer/MappingUtils.h \
  ../base/phyLayer/MappingBase.h \
  analogueModel/SimplePathlossModel.h \
  ../base/phyLayer/Interpolation.h \
  ../base/phyLayer/Signal_.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/phyLayer/Mapping.h \
  ../base/utils/HostState.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/utils/ImNotifiable.h \
  ../base/phyLayer/AnalogueModel.h \
  ../base/modules/BaseUtility.h \
  ../base/utils/FWMath.h
$O/application/BurstApplLayer.o: application/BurstApplLayer.cc \
  ../base/utils/FindModule.h \
  ../base/modules/BaseApplLayer.h \
  ../base/utils/PassedMessage.h \
  ../base/modules/BaseModule.h \
  ../base/modules/BaseBattery.h \
  ../base/messages/ApplPkt_m.h \
  ../base/modules/BatteryAccess.h \
  application/BurstApplLayer.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseLayer.h \
  ../base/utils/Coord.h \
  ../base/utils/ImNotifiable.h \
  ../base/modules/TestApplLayer.h \
  ../base/modules/BaseUtility.h \
  ../base/utils/FWMath.h
$O/application/BurstApplLayerBattery.o: application/BurstApplLayerBattery.cc \
  ../base/modules/BaseApplLayer.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseModule.h \
  application/BurstApplLayer.h \
  application/BurstApplLayerBattery.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/TestApplLayer.h \
  ../base/utils/FindModule.h \
  ../base/utils/PassedMessage.h \
  ../base/messages/ApplPkt_m.h \
  ../base/modules/BatteryAccess.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseLayer.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/connectionManager/UnitDisk.o: connectionManager/UnitDisk.cc \
  ../base/connectionManager/NicEntry.h \
  ../base/connectionManager/ConnectionManager.h \
  connectionManager/UnitDisk.h \
  ../base/utils/Coord.h \
  ../base/connectionManager/BaseConnectionManager.h \
  ../base/utils/FWMath.h
$O/mac/CSMAMacLayer.o: mac/CSMAMacLayer.cc \
  ../base/phyLayer/MappingUtils.h \
  ../base/modules/BaseMacLayer.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseModule.h \
  ../base/phyLayer/Interpolation.h \
  ../base/phyLayer/Mapping.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/utils/FindModule.h \
  ../base/messages/MacPkt_m.h \
  ../base/utils/PassedMessage.h \
  ../base/phyLayer/MappingBase.h \
  ../base/utils/SimpleAddress.h \
  mac/CSMAMacLayer.h \
  ../base/phyLayer/Signal_.h \
  ../base/modules/BatteryAccess.h \
  ../base/phyLayer/PhyUtils.h \
  ../base/phyLayer/MacToPhyControlInfo.h \
  ../base/phyLayer/ChannelState.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseLayer.h \
  ../base/utils/ImNotifiable.h \
  ../base/modules/BaseArp.h \
  ../base/phyLayer/MacToPhyInterface.h \
  ../base/phyLayer/AnalogueModel.h \
  ../base/utils/FWMath.h
$O/mac/Mac80211.o: mac/Mac80211.cc \
  ../base/utils/MacControlInfo.h \
  ../base/modules/BaseModule.h \
  mac/Mac80211.h \
  ../base/phyLayer/Interpolation.h \
  phy/Decider80211.h \
  ../base/utils/FindModule.h \
  messages/Mac80211Pkt_m.h \
  ../base/utils/PassedMessage.h \
  ../base/phyLayer/Signal_.h \
  ../base/phyLayer/PhyToMacControlInfo.h \
  ../base/phyLayer/PhyUtils.h \
  ../base/phyLayer/ChannelState.h \
  ../base/phyLayer/BaseDecider.h \
  ../base/phyLayer/Decider.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseLayer.h \
  ../base/phyLayer/DeciderToPhyInterface.h \
  utility/Consts80211.h \
  ../base/modules/BaseArp.h \
  ../base/phyLayer/MacToPhyInterface.h \
  ../base/phyLayer/MappingUtils.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseMacLayer.h \
  ../base/phyLayer/Mapping.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/messages/MacPkt_m.h \
  ../base/messages/ChannelSenseRequest_m.h \
  ../base/utils/SimpleAddress.h \
  ../base/phyLayer/MappingBase.h \
  phy/DeciderResult80211.h \
  ../base/modules/BatteryAccess.h \
  ../base/phyLayer/MacToPhyControlInfo.h \
  ../base/messages/AirFrame_m.h \
  ../base/utils/ImNotifiable.h \
  ../base/phyLayer/AnalogueModel.h \
  ../base/utils/FWMath.h
$O/mobility/ANSimMobility.o: mobility/ANSimMobility.cc \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  mobility/ANSimMobility.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  mobility/LineSegmentsMobilityBase.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/BonnMotionFileCache.o: mobility/BonnMotionFileCache.cc \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  mobility/BonnMotionFileCache.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/BonnMotionMobility.o: mobility/BonnMotionMobility.cc \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  mobility/BonnMotionFileCache.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  mobility/LineSegmentsMobilityBase.h \
  mobility/BonnMotionMobility.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/CircleMobility.o: mobility/CircleMobility.cc \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  mobility/CircleMobility.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/ConstSpeedMobility.o: mobility/ConstSpeedMobility.cc \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  mobility/ConstSpeedMobility.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/LineSegmentsMobilityBase.o: mobility/LineSegmentsMobilityBase.cc \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  mobility/LineSegmentsMobilityBase.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/LinearMobility.o: mobility/LinearMobility.cc \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  mobility/LinearMobility.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/MassMobility.o: mobility/MassMobility.cc \
  mobility/MassMobility.h \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/RectangleMobility.o: mobility/RectangleMobility.cc \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  mobility/RectangleMobility.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/TractorMobility.o: mobility/TractorMobility.cc \
  ../base/modules/BaseModule.h \
  mobility/TractorMobility.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/mobility/TurtleMobility.o: mobility/TurtleMobility.cc \
  ../base/modules/BaseModule.h \
  ../base/utils/FindModule.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseMobility.h \
  ../base/modules/BaseBattery.h \
  mobility/TurtleMobility.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/modules/BatteryAccess.h \
  mobility/LineSegmentsMobilityBase.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/phy/Decider80211.o: phy/Decider80211.cc \
  ../base/phyLayer/Interpolation.h \
  phy/Decider80211.h \
  messages/Mac80211Pkt_m.h \
  ../base/phyLayer/Signal_.h \
  ../base/phyLayer/ChannelState.h \
  ../base/phyLayer/BaseDecider.h \
  ../base/utils/HostState.h \
  ../base/phyLayer/Decider.h \
  utility/Consts80211.h \
  ../base/phyLayer/DeciderToPhyInterface.h \
  ../base/phyLayer/MappingUtils.h \
  ../base/phyLayer/Mapping.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/messages/MacPkt_m.h \
  ../base/messages/ChannelSenseRequest_m.h \
  ../base/phyLayer/MappingBase.h \
  phy/DeciderResult80211.h \
  ../base/messages/AirFrame_m.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/phy/Decider80211Battery.o: phy/Decider80211Battery.cc \
  ../base/phyLayer/Interpolation.h \
  phy/Decider80211.h \
  ../base/phyLayer/Signal_.h \
  ../base/phyLayer/ChannelState.h \
  ../base/phyLayer/Decider.h \
  ../base/utils/HostState.h \
  ../base/phyLayer/BaseDecider.h \
  ../base/phyLayer/DeciderToPhyInterface.h \
  utility/Consts80211.h \
  phy/Decider80211Battery.h \
  ../base/phyLayer/MappingUtils.h \
  ../base/phyLayer/Mapping.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/messages/ChannelSenseRequest_m.h \
  ../base/phyLayer/MappingBase.h \
  ../base/messages/AirFrame_m.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/phy/PhyLayer.o: phy/PhyLayer.cc \
  ../base/modules/BaseModule.h \
  ../base/phyLayer/Interpolation.h \
  phy/Decider80211.h \
  ../base/phyLayer/ChannelInfo.h \
  ../base/utils/FindModule.h \
  analogueModel/JakesFading.h \
  ../base/connectionManager/ChannelAccess.h \
  ../base/phyLayer/Signal_.h \
  ../base/phyLayer/PhyUtils.h \
  ../base/phyLayer/ChannelState.h \
  phy/PhyLayer.h \
  ../base/phyLayer/BaseDecider.h \
  ../base/utils/HostState.h \
  ../base/phyLayer/Decider.h \
  utility/Consts80211.h \
  ../base/phyLayer/DeciderToPhyInterface.h \
  ../base/phyLayer/MacToPhyInterface.h \
  ../base/phyLayer/MappingUtils.h \
  ../base/connectionManager/NicEntry.h \
  ../base/phyLayer/BasePhyLayer.h \
  ../base/modules/BaseBattery.h \
  phy/SNRThresholdDecider.h \
  analogueModel/SimplePathlossModel.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/phyLayer/Mapping.h \
  analogueModel/LogNormalShadowing.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/messages/ChannelSenseRequest_m.h \
  ../base/messages/MacPkt_m.h \
  ../base/phyLayer/MappingBase.h \
  ../base/modules/BatteryAccess.h \
  ../base/messages/AirFrame_m.h \
  ../base/utils/ImNotifiable.h \
  ../base/connectionManager/BaseConnectionManager.h \
  ../base/utils/FWMath.h \
  ../base/phyLayer/AnalogueModel.h
$O/phy/PhyLayerBattery.o: phy/PhyLayerBattery.cc \
  ../base/modules/BaseModule.h \
  ../base/phyLayer/Interpolation.h \
  phy/Decider80211.h \
  ../base/phyLayer/ChannelInfo.h \
  ../base/utils/FindModule.h \
  ../base/connectionManager/ChannelAccess.h \
  ../base/phyLayer/Signal_.h \
  ../base/phyLayer/PhyUtils.h \
  phy/PhyLayer.h \
  ../base/phyLayer/ChannelState.h \
  ../base/utils/HostState.h \
  ../base/phyLayer/Decider.h \
  ../base/phyLayer/BaseDecider.h \
  ../base/phyLayer/DeciderToPhyInterface.h \
  utility/Consts80211.h \
  phy/PhyLayerBattery.h \
  ../base/phyLayer/MacToPhyInterface.h \
  phy/Decider80211Battery.h \
  ../base/phyLayer/MappingUtils.h \
  ../base/connectionManager/NicEntry.h \
  ../base/phyLayer/BasePhyLayer.h \
  ../base/modules/BaseBattery.h \
  ../base/modules/BaseWorldUtility.h \
  ../base/phyLayer/Mapping.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/messages/ChannelSenseRequest_m.h \
  ../base/messages/MacPkt_m.h \
  ../base/phyLayer/MappingBase.h \
  ../base/modules/BatteryAccess.h \
  ../base/messages/AirFrame_m.h \
  ../base/utils/ImNotifiable.h \
  ../base/connectionManager/BaseConnectionManager.h \
  ../base/utils/FWMath.h \
  ../base/phyLayer/AnalogueModel.h
$O/phy/SNRThresholdDecider.o: phy/SNRThresholdDecider.cc \
  ../base/phyLayer/Interpolation.h \
  ../base/phyLayer/Signal_.h \
  ../base/phyLayer/ChannelState.h \
  ../base/phyLayer/BaseDecider.h \
  ../base/utils/HostState.h \
  ../base/phyLayer/Decider.h \
  ../base/phyLayer/DeciderToPhyInterface.h \
  ../base/phyLayer/MappingUtils.h \
  phy/SNRThresholdDecider.h \
  ../base/phyLayer/Mapping.h \
  ../base/utils/Move.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/messages/ChannelSenseRequest_m.h \
  ../base/phyLayer/MappingBase.h \
  ../base/messages/AirFrame_m.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/power/battery/BatteryStats.o: power/battery/BatteryStats.cc \
  ../base/modules/BaseModule.h \
  power/battery/BatteryState.h \
  power/battery/BatteryStats.h \
  power/battery/SimpleBattery.h \
  ../base/utils/FindModule.h \
  power/battery/DeviceEntry.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseBattery.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
$O/power/battery/SimpleBattery.o: power/battery/SimpleBattery.cc \
  ../base/modules/BaseModule.h \
  power/battery/BatteryState.h \
  power/battery/BatteryStats.h \
  power/battery/SimpleBattery.h \
  ../base/utils/FindModule.h \
  power/battery/DeviceEntry.h \
  ../base/utils/HostState.h \
  ../base/modules/BaseBattery.h \
  ../base/utils/Coord.h \
  ../base/modules/BaseUtility.h \
  ../base/utils/ImNotifiable.h \
  ../base/utils/FWMath.h
