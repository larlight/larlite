#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = BasicTool_LArPyBackendDict
SHLIB = libBasicTool_LArPyBackend.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell basictool-config --includes)
ifeq ($(LARLITE_PYTHON_VERSION),2)
INCFLAGS += $(shell python-config --includes)
INCFLAGS += -DLARLITE_PY2
else ifeq ($(LARLITE_PYTHON_VERSION),3)
INCFLAGS += $(shell python3-config --includes)
INCFLAGS += -DLARLITE_PY3
endif

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# Use build compiler to pre-process macros instead of rootcint
CINTFLAGS += -p

ifeq ($(LARLITE_PYTHON_VERSION),2)
LDFLAGS += -L$(shell python-config --prefix)/lib 
LDFLAGS += $(shell python-config --libs)
else ifeq ($(LARLITE_PYTHON_VERSION),3)
LDFLAGS += -L$(shell python3-config --prefix)/lib 
LDFLAGS += $(shell python3-config --libs)
endif
LDFLAGS += -L$(LARLITE_LIBDIR) -lBasicTool_GeoAlgo

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE



