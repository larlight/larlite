
OSNAME = $(shell uname -s)
include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

LD := $(subst build/,$(ROOTSYS)/build/,$(LD))
LDFLAGS  += -L$(ROOTSYS)/lib  $(shell root-config --libs) -g

# Include your header file location
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/Base 
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/DataFormat
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/Analysis
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/LArUtil
CXXFLAGS += -I$(USER_DEV_DIR)/FANN
CXXFLAGS += -I$(USER_DEV_DIR)/FANN/include
CXXFLAGS += -I$(USER_DEV_DIR)/ClusterRecoUtil
CXXFLAGS += -I$(USER_DEV_DIR)/CMTool
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(LARLIGHT_LIBDIR) -lCMTool -lClusterRecoUtil -lFANN -lAnalysis -lDataFormat -lLArUtil -lBase -lHist -lGpad 

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = run_merge_viewer test_merge

all:		$(PROGRAMS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)
