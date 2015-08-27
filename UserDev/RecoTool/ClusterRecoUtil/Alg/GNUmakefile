#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = RecoTool_ClusterRecoUtilAlgDict
SHLIB = libRecoTool_ClusterRecoUtilAlg.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                      #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell recotool-config --includes)
INCFLAGS += -I.. #Include one layer up

# Add -p flag to CINT 
CINTFLAGS += -p

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# linker flags
LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_FANN
# LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_ClusterRecoUtil
LDFLAGS += -L$(LARLITE_LIBDIR) -lfloatfann


include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE





