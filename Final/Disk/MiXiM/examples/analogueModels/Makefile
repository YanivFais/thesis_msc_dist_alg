#
# OMNeT++/OMNEST Makefile for MiXiM
#
# This file was generated with the command:
#  opp_makemake -f -o MiXiM -O out -I../../modules/phy -I../../modules/messages -I../../base/modules -I../../base/messages -I../../base/phyLayer -I../../modules/analogueModel -I../../base/connectionManager -I../../base/utils -I../../modules/utility -L../../out/gcc-debug/base -L../../out/gcc-debug/modules -L../../out/gcc-debug/tests/testUtils -lmiximbase -lmiximmodules
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
    -I../../base/connectionManager \
    -I../../base/utils \
    -I../../modules/utility \
    -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../out/gcc-debug/base -L../../out/gcc-debug/modules -L../../out/gcc-debug/tests/testUtils  -lmiximbase -lmiximmodules
LIBS += -Wl,-rpath,`abspath ../../out/gcc-debug/base` -Wl,-rpath,`abspath ../../out/gcc-debug/modules` -Wl,-rpath,`abspath ../../out/gcc-debug/tests/testUtils`

# Output directory
PROJECT_OUTPUT_DIR = ../../out
PROJECTRELATIVE_PATH = /examples/analogueModels
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/RandomFreqTimeModel.o $O/RandomFrequencyOnlyModel.o $O/SamplePhyLayer.o $O/SimpleMacLayer.o

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
	ln -s -f $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@mkdir -p $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

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
	-rm -f MiXiM MiXiM.exe libMiXiM.so libMiXiM.a libMiXiM.dll libMiXiM.dylib
	-rm -f ./*_m.cc ./*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/RandomFreqTimeModel.o: RandomFreqTimeModel.cc \
  ./RandomFreqTimeModel.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/HostState.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/utils/Move.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h
$O/RandomFrequencyOnlyModel.o: RandomFrequencyOnlyModel.cc \
  ./RandomFrequencyOnlyModel.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/HostState.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/utils/Move.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h
$O/SamplePhyLayer.o: SamplePhyLayer.cc \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseWorldUtility.h \
  ../../base/modules/BaseUtility.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/HostState.h \
  ../../modules/phy/PhyLayer.h \
  ../../base/connectionManager/ChannelAccess.h \
  ../../base/phyLayer/ChannelInfo.h \
  ../../base/messages/MacPkt_m.h \
  ../../base/phyLayer/DeciderToPhyInterface.h \
  ../../base/phyLayer/BasePhyLayer.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/Coord.h \
  ./RandomFrequencyOnlyModel.h \
  ../../base/messages/ChannelSenseRequest_m.h \
  ./RandomFreqTimeModel.h \
  ../../base/modules/BaseModule.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/utils/Move.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/phyLayer/Decider.h \
  ./SamplePhyLayer.h \
  ../../base/phyLayer/PhyUtils.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/utils/FWMath.h \
  ./ThresholdDecider.h
$O/SimpleMacLayer.o: SimpleMacLayer.cc \
  ../../base/phyLayer/ChannelState.h \
  ../../base/modules/BaseUtility.h \
  ../../base/utils/HostState.h \
  ../../base/messages/MacPkt_m.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/Coord.h \
  ../../base/phyLayer/MacToPhyInterface.h \
  ../../base/modules/BaseModule.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/phyLayer/MacToPhyControlInfo.h \
  ../../base/phyLayer/AnalogueModel.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/phyLayer/PhyUtils.h \
  ./SimpleMacLayer.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h
