#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = DisplayTool_RecoViewerCint
SHLIB = libDisplayTool_RecoViewer.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell recotool-config --includes)
INCFLAGS += $(shell python-config --includes)
INCFLAGS += -I$(shell python -c "import numpy; print numpy.get_include()")

# Add -p flag to CINT compilation
CINTFLAGS += -p

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += $(shell recotool-config --libs)
LDFLAGS += -L$(LARLITE_BASEDIR)/lib -lRecoTool_ClusterRecoUtil
LDFLAGS += -L$(LARLITE_BASEDIR)/lib -lRecoTool_CMToolBase
LDFLAGS += -L$(LARLITE_BASEDIR)/lib -lRecoTool_CMTAlgPriority
LDFLAGS += -L$(LARLITE_BASEDIR)/lib -lRecoTool_CMTAlgMatch
LDFLAGS += -L$(LARLITE_LIBDIR) -lRecoTool_ClusterRecoUtilAlg
LDFLAGS += -L.
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE


