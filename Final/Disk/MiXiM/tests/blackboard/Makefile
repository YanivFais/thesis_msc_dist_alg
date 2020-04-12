#
# OMNeT++/OMNEST Makefile for blackboard
#
# This file was generated with the command:
#  opp_makemake -f -o blackboard -O out -u CMDENV -I../../base/messages -I../../base/phyLayer -I../../base/modules -I../../base/connectionManager -I../../base/utils -L../../out/gcc-debug/base -L../../out/gcc-debug/modules -L../../out/gcc-debug/tests/testUtils -lmiximbase -lmiximmodules -lmiximtestUtils
#

# Name of target to be created (-o option)
TARGET = blackboard$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
#USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../base/messages \
    -I../../base/phyLayer \
    -I../../base/modules \
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
PROJECTRELATIVE_PATH = /tests/blackboard
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/StateChanger.o $O/StateView.o $O/StateVisualize.o $O/TestStateSub.o

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
	-rm -f blackboard blackboard.exe libblackboard.so libblackboard.a libblackboard.dll libblackboard.dylib
	-rm -f ./*_m.cc ./*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/StateChanger.o: StateChanger.cc \
  ./YetAnother.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseUtility.h \
  ./TestHostState.h \
  ../../base/utils/HostState.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h \
  ./StateChanger.h \
  ./TestParam.h
$O/StateView.o: StateView.cc \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseUtility.h \
  ./TestHostState.h \
  ../../base/utils/HostState.h \
  ./StateView.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h \
  ./TestParam.h
$O/StateVisualize.o: StateVisualize.cc \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseUtility.h \
  ./TestHostState.h \
  ./StateVisualize.h \
  ../../base/utils/HostState.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h
$O/TestStateSub.o: TestStateSub.cc \
  ./TestStateSub.h \
  ../../base/modules/BaseModule.h \
  ../../base/modules/BaseUtility.h \
  ../../base/utils/HostState.h \
  ../../base/utils/FindModule.h \
  ../../base/utils/ImNotifiable.h \
  ../../base/utils/FWMath.h \
  ../../base/utils/Coord.h \
  ./TestParam.h
