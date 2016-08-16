#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = TreeGraph_GeoGraphCint
SHLIB = libTreeGraph_GeoGraph.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell basictool-config --includes)
INCFLAGS += -I$(LARLITE_USERDEVDIR)/larcoreobj/larcoreobj

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

LDFLAGS += -L$(LARLITE_LIBDIR) -llarcoreobj_GeoAlgo # THIS NEEDS TO BE CHANGED TO AUTOMATICALLY LOOK FOR ENVIRONMENT VARIABLE $LARCOREOBJ_LIB

# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
