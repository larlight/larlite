#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = BasicTool_LArPyBackendDict
SHLIB = libBasicTool_LArPyBackend.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell basictool-config --includes)
INCFLAGS += $(shell python-config --includes)

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# Use build compiler to pre-process macros instead of rootcint
CINTFLAGS += -p

LDFLAGS += -L$(shell python-config --prefix)/lib 
LDFLAGS += $(shell python-config --libs)
LDFLAGS += -L$(LARLITE_LIBDIR) -llarcoreobj_GeoAlgo  # THIS NEEDS TO BE CHANGED TO AUTOMATICALLY LOOK FOR ENVIRONMENT VARIABLE $LARCOREOBJ_LIB

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE



