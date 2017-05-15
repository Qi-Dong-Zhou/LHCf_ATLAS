#####################################################################
#------------------------------------------------
#
# Configuration for each architecture
#

MAKEARCH        :=      $(shell find $(ROOTSYS)/test -name Makefile.arch)
ifeq ($(MAKEARCH), )
# 41594 or later
MAKEARCH        :=      $(shell find $(ROOTSYS)/etc -name Makefile.arch)
endif

include $(MAKEARCH)

ROOT_CFLAGS=`root-config --cflags`
ROOT_LFLAGS=`root-config --glibs`

CXX=g++
CXXFLAGS=-g -Wall -std=c++11 $(ROOT_CFLAGS)





INCALL += -I./include/ 
CXXFLAGS +=  $(INCALL) -g -fopenmp

BINDIR=./bin

TARGETS  = LHCf_ATLAS 

all: $(TARGETS)	

LHCf_ATLAS: src/LHCf_ATLAS.o ./include/LHCfData.cpp \
			 ./include/ATLASData.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^ \
	$(LIBS) $(ROOTLIBS) $(ROOTGLIBS)  

 
%.o	:   %.cpp %.h
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) $(OSFLAGS) -c $< -o $@

src/%.o  :  src/%.cpp  src/%.h
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) $(OSFLAGS) -c $< -o $@

src/%.o  :  src/%.cpp
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) $(OSFLAGS) -c $< -o $@

$(SRC_DIR)/%.o  :  $(SRC_DIR)/%.cpp  $(INC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) $(OSFLAGS) -c $< -o $@

clean:
	rm -f *.o */*.o */*~ *~ \#*
	rm -f $(BINDIR)/LHCf_ATLAS



