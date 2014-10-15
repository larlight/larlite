# Include your header file location
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/Base 
CXXFLAGS += -I$(LARLIGHT_CORE_DIR)/DataFormat
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(LARLIGHT_LIBDIR) -lDataFormat -lBase 
LDFLAGS += $(shell root-config --libs) -lPhysics -lMatrix -g

# platform-specific options
OSNAME = $(shell uname -s)
include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = simple_read simple_write test_simple_io gen_sample_wf

TARGETS = $(addsuffix .cc, $(PROGRAMS))
#all:		$(PROGRAMS)

all: $(PROGRAMS)

$(PROGRAMS): 
	@echo '<<compiling' $@'>>'
	@$(CXX) $@.cc -o $@ $(CXXFLAGS) $(LDFLAGS) 
	@rm -rf *.dSYM

clean:	
	rm -f $(PROGRAMS)
