#
# This is an example GNUmakefile for CMToolBase package
#

# specific names for this package
DICT  = CMToolAppCint
SHLIB = libRecoTool_CMToolApp.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell recotool-config --includes)
INCFLAGS += -I$(LARLITE_USERDEVDIR)/RecoTool/ClusterRecoUtil/Alg

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

CINTFLAGS += -p

LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_CMTAlgMerge
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_CMTAlgMatch
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_CMTAlgPriority
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_CMToolBase
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_ClusterRecoUtil
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_ClusterRecoUtilAlg
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_Cluster3DRecoUtil
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_Cluster3DRecoUtilAlg

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE



