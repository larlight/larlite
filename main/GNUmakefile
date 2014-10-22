# Include your header file location
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(LARLITE_LIBDIR) -lLArLite_Base  -lLArLite_DataFormat -lLArLite_LArUtil -lLArLite_Analysis
LDFLAGS += $(shell root-config --libs) -lGeom -lPhysics -lMatrix -g

# platform-specific options
OSNAME = $(shell uname -s)
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.

all: ../bin/larlite

../bin/larlite:
	@echo '<<compiling' $@'>>'
	@mkdir -p ../bin
	$(CXX) larlite.cc -o ../bin/$@ $(CXXFLAGS) $(LDFLAGS)
	@rm -rf ../bin/*.dSYM

clean:	
	rm -f ../bin/larlite
