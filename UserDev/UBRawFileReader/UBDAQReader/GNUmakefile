#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = UBRawFileReader_UBDAQReaderCint
SHLIB = libUBRawFileReader_UBDAQReader.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += -I$(shell pg_config --includedir)
INCFLAGS += -I${BOOST_INC}
INCFLAGS += -I${UBOONEDAQ_DATATYPES_INC}
INCFLAGS += -I$(LARLITE_USERDEVDIR)
INCFLAGS += $(shell python-config --includes)
INCFLAGS += -I$(shell python -c "import numpy; print numpy.get_include()")

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

LDFLAGS += $(shell larlite-config --libs) -lDisplayTool_RawViewer
LDFLAGS += -L$(shell pg_config --libdir) -lpq
LDFLAGS += -L${BOOST_LIB} -lboost_serialization
LDFLAGS += -L${UBOONEDAQ_DATATYPES_LIB} -lubdata_types

CINTFLAGS += "-p"
# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}
# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
