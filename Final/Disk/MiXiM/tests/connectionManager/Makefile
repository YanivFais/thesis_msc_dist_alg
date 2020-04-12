#
# OMNeT++/OMNEST Makefile for connectionManager
#
# This file was generated with the command:
#  opp_makemake -f -o connectionManager -O out -u CMDENV -I../../base/modules -I../../base/phyLayer -I../../base/messages -I../../base/connectionManager -I../../base/utils -L../../out/gcc-debug/base -L../../out/gcc-debug/modules -L../../out/gcc-debug/tests/testUtils -lmiximbase -lmiximmodules -lmiximtestUtils
#

# Name of target to be created (-o option)
TARGET = connectionManager$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
#USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../base/modules \
    -I../../base/phyLayer \
    -I../../base/messages \
    -I../../base/connectionManager \
    -I../../base/utils \
    -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../out/gcc-debug/base -L../../out/gcc-debug/modules -L../../out/gcc-debug/tests/testUtils  -lmiximbase -lmiximmodules -lmiximtestUtils
LIBS += -Wl,-rpath,`abspath ../../out/gcc-debug/base` -Wl,-rpath,`abspath ../../out/gcc-debug/modules` -Wl,-rpath,`abspath ../../out/gcc-debug/tests/testUtils`

# Output directory
PROJECT_OUTPUT_DIR = ../../out
PROJECTRELATIVE_PATH = /tests/connectionManager
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/CMPhyLayer.o $O/ConnectedBCNodePhyLayer.o $O/ConnectedRNodePhyLayer.o $O/NotConnectedBCNodePhyLayer.o $O/NotConnectedRNodePhyLayer.o

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
	-rm -f connectionManager connectionManager.exe libconnectionManager.so libconnectionManager.a libconnectionManager.dll libconnectionManager.dylib
	-rm -f ./*_m.cc ./*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/CMPhyLayer.o: CMPhyLayer.cc \
  ../../base/modules/BaseWorldUtility.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/HostState.h \
  ../../base/utils/Move.h \
  ../../base/connectionManager/ChannelAccess.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/messages/MacPkt_m.h \
  ./CMPhyLayer.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/modules/BaseBattery.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/utils/FindModule.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h
$O/ConnectedBCNodePhyLayer.o: ConnectedBCNodePhyLayer.cc \
  ../../base/modules/BaseWorldUtility.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/HostState.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/connectionManager/ChannelAccess.h \
  ../../base/messages/MacPkt_m.h \
  ./ConnectedBCNodePhyLayer.h \
  ./CMPhyLayer.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h
$O/ConnectedRNodePhyLayer.o: ConnectedRNodePhyLayer.cc \
  ../../base/modules/BaseWorldUtility.h \
  ../../base/modules/BaseModule.h \
  ./ConnectedRNodePhyLayer.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/HostState.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/connectionManager/ChannelAccess.h \
  ../../base/messages/MacPkt_m.h \
  ./CMPhyLayer.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h
$O/NotConnectedBCNodePhyLayer.o: NotConnectedBCNodePhyLayer.cc \
  ../../base/modules/BaseWorldUtility.h \
  ../../base/modules/BaseModule.h \
  ./NotConnectedBCNodePhyLayer.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/HostState.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/connectionManager/ChannelAccess.h \
  ../../base/messages/MacPkt_m.h \
  ./CMPhyLayer.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/phyLayer/Mapping.h \
  ../../base/utils/FindModule.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h
$O/NotConnectedRNodePhyLayer.o: NotConnectedRNodePhyLayer.cc \
  ../../base/modules/BaseWorldUtility.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseUtility.h \
  ../../base/phyLayer/Signal_.h \
  ../../base/messages/AirFrame_m.h \
  ../../base/utils/HostState.h \
  ../../base/utils/Move.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/connectionManager/ChannelAccess.h \
  ../../base/messages/MacPkt_m.h \
  ./CMPhyLayer.h \
  ../../base/phyLayer/MappingUtils.h \
  ../../base/connectionManager/BaseConnectionManager.h \
  ../../base/phyLayer/Mapping.h \
  ./NotConnectedRNodePhyLayer.h \
  ../../base/utils/FindModule.h \
  ../../base/phyLayer/Interpolation.h \
  ../../base/modules/BatteryAccess.h \
  ../../base/modules/BaseBattery.h \
  ../../base/phyLayer/MappingBase.h \
  ../../base/connectionManager/NicEntry.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h
