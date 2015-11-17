#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = SelectionTool_OpT0FinderAlgorithmsDict
SHLIB = libSelectionTool_OpT0FinderAlgorithms.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell basictool-config --includes)
INCFLAGS += -I$(LARLITE_USERDEVDIR)/SelectionTool

LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += $(shell basictool-config --libs) 
LDFLAGS += -L$(LARLITE_LIBDIR) -lSelectionTool_OpT0FinderBase
LDFLAGS += -L$(LARLITE_LIBDIR) -lSelectionTool_OpT0PhotonLibrary
LDFLAGS += -L$(shell root-config --libdir) -lMinuit #-lMinuit2

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
