#
# OMNeT++/OMNEST Makefile for libmiximbase
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -o miximbase -O out -L../out/gcc-debug/modules -L../out/gcc-debug/tests/testUtils
#

# Name of target to be created (-o option)
TARGET = libmiximbase$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -IconnectionManager \
    -Imessages \
    -Imodules \
    -IphyLayer \
    -Itimers \
    -Iutils

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../out/gcc-debug/modules -L../out/gcc-debug/tests/testUtils
LIBS += -Wl,-rpath,`abspath ../out/gcc-debug/modules` -Wl,-rpath,`abspath ../out/gcc-debug/tests/testUtils`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = /base
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/connectionManager/BaseConnectionManager.o \
    $O/connectionManager/ChannelAccess.o \
    $O/connectionManager/ConnectionManager.o \
    $O/connectionManager/NicEntryDebug.o \
    $O/connectionManager/NicEntryDirect.o \
    $O/modules/BaseApplLayer.o \
    $O/modules/BaseArp.o \
    $O/modules/BaseBattery.o \
    $O/modules/BaseLayer.o \
    $O/modules/BaseMacLayer.o \
    $O/modules/BaseMobility.o \
    $O/modules/BaseModule.o \
    $O/modules/BaseNetwLayer.o \
    $O/modules/BaseUtility.o \
    $O/modules/BaseWorldUtility.o \
    $O/modules/BatteryAccess.o \
    $O/modules/TestApplLayer.o \
    $O/phyLayer/BaseDecider.o \
    $O/phyLayer/BasePhyLayer.o \
    $O/phyLayer/ChannelInfo.o \
    $O/phyLayer/ChannelState.o \
    $O/phyLayer/Decider.o \
    $O/phyLayer/MappingBase.o \
    $O/phyLayer/MappingUtils.o \
    $O/phyLayer/PhyUtils.o \
    $O/phyLayer/Signal.o \
    $O/timers/FrameTimer.o \
    $O/timers/FrameTimerGenerator.o \
    $O/timers/GlobalTime.o \
    $O/timers/RepeatTimer.o \
    $O/timers/RepeatTimerCore.o \
    $O/timers/Timer.o \
    $O/timers/TimerCore.o \
    $O/utils/Coord.o \
    $O/utils/winsupport.o \
    $O/messages/AirFrame_m.o \
    $O/messages/ApplPkt_m.o \
    $O/messages/BorderMsg_m.o \
    $O/messages/ChannelSenseRequest_m.o \
    $O/messages/MacPkt_m.o \
    $O/messages/NetwPkt_m.o

# Message files
MSGFILES = \
    messages/AirFrame.msg \
    messages/ApplPkt.msg \
    messages/BorderMsg.msg \
    messages/ChannelSenseRequest.msg \
    messages/MacPkt.msg \
    messages/NetwPkt.msg

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
	-rm -f miximbase miximbase.exe libmiximbase.so libmiximbase.a libmiximbase.dll libmiximbase.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f connectionManager/*_m.cc connectionManager/*_m.h
	-rm -f messages/*_m.cc messages/*_m.h
	-rm -f modules/*_m.cc modules/*_m.h
	-rm -f phyLayer/*_m.cc phyLayer/*_m.h
	-rm -f timers/*_m.cc timers/*_m.h
	-rm -f utils/*_m.cc utils/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc connectionManager/*.cc messages/*.cc modules/*.cc phyLayer/*.cc timers/*.cc utils/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/connectionManager/BaseConnectionManager.o: connectionManager/BaseConnectionManager.cc \
  utils/FindModule.h \
  modules/BaseWorldUtility.h \
  utils/FWMath.h \
  connectionManager/NicEntry.h \
  connectionManager/NicEntryDebug.h \
  utils/Coord.h \
  connectionManager/BaseConnectionManager.h \
  connectionManager/NicEntryDirect.h
$O/connectionManager/ChannelAccess.o: connectionManager/ChannelAccess.cc \
  phyLayer/MappingUtils.h \
  utils/FWMath.h \
  connectionManager/NicEntry.h \
  modules/BaseBattery.h \
  messages/AirFrame_m.h \
  modules/BaseModule.h \
  utils/Move.h \
  utils/HostState.h \
  modules/BaseWorldUtility.h \
  utils/ImNotifiable.h \
  utils/FindModule.h \
  connectionManager/ChannelAccess.h \
  modules/BatteryAccess.h \
  phyLayer/Signal_.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  connectionManager/BaseConnectionManager.h \
  phyLayer/Interpolation.h \
  modules/BaseUtility.h \
  phyLayer/Mapping.h
$O/connectionManager/ConnectionManager.o: connectionManager/ConnectionManager.cc \
  utils/FWMath.h \
  connectionManager/NicEntry.h \
  connectionManager/ConnectionManager.h \
  modules/BaseWorldUtility.h \
  utils/Coord.h \
  connectionManager/BaseConnectionManager.h
$O/connectionManager/NicEntryDebug.o: connectionManager/NicEntryDebug.cc \
  phyLayer/MappingUtils.h \
  utils/FWMath.h \
  connectionManager/NicEntry.h \
  modules/BaseBattery.h \
  messages/AirFrame_m.h \
  utils/Move.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/FindModule.h \
  utils/ImNotifiable.h \
  modules/BaseWorldUtility.h \
  connectionManager/ChannelAccess.h \
  phyLayer/Signal_.h \
  modules/BatteryAccess.h \
  connectionManager/NicEntryDebug.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h \
  connectionManager/BaseConnectionManager.h \
  modules/BaseUtility.h \
  phyLayer/Mapping.h
$O/connectionManager/NicEntryDirect.o: connectionManager/NicEntryDirect.cc \
  phyLayer/MappingUtils.h \
  utils/FWMath.h \
  connectionManager/NicEntry.h \
  modules/BaseBattery.h \
  messages/AirFrame_m.h \
  utils/Move.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/FindModule.h \
  utils/ImNotifiable.h \
  modules/BaseWorldUtility.h \
  connectionManager/ChannelAccess.h \
  phyLayer/Signal_.h \
  modules/BatteryAccess.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h \
  connectionManager/BaseConnectionManager.h \
  modules/BaseUtility.h \
  connectionManager/NicEntryDirect.h \
  phyLayer/Mapping.h
$O/modules/BaseApplLayer.o: modules/BaseApplLayer.cc \
  modules/BaseLayer.h \
  utils/FWMath.h \
  messages/ApplPkt_m.h \
  modules/BaseBattery.h \
  modules/BaseModule.h \
  utils/PassedMessage.h \
  utils/HostState.h \
  utils/FindModule.h \
  utils/ImNotifiable.h \
  modules/BaseApplLayer.h \
  modules/BatteryAccess.h \
  utils/Coord.h \
  modules/BaseUtility.h
$O/modules/BaseArp.o: modules/BaseArp.cc \
  modules/BaseArp.h \
  utils/FWMath.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/FindModule.h \
  utils/ImNotifiable.h \
  utils/Coord.h \
  modules/BaseUtility.h
$O/modules/BaseBattery.o: modules/BaseBattery.cc \
  utils/FWMath.h \
  modules/BaseBattery.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/FindModule.h \
  utils/ImNotifiable.h \
  utils/Coord.h \
  modules/BaseUtility.h
$O/modules/BaseLayer.o: modules/BaseLayer.cc \
  modules/BaseLayer.h \
  utils/FWMath.h \
  modules/BaseBattery.h \
  modules/BaseModule.h \
  utils/PassedMessage.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  utils/FindModule.h \
  modules/BatteryAccess.h \
  utils/Coord.h \
  modules/BaseUtility.h
$O/modules/BaseMacLayer.o: modules/BaseMacLayer.cc \
  modules/BaseLayer.h \
  phyLayer/MacToPhyInterface.h \
  phyLayer/MappingUtils.h \
  utils/FWMath.h \
  modules/BaseBattery.h \
  modules/BaseMacLayer.h \
  phyLayer/AnalogueModel.h \
  phyLayer/ChannelState.h \
  messages/MacPkt_m.h \
  utils/MacControlInfo.h \
  phyLayer/PhyUtils.h \
  modules/BaseModule.h \
  utils/Move.h \
  utils/PassedMessage.h \
  utils/HostState.h \
  utils/FindModule.h \
  utils/ImNotifiable.h \
  modules/BatteryAccess.h \
  phyLayer/Signal_.h \
  utils/SimpleAddress.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h \
  modules/BaseUtility.h \
  phyLayer/Mapping.h
$O/modules/BaseMobility.o: modules/BaseMobility.cc \
  messages/BorderMsg_m.h \
  utils/FWMath.h \
  modules/BaseBattery.h \
  modules/BaseMobility.h \
  utils/Move.h \
  modules/BaseModule.h \
  utils/HostState.h \
  modules/BaseWorldUtility.h \
  utils/ImNotifiable.h \
  utils/FindModule.h \
  modules/BatteryAccess.h \
  utils/Coord.h \
  modules/BaseUtility.h
$O/modules/BaseModule.o: modules/BaseModule.cc \
  utils/FWMath.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  utils/FindModule.h \
  utils/Coord.h \
  modules/BaseUtility.h
$O/modules/BaseNetwLayer.o: modules/BaseNetwLayer.cc \
  modules/BaseLayer.h \
  modules/BaseArp.h \
  phyLayer/MacToPhyInterface.h \
  phyLayer/MappingUtils.h \
  utils/FWMath.h \
  modules/BaseBattery.h \
  modules/BaseMacLayer.h \
  phyLayer/AnalogueModel.h \
  phyLayer/ChannelState.h \
  messages/MacPkt_m.h \
  utils/MacControlInfo.h \
  phyLayer/PhyUtils.h \
  utils/Move.h \
  modules/BaseModule.h \
  utils/NetwControlInfo.h \
  utils/PassedMessage.h \
  utils/HostState.h \
  utils/FindModule.h \
  utils/ImNotifiable.h \
  messages/NetwPkt_m.h \
  modules/BaseNetwLayer.h \
  phyLayer/Signal_.h \
  modules/BatteryAccess.h \
  utils/SimpleAddress.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h \
  modules/BaseUtility.h \
  phyLayer/Mapping.h
$O/modules/BaseUtility.o: modules/BaseUtility.cc \
  utils/FWMath.h \
  utils/Move.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  utils/FindModule.h \
  modules/BaseWorldUtility.h \
  utils/Coord.h \
  modules/BaseUtility.h
$O/modules/BaseWorldUtility.o: modules/BaseWorldUtility.cc \
  utils/FWMath.h \
  modules/BaseWorldUtility.h \
  utils/Coord.h
$O/modules/BatteryAccess.o: modules/BatteryAccess.cc \
  utils/FWMath.h \
  modules/BaseBattery.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/FindModule.h \
  utils/ImNotifiable.h \
  modules/BatteryAccess.h \
  utils/Coord.h \
  modules/BaseUtility.h
$O/modules/TestApplLayer.o: modules/TestApplLayer.cc \
  modules/BaseLayer.h \
  utils/FWMath.h \
  messages/ApplPkt_m.h \
  modules/BaseBattery.h \
  modules/BaseModule.h \
  utils/NetwControlInfo.h \
  utils/PassedMessage.h \
  modules/TestApplLayer.h \
  utils/HostState.h \
  modules/BaseApplLayer.h \
  utils/ImNotifiable.h \
  utils/FindModule.h \
  modules/BatteryAccess.h \
  utils/SimpleAddress.h \
  utils/Coord.h \
  modules/BaseUtility.h
$O/phyLayer/BaseDecider.o: phyLayer/BaseDecider.cc \
  messages/ChannelSenseRequest_m.h \
  phyLayer/MappingUtils.h \
  utils/FWMath.h \
  phyLayer/DeciderToPhyInterface.h \
  phyLayer/ChannelState.h \
  messages/AirFrame_m.h \
  utils/Move.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  phyLayer/BaseDecider.h \
  phyLayer/Signal_.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h \
  modules/BaseUtility.h \
  phyLayer/Decider.h \
  phyLayer/Mapping.h
$O/phyLayer/BasePhyLayer.o: phyLayer/BasePhyLayer.cc \
  messages/ChannelSenseRequest_m.h \
  phyLayer/MacToPhyInterface.h \
  utils/FWMath.h \
  modules/BaseBattery.h \
  phyLayer/AnalogueModel.h \
  phyLayer/ChannelState.h \
  messages/AirFrame_m.h \
  phyLayer/PhyUtils.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  modules/BaseWorldUtility.h \
  connectionManager/ChannelAccess.h \
  phyLayer/Signal_.h \
  modules/BaseUtility.h \
  phyLayer/Decider.h \
  phyLayer/MacToPhyControlInfo.h \
  phyLayer/MappingUtils.h \
  phyLayer/BasePhyLayer.h \
  phyLayer/DeciderToPhyInterface.h \
  connectionManager/NicEntry.h \
  messages/MacPkt_m.h \
  utils/Move.h \
  utils/FindModule.h \
  modules/BatteryAccess.h \
  phyLayer/ChannelInfo.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/PhyToMacControlInfo.h \
  phyLayer/Interpolation.h \
  connectionManager/BaseConnectionManager.h \
  phyLayer/Mapping.h
$O/phyLayer/ChannelInfo.o: phyLayer/ChannelInfo.cc \
  utils/FWMath.h \
  messages/AirFrame_m.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  phyLayer/Signal_.h \
  modules/BaseUtility.h \
  phyLayer/MappingUtils.h \
  utils/Move.h \
  phyLayer/ChannelInfo.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h \
  phyLayer/Mapping.h
$O/phyLayer/ChannelState.o: phyLayer/ChannelState.cc \
  phyLayer/ChannelState.h
$O/phyLayer/Decider.o: phyLayer/Decider.cc \
  messages/ChannelSenseRequest_m.h \
  utils/FWMath.h \
  phyLayer/ChannelState.h \
  messages/AirFrame_m.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  phyLayer/Signal_.h \
  modules/BaseUtility.h \
  phyLayer/Decider.h \
  phyLayer/MappingUtils.h \
  phyLayer/DeciderToPhyInterface.h \
  utils/Move.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h \
  phyLayer/Mapping.h
$O/phyLayer/MappingBase.o: phyLayer/MappingBase.cc \
  utils/FWMath.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h
$O/phyLayer/MappingUtils.o: phyLayer/MappingUtils.cc \
  utils/FWMath.h \
  phyLayer/MappingUtils.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h
$O/phyLayer/PhyUtils.o: phyLayer/PhyUtils.cc \
  utils/FWMath.h \
  phyLayer/AnalogueModel.h \
  phyLayer/PhyUtils.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  phyLayer/Signal_.h \
  modules/BaseUtility.h \
  phyLayer/MappingUtils.h \
  utils/Move.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h \
  phyLayer/Mapping.h
$O/phyLayer/Signal.o: phyLayer/Signal.cc \
  utils/FWMath.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  phyLayer/Signal_.h \
  modules/BaseUtility.h \
  phyLayer/MappingUtils.h \
  utils/Move.h \
  utils/Coord.h \
  phyLayer/MappingBase.h \
  phyLayer/Interpolation.h \
  phyLayer/Mapping.h
$O/timers/FrameTimer.o: timers/FrameTimer.cc \
  utils/FWMath.h \
  timers/GlobalTime.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  modules/BaseUtility.h \
  timers/FrameTimer.h \
  timers/FrameTimerGenerator.h \
  utils/FindModule.h \
  utils/Coord.h
$O/timers/FrameTimerGenerator.o: timers/FrameTimerGenerator.cc \
  utils/FWMath.h \
  timers/GlobalTime.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  modules/BaseUtility.h \
  timers/FrameTimer.h \
  timers/FrameTimerGenerator.h \
  utils/FindModule.h \
  utils/Coord.h
$O/timers/GlobalTime.o: timers/GlobalTime.cc \
  timers/GlobalTime.h
$O/timers/RepeatTimer.o: timers/RepeatTimer.cc \
  utils/FWMath.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  modules/BaseUtility.h \
  timers/RepeatTimerCore.h \
  utils/FindModule.h \
  timers/RepeatTimer.h \
  utils/Coord.h
$O/timers/RepeatTimerCore.o: timers/RepeatTimerCore.cc \
  utils/FWMath.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  modules/BaseUtility.h \
  timers/RepeatTimerCore.h \
  utils/FindModule.h \
  timers/RepeatTimer.h \
  utils/Coord.h
$O/timers/Timer.o: timers/Timer.cc \
  utils/FWMath.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  modules/BaseUtility.h \
  timers/TimerCore.h \
  timers/Timer.h \
  utils/FindModule.h \
  utils/Coord.h
$O/timers/TimerCore.o: timers/TimerCore.cc \
  utils/FWMath.h \
  modules/BaseModule.h \
  utils/HostState.h \
  utils/ImNotifiable.h \
  modules/BaseUtility.h \
  timers/TimerCore.h \
  timers/Timer.h \
  utils/FindModule.h \
  utils/Coord.h
$O/utils/Coord.o: utils/Coord.cc \
  utils/FWMath.h \
  utils/Coord.h
$O/utils/winsupport.o: utils/winsupport.cc \
  utils/winsupport.h
