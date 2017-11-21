////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////

#ifndef LARLITE_BACKTRACKERMATCHINGDATA_CXX
#define LARLITE_BACKTRACKERMATCHINGDATA_CXX

#include "backtrackermatchingdata.h"

namespace larlite {

  //----------------------------------------------------------------------

  void backtrackermatchingdata::clear_data(){
    fcleanliness = 0 ;
    fcompleteness = 0 ;
    fideFraction = 0 ;
    fisMaxIDE = 0 ;
    fideNFraction = 0 ;
    fisMaxIDEN = 0 ;
    fnumElectrons = 0 ;
    fenergy = 0 ;    
  }

  backtrackermatchingdata::backtrackermatchingdata(  double Cleanliness,     
						     double Completeness,     
						     float  IdeFraction,     
						     int    IsMaxIDE,     
						     float  IdeNFraction,     
						     int    IsMaxIDEN,     
						     float  NumElectrons,     
						     float  Energy) 
  {
    fcleanliness = Cleanliness;
    fcompleteness = Completeness;
    fideFraction = IdeFraction;
    fisMaxIDE = IsMaxIDE;
    fideNFraction = IdeNFraction;
    fisMaxIDEN = IsMaxIDEN;
    fnumElectrons = NumElectrons;
    fenergy = Energy;
  }

  
  double backtrackermatchingdata::cleanliness()  const{
    return fcleanliness;}
  double backtrackermatchingdata::completeness()  const{ 
    return fcompleteness;}
  float backtrackermatchingdata::ideFraction()  const{ 
    return fideFraction;}
  int backtrackermatchingdata::isMaxIDE()  const{ 
    return fisMaxIDE;}
  float backtrackermatchingdata::ideNFraction()  const{ 
    return fideNFraction;}
  int backtrackermatchingdata::isMaxIDEN()  const{ 
    return fisMaxIDEN;}
  float backtrackermatchingdata::numElectrons()  const{ 
    return fnumElectrons;}
  float backtrackermatchingdata::energy()  const{ 
    return fenergy;}
}

#endif
