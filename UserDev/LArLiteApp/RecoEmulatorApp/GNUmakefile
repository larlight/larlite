#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = LArLiteApp_RecoEmulatorAppDict
SHLIB = libLArLiteApp_RecoEmulatorApp.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config   --includes)
INCFLAGS += $(shell basictool-config --includes)
INCFLAGS += $(shell seltool-config   --includes)
# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

LDFLAGS += $(shell larlite-config   --libs)
LDFLAGS += $(shell basictool-config --libs)
LDFLAGS += $(shell seltool-config   --libs)
# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
