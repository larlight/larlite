#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = SelectionTool_RecoEmulatorDict
SHLIB = libSelectionTool_RecoEmulator.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell basictool-config --includes)

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)
CINTFLAGS += "-p" # Use system compiler to preprocess macros (something in Base is messing w/ rootcint)
LDFLAGS += $(shell basictool-config --libs)
# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
