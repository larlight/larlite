# Include your header file location
CXXFLAGS += $(shell root-config --cflags) -g
CXXFLAGS += $(shell larlite-config --includes)

# Include your shared object lib location
LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += $(shell root-config --libs) -lGeom -lPhysics -lMatrix -g

# platform-specific options
OSNAME = $(shell uname -s)
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.

all: ../bin/larlite ../bin/llhadd

../bin/larlite:
	@echo '<<compiling' $@'>>'
	@mkdir -p ../bin
	@$(CXX) larlite.cc -o ../bin/$@ $(CXXFLAGS) $(LDFLAGS)
	@rm -rf ../bin/*.dSYM

../bin/llhadd:
	@echo '<<compiling' $@'>>'
	@mkdir -p ../bin
	@$(CXX) llhadd.cc -o ../bin/$@ $(CXXFLAGS) $(LDFLAGS)
	@rm -rf ../bin/*.dSYM

clean:	
	rm -f ../bin/larlite
	rm -f ../bin/llhadd
