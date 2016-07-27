// We need this because gmake fails if there are no
// source files and at the moment I created this file
// there were no source files other than the one implementing
// this function. There is probably some way to modify
// the GNUmakefile to make this unnecessary ...
namespace larlite {
  void dummyWrappedLarSoftDictionaries();
}
