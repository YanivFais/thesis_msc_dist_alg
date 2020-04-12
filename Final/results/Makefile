
.PHONY : all gen_csv ini run

UNAME := $(shell uname)

ifneq ($(UNAME),Linux)
EXE := .exe
else
EXE :=  
endif

PARAMS?=params
CONFIG?=Main
OMNETINI?=omnetpp.ini

#$(warning $(UNAME),$(EXE),value)

all: ini run gen_csv

ifneq ($(GDB),)
PRE := gdb --args
endif

ifneq ($(VALGRIND),)
PRE := valgrind --log-file=valgrind.log --error-limit=no --read-var-info=yes
endif


TEST_EXIST=$(wildcard $(TEST)/$(PARAMS))
ifeq ($(TEST_EXIST),)
$(note Can't find test file $(TEST)/$(PARAMS), trying with relative path)
PREFIX := ../
else
endif

ifneq ($(COPY),)
POST := cp $(CONFIG).csv $(PREFIX)$(TEST) && cp $(CONFIG)_log.csv $(PREFIX)$(TEST) && cp ../IniGen/omnetpp.ini.include $(PREFIX)$(TEST)
endif


ini: 
	cd ../IniGen && bin/gen$(EXE) $(TEST)/$(PARAMS) $(TEST) $(PREFIX)

run:
	cd ../ && $(PRE) ./MiXiM$(EXE) -r 0 -c $(CONFIG) -u Cmdenv $(OMNETINI)

gen_csv:
	scavetool vector -O $(CONFIG).csv -F csv $(CONFIG)-0.vec ;
	perl -p -i.bak -e 's/sim\.channel\d+_node\[\d+\]\.appl\///g' $(CONFIG).csv ;
	perl -p -i.bak -e 's/sim\.channel\d+_node\[\d+\]\.mobility\///g' $(CONFIG).csv ;
	perl -p -i.bak -e 's/sim\.channel\d+_node\[\d+\]\.nic\.mac\///g' $(CONFIG).csv ;
	perl -p -i.bak -e 's/sim\.channel\d+_node\[\d+\]\.nic\.phy\///g' $(CONFIG).csv ;
	perl -p -i.bak -e 's/,,/,NaN,/g' $(CONFIG).csv ;
	perl -p -i.bak -e 's/,,/,NaN,/g' $(CONFIG).csv ;
	perl -p -i.bak -e 's/,$$/,NaN/g' $(CONFIG).csv ;
	$(POST)


