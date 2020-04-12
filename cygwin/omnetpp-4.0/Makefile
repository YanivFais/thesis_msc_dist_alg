#
# Toplevel makefile for OMNeT++ libraries and programs
#
#=====================================================================

#=====================================================================
#
# Main targets
#
#=====================================================================

ifeq ("$(MODE)","")
all: allmodes
else
all: components
endif

allmodes:
	$(MAKE) MODE=release
	$(MAKE) MODE=debug
	@echo
	@echo "Now you can type \"`echo $(OMNETPP_RELEASE) | sed 's/-.*//'`\" to start the IDE"

components: base samples

#=====================================================================
#
# Includes and basic checks
#
#=====================================================================

include Makefile.inc

# Make sure that output locations are set
ifeq ("$(strip $(OMNETPP_BIN_DIR))","")
$(error OMNETPP_BIN_DIR must be correctly set)
endif
ifeq ("$(strip $(OMNETPP_OUT_DIR))","")
$(error OMNETPP_OUT_DIR must be correctly set)
endif
ifeq ("$(strip $(OMNETPP_LIB_DIR))","")
$(error OMNETPP_LIB_DIR must be correctly set)
endif
#=====================================================================
#
# OMNeT++ components
#
#=====================================================================

BASE=common layout eventlog scave nedxml sim envir cmdenv tkenv utils
SAMPLES=aloha cqn dyna embedding embedding2 fifo hypercube histograms neddemo queueinglib queueinglibext routing tictoc sockets
JNILIBS=org.omnetpp.ned.model org.omnetpp.ide.nativelibs

#
# Group targets.
#
base: $(BASE)
	cd $(OMNETPP_SRC_DIR)/envir && $(MAKE) opp_run_executable

ui : common layout eventlog scave nedxml $(JNILIBS)
samples: $(SAMPLES)

# dependencies (because of ver.h, opp_msgc, etc)
clean depend: makefiles
common layout eventlog scave nedxml sim envir cmdenv tkenv makefiles: utils
layout eventlog scave nedxml sim envir cmdenv tkenv : common
envir : sim
tkenv cmdenv : envir
tkenv : layout
sim : nedxml common
$(SAMPLES) : makefiles base
$(BASE) : check-env
queueinglibext : queueinglib

.PHONY: check-env cleanall depend makefiles clean apis docu tests all allmodes \
        components base ui samples common layout eventlog scave nedxml sim \
        envir cmdenv tkenv utils

#
# Core libraries and programs
#
$(BASE):
	@echo ===== Compiling $@ ====
	cd $(OMNETPP_SRC_DIR)/$@ && $(MAKE)

#
# Native libs for the UI
#
$(JNILIBS): nedxml
	@echo ===== Compiling $@ ====
	cd $(OMNETPP_UI_DIR)/$@ && $(MAKE) clean
	cd $(OMNETPP_UI_DIR)/$@ && $(MAKE)

#
# Sample programs
#

$(SAMPLES):
	@echo ===== Compiling $@ ====
	cd $(OMNETPP_SAMPLES_DIR)/$@ && $(MAKE)


#
# Documentation
#
apis:
	cd $(OMNETPP_DOC_DIR)/src && $(MAKE) apis

docu:
	cd $(OMNETPP_DOC_DIR)/src && $(MAKE)

#
# Test
#
tests: base
	cd $(OMNETPP_TEST_DIR) && $(MAKE)

#=====================================================================
#
# Utilities
#
#=====================================================================

check-env:
	@echo "***** Configuration: MODE=$(MODE), TOOLCHAIN_NAME=$(TOOLCHAIN_NAME), LIB_SUFFIX=$(LIB_SUFFIX) ****"
	@echo ===== Checking environment =====
	@probefile=__probe__; \
	if (echo '#!/bin/sh' >$(OMNETPP_BIN_DIR)/$$probefile && \
	    chmod +x $(OMNETPP_BIN_DIR)/$$probefile) 2>/dev/null; then \
	  if $$probefile >/dev/null 2>/dev/null; then :; else \
	    echo 'ERROR: $(OMNETPP_BIN_DIR) is not in the path! You can add it by entering:'; \
	    echo '   export PATH=$(OMNETPP_BIN_DIR):$$PATH'; \
	    exit 1; \
	  fi; \
	else \
	  echo 'ERROR: Cannot write to $(OMNETPP_BIN_DIR)! Please make sure it exists, and has write permission.'; \
	  exit 1; \
	fi; \
	rm -f $(OMNETPP_BIN_DIR)/$$probefile; \

clean: makefiles
	-rm -f $(OMNETPP_LIB_DIR)/*.*
	-rm -rf $(OMNETPP_OUT_DIR)/$(CONFIGNAME)
	-rm -rf $(OMNETPP_LIB_DIR)/$(CONFIGNAME)
	for i in $(BASE); do \
	    (cd $(OMNETPP_SRC_DIR)/$$i && $(MAKE) clean); \
	done
	for i in $(SAMPLES) ""; do \
	    if [ "$$i" != "" ]; then (cd $(OMNETPP_SAMPLES_DIR)/$$i && $(MAKE) clean); fi;\
	done
	cd $(OMNETPP_TEST_DIR) && $(MAKE) clean
	-rm -f $(OMNETPP_BIN_DIR)/*

cleanall: makefiles
	-rm -rf $(OMNETPP_OUT_DIR)
	-rm -rf $(OMNETPP_LIB_DIR)/*
	for i in $(BASE); do \
	    (cd $(OMNETPP_SRC_DIR)/$$i && $(MAKE) clean); \
	done
	for i in $(SAMPLES) ""; do \
	    if [ "$$i" != "" ]; then (cd $(OMNETPP_SAMPLES_DIR)/$$i && $(MAKE) cleanall); fi;\
	done
	cd $(OMNETPP_TEST_DIR) && $(MAKE) clean
# bin should be removed last because opp_configfilepath (in bin directory) is needed to clean
	-rm -rf $(OMNETPP_BIN_DIR)/*


depend:
	for i in $(BASE); do \
	    (cd $(OMNETPP_SRC_DIR)/$$i && $(MAKE) depend); \
	done
	for i in $(SAMPLES) ""; do \
	    if [ "$$i" != "" ]; then (cd $(OMNETPP_SAMPLES_DIR)/$$i && $(MAKE) depend); fi;\
	done

makefiles:
	for i in $(SAMPLES) ""; do \
	    if [ "$$i" != "" ]; then (cd $(OMNETPP_SAMPLES_DIR)/$$i && (opp_makemake -f --deep)); fi;\
	done
	(cd $(OMNETPP_SAMPLES_DIR)/embedding && (opp_makemake -f --deep --nolink))
	(cd $(OMNETPP_SAMPLES_DIR)/embedding2 && (opp_makemake -f --deep --nolink))
	(cd $(OMNETPP_SAMPLES_DIR)/queueinglib && (opp_makemake -f --make-so))
	(cd $(OMNETPP_SAMPLES_DIR)/queueinglibext && (opp_makemake -f --make-so -I../queueinglib -L../queueinglib/out/$(CONFIGNAME) -lqueueinglib -KQUEUEINGLIB_PROJ=../queueinglib))
	(cd $(OMNETPP_SAMPLES_DIR)/queuenet && (opp_makemake -f -n))

# copy the documentation to the UI doc folder too.
# patch some files to have correct URLs and add generic eclipse stylsheet when needed
copy-ui-docu:
	txt2html $(OMNETPP_DOC_DIR)/WhatsNew --outfile $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/WhatsNew.html
	rm -rf $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/api
	rm -rf $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/manual
	rm -rf $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/userguide
	rm -rf $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/migration
	rm -rf $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/tictoc-tutorial
	rm -rf $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/ide-overview
	cp -r $(OMNETPP_DOC_DIR)/api $(OMNETPP_UI_DIR)/org.omnetpp.doc/content
	cp -r $(OMNETPP_DOC_DIR)/src/manual/eclipse $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/manual
	cp -r $(OMNETPP_DOC_DIR)/src/userguide/eclipse $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/userguide
	cp -r $(OMNETPP_DOC_DIR)/src/migrationguide/eclipse $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/migration
	cp -r $(OMNETPP_DOC_DIR)/tictoc-tutorial $(OMNETPP_UI_DIR)/org.omnetpp.doc/content
	cp -r $(OMNETPP_DOC_DIR)/ide-overview $(OMNETPP_UI_DIR)/org.omnetpp.doc/content
	echo "<html><body><pre>" >$(OMNETPP_UI_DIR)/org.omnetpp.doc/content/License.html
	cat $(OMNETPP_DOC_DIR)/License >>$(OMNETPP_UI_DIR)/org.omnetpp.doc/content/License.html
	echo "</pre></body></html>" >>$(OMNETPP_UI_DIR)/org.omnetpp.doc/content/License.html
	rm -rf $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/userguide/plugin.xml
	rm -rf $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/migration/plugin.xml
	perl -i -pe 's!href="!href="content/manual/!gi' $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/manual/toc.xml
	perl -i -pe 's!href="!href="content/userguide/!gi' $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/userguide/toc.xml
	perl -i -pe 's!<head>!<head><link rel="STYLESHEET" href="../book.css"  type="text/css"/>!gi' $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/userguide/*.html
	perl -i -pe 's!href="!href="content/migration/!gi' $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/migration/toc.xml
	perl -i -pe 's!<head>!<head><link rel="STYLESHEET" href="../book.css"  type="text/css"/>!gi' $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/migration/*.html
	perl -i -pe 's!<head>!<head><link rel="STYLESHEET" href="book.css"  type="text/css"/>!gi' $(OMNETPP_UI_DIR)/org.omnetpp.doc/content/WhatsNew.html

# utility target to copy 3rd party DLLs to the bin directory on MINGW build
copy-dlls:
	cp mingw/bin/libgcc_sjlj_1.dll bin
	cp mingw/bin/libstdc++_sjlj_6.dll bin
	cp msys/bin/tcl84.dll bin
	cp msys/bin/tclpip84.dll bin
	cp msys/bin/tk84.dll bin
	cp msys/bin/BLT24.dll bin
	cp msys/bin/BLTlite24.dll bin
	cp msys/bin/zlib1.dll bin
	cp msys/bin/libxml2.dll bin


