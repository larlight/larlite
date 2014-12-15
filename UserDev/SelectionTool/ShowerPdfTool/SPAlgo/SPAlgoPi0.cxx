#ifndef SELECTIONTOOL_SPALGOPI0_CXX
#define SELECTIONTOOL_SPALGOPI0_CXX

#include "SPAlgoPi0.h"

namespace sptool {

  SPAlgoPi0::SPAlgoPi0() : SPAlgoBase()
  {
    std::cout<<"Constructor"<<std::endl;
    _name     = "SPAlgoPi0";
    _xmin = 0;
    _xmax = 100;
    _lmin = 0;
    _lmax = 100;
    Reset();
    std::cout<<"Constructor end"<<std::endl;
  }

  void SPAlgoPi0::Reset()
  {
    std::cout<<"Calling "<<__FUNCTION__<<std::endl;
    _vars.clear();
    _vars.reserve(2);
    _vars.push_back(RooRealVar("_x","Distance [cm]",_xmin,_xmax));
    _vars.push_back(RooRealVar("_l","Radiation length [cm]",_lmin,_lmax));
    _pdf = _factory.RadLenPdf(_vars);
    std::cout<<"Called "<<__FUNCTION__<<std::endl;
  }

  SPArticleSet SPAlgoPi0::Select(const SPAData &data)
  { return SPArticleSet(); }

  void SPAlgoPi0::Fill(const SPAData &data)
  {}

}

#endif
