#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = SelectionTool_OpT0FinderAnaDict
SHLIB = libSelectionTool_OpT0FinderAna.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += -L$(LARLITE_LIBDIR) -lSelectionTool_OpT0FinderBase -lSelectionTool_OpT0FinderAlgorithms
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE


