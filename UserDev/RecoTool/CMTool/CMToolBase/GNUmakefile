#
# This is an example GNUmakefile for CMToolBase package
#

# specific names for this package
DICT  = CMToolBaseCint
SHLIB = libRecoTool_CMToolBase.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += -I$(LARLITE_COREDIR)
INCFLAGS += -I$(LARLITE_USERDEVDIR)/RecoTool
INCFLAGS += -I$(LARLITE_USERDEVDIR)/RecoTool/FANN/include
# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)


CINTFLAGS += -p
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE



