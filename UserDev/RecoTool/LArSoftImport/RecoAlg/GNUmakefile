#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = RecoTool_LArSoftImport_RecoAlgCint
SHLIB = libRecoTool_LArSoftImport_RecoAlg.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes) #larlite
INCFLAGS += $(shell basictool-config --includes)

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

#LDFLAGS += -lMinuit2
LDFLAGS += -L$(shell root-config --libdir)
LDFLAGS += $(shell basictool-config --libs)
LDFLAGS += $(shell larlite-config --libs)
# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
