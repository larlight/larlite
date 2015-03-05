#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = CosmicTagTool_MCTaggerDict
SHLIB = libCosmicTagTool_MCTagger.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell basictool-config --includes)

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# set CINT flag
CINTFLAGS += -p

# set linker flags
LDFLAGS += $(shell basictool-config --libs)
LDFLAGS += $(shell larlite-config --libs)

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE



