# Include your header file location
CXXFLAGS += -I$(LARLITE_USERDEVDIR)/SelectionTool
CXXFLAGS += -I. -I./.. -I./../algosrc $(shell root-config --cflags) -g
CXXFLAGS += $(shell larlite-config --includes)
CXXFLAGS += -I$(LARLITE_USERDEVDIR)/BasicTool

# Include your shared object lib location
LDFLAGS += -L$(LARLITE_LIBDIR) -lSelectionTool_LEEPreCuts
LDFLAGS += -L$(LARLITE_LIBDIR) -lBasicTool_FhiclLite
LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += $(shell root-config --libs) -lPhysics -lMatrix -g

# platform-specific options
OSNAME = $(shell uname -s)
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = example

all:		$(PROGRAMS)

$(PROGRAMS):
	@echo '<<compiling' $@'>>'
	@$(CXX) $@.cc -o $@ $(CXXFLAGS) $(LDFLAGS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)
