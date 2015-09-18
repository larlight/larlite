#
# This is an example GNUmakefile for OpT0Finder package
#

#SUBDIRS := Base Filter Algo App
SUBDIRS := Base PhotonLibrary Algorithms App Ana

.phony: all clean

all:
	@for i in $(SUBDIRS); do ( echo "" && echo "Compiling $$i..." && cd $$i && $(MAKE) ) || exit $$?; done

clean:
	@for i in $(SUBDIRS); do ( echo "" && echo "Cleaning $$i..." && cd $$i && $(MAKE) clean && rm -f $(LARLIGHT_LIBDIR)/$$i.* ) || exit $$?; done

doxygen:
	@doxygen doc/doxygenMyProject.script


