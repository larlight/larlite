#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = BasicTool_GeoAlgoDict
SHLIB = libBasicTool_GeoAlgo.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# set CINT flag
CINTFLAGS += -p

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE



