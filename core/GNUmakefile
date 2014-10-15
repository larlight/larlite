#####################################################################################
#
# A top Makefile for building my project.
# One needs to define $LARLITE_COREDIR environment variable and set it to where this
# makefile exists. 
# One can type "make" and this builds packages that are added in $SUBDIR defined below.
# 
# The original is taken from Glenn A. Smith's example for Double Chooz experiment.
#
#####################################################################################
#
# IMPOSE CONDITION BETWEEN LARLITE_COREDIR & PWD =>
#   do not compile if PWD !=$LARLITE_COREDIR is set elsewhere
#
ifndef LARLITE_COREDIR
 LARLITE_COREDIR := $(shell cd . && pwd -P)
endif
NORMALIZED_LARLITE_COREDIR := $(shell cd ${LARLITE_COREDIR} && pwd -P)
ifneq ($(NORMALIZED_LARLITE_COREDIR), $(shell cd . && pwd -P))
 ifeq ($(wildcard ./Base/*),)
ERROR_MESSAGE := $(error Your source code detected in current dir, but LARLITE_COREDIR is not current dir.  \
   To avoid recompiling the wrong installation,\
   LARLITE_COREDIR must be set to the current directory when making.  \
   Currently it is set to ${LARLITE_COREDIR} [$(NORMALIZED_LARLITE_COREDIR)].  \
   Current directory is [$(shell pwd)].)
 endif
endif
export LARLITE_COREDIR
#
#####################################################################################
#
# Define directories to be compile upon a global "make"...
#
SUBDIRS :=  Base LArUtil DataFormat Analysis

#####################################################################################
#
# COMPILATION...
#
#.phony: all configure default-config clean
.phony: all clean

all:
	@for i in $(SUBDIRS); do ( echo "" && echo "Compiling $$i..." && cd $$i && $(MAKE) ) || exit $$?; done
#####################################################################################
#
# CLEANs...
#
clean:
	@for i in $(SUBDIRS); do ( echo "" && echo "Cleaning $$i..." && cd $$i && $(MAKE) clean && rm -f $(LARLITE_LIBDIR)/$$i.* ) || exit $$?; done

#####################################################################################
#
# DOCUMENTATION...
#
doxygen:
	@echo 'dOxygenising your code...'
	@mkdir -p $(LARLITE_COREDIR)/doc/dOxygenMyProject
	@doxygen $(LARLITE_COREDIR)/doc/doxygenMyProject.script

doxygen+:
	@echo 'dOxygenising MyProject + local-ROOT...'
	@mkdir -p $(LARLITE_COREDIR)/doc/dOxygenMyProject+
	@doxygen $(LARLITE_COREDIR)/doc/doxygenMyProject+.script
#
#####################################################################################
#EOF
