
CXX := g++

CFLAGS := -Wall -ggdb3  

#-fstack-protector-all

SRCS := $(wildcard c/*.cpp) 

OBJ_DIR:= obj

OBJS := $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS)))
OBJS := $(OBJS:.cpp=.o)


INCLUDES := -Iinclude  

LFLAGS := 

LIBS := 

all: bin/CountTable

.PHONY: run
run: bin/CountTable 
	bin/gen params1.txt

$(OBJ_DIR):
	-mkdir -p $(OBJ_DIR)


$(OBJS): $(OBJ_DIR)/%.o: c/%.cpp 
	$(CXX) $(CFLAGS) $(INCLUDES) -MD -MP -c -o $@ $<


bin/CountTable: $(OBJ_DIR)  $(OBJS) 
	$(CXX) $(CFLAGS) $(LFLAGS) -o $@ $(OBJS) $(LIBS)


.PHONY: clean
clean:
	-rm -rf \
		$(OBJS) \
		$(OBJ_DIR) \

# used for printing variables , like print-SOURCE_FILES
print-%: ; @$(warning $* is $($*) ($(value $*)) (from $(origin $*)))

-include $(OBJ_DIR)/*.d
