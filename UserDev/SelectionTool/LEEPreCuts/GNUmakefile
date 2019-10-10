#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = SelectionTool_LEEPreCutsDict
SHLIB = libSelectionTool_LEEPreCuts.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
ALGOSRC = $(wildcard algosrc/*.cxx)
ALGOOBJ = $(notdir $(ALGOSRC))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)
OBJECTS += $(ALGOOBJ:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)
INCFLAGS += $(shell larlite-config --includes)/../UserDev/BasicTool

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)


# call kernel specific compiler setup
include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
LDFLAGS += $(shell larlite-config --libs) -lBasicTool_FhiclLite
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
#
algosrc/LEEPreCutAlgo.h:
	@echo '<<init submodule>>'
	@git submodule init
	@git submodule update

%.o: algosrc/%.cxx
	@echo '<< compiling' $< '>>'
	@$(CXX) $(CXXFLAGS) $(LARLITE_BASIC_ROOTINC) $(INCFLAGS) -c $<

