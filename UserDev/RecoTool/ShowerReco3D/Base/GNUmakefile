#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = RecoTool_ShowerRecoBaseDict
SHLIB = libRecoTool_ShowerRecoBase.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS = -I.                       #Include itself
INCFLAGS += $(shell basictool-config --includes)
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell recotool-config --includes)

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# CINT flag
CINTFLAGS += "-p"

LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += $(shell basictool-config --libs)
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_AnalysisAlg
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_MCComp
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_CMTAlgMatch
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_CMToolBase
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_ClusterRecoUtil
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_ClusterRecoUtilAlg
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_Cluster3DRecoUtil
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_Cluster3DRecoUtilAlg

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE



