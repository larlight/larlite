#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = FANNCint
SHLIB = libRecoTool_FANN.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += -I./include
INCFLAGS += -I$(LARLITE_COREDIR)

# Add -p flag to CINT compilation
CINTFLAGS += -p

# linking for the fann packages:
LIBS += -Llib -lfann 

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE

pre_build:
	@python bin/fann_build.py

pre_clean:
	@python bin/fann_clean.py


