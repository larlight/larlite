#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = SelectionTool_ERAlgoDict
SHLIB = libSelectionTool_ERAlgo.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell basictool-config --includes)
INCFLAGS += $(shell seltool-config --includes)
INCFLAGS += -I$(LARLITE_USERDEVDIR)/BasicTool/TreeGraph

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# set linker flags
LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += -L$(shell root-config --libdir) -lEG -lRooFit -lRooFitCore -lRooStats
#LDFLAGS += $(shell seltool-config --libs)
LDFLAGS += $(shell basictool-config --libs)
LDFLAGS += -L$(LARLITE_LIBDIR) -lSelectionTool_ERBase
LDFLAGS += -L$(LARLITE_LIBDIR) -lTreeGraph_GeoGraph

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE





