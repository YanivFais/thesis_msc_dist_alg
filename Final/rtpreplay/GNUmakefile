
CXX := g++
CC := gcc

CFLAGS := -Wall -ggdb3  

#-fstack-protector-all

SRCS := rtpreplay.cpp rd.c host2ip.c hpt.c multimer.c notify.c RtpData.cpp yplib.c 
#$(wildcard *.cpp) $(wildcar *.c) 

OBJ_DIR:= obj

OBJS := $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS)))
OBJS := $(filter %.o,$(OBJS:.cpp=.o)) $(filter %.o,$(OBJS:.c=.o))


INCLUDES := -I.  

LFLAGS := 

LIBS := 
#-lnsl 

all: bin/rtpreplay

$(OBJ_DIR):
	-mkdir -p $(OBJ_DIR)


$(OBJ_DIR)/%.o: %.c  
	$(CC) $(CFLAGS) $(INCLUDES) -MD -MP -c -o $@ $<

$(OBJ_DIR)/%.o: %.cpp  
	$(CXX) $(CFLAGS) $(INCLUDES) -MD -MP -c -o $@ $<


bin/rtpreplay: $(OBJ_DIR)  $(OBJS) 
	$(CXX) $(CFLAGS) $(LFLAGS) -o $@ $(OBJS) $(LIBS)


.PHONY: clean
clean:
	-rm -rf \
		$(OBJS) \
		$(OBJ_DIR) \

# used for printing variables , like print-SOURCE_FILES
print-%: ; @$(warning $* is $($*) ($(value $*)) (from $(origin $*)))

-include $(OBJ_DIR)/*.d
