#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = LArLite_DataFormatDict
SHLIB = libLArLite_DataFormat.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I. 
INCFLAGS += -I$(LARLITE_COREDIR)

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# set linker flags
LDFLAGS += -L$(LARLITE_LIBDIR) -lLArLite_LArUtil
LDFLAGS += -L$(LARLITE_LIBDIR) -lLArLite_Base

CINTFLAGS += -p

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
