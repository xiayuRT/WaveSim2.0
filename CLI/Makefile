EXEC_NAME := "main"
INCLUDE_DIR := -I/external/inc-pub -I/inc
LIBS := -lsFoundation20 -lpthread -lpigpio
CC := g++
OPTIMIZATION := -O3
DEBUG_OPTIMIZATION := -O0
CXXFLAGS := -I$(INCLUDE_DIR) $(OPTIMIZATION)
DEBUG_CXXFLAGS := $(CXXFLAGS) -Wall -Wextra -pedantic -g3 $(DEBUG_OPTIMIZATION)

# Specify source files here
MAIN_SRC = src
SRC_FILES  = ../common/src
ALL_SRC_FILES := $(wildcard $(SRC_FILES)/*.cpp  $(MAIN_SRC)/*.cpp)
ALL_OBJS := $(patsubst %.cpp,%.o,$(ALL_SRC_FILES))


# Default target
all: main

# Generic rule to compile a CPP file into an object
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o "$@" $<

main: $(ALL_OBJS)
    ifneq ($(RPATH),)
	$(CC) -L$(RPATH) -Wl,-rpath=$(RPATH) -o $(EXEC_NAME) $(ALL_OBJS) $(LIBS)
    else
	$(CC) -o $(EXEC_NAME) $(ALL_OBJS) $(LIBS)
    endif

# Remove all object files
.PHONY: clean
clean:
	-find . -type f -name "*.o" -delete
	-find . -type f -name "*.txt" -delete

# Sayonara. Viciously destroys all build artifacts, including the executable.
.PHONY: real_clean
real_clean: clean
	-rm $(EXEC_NAME)
