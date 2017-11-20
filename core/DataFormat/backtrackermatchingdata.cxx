////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////

#ifndef LARLITE_BACKTRACKERMATCHINGDATA_CXX
#define LARLITE_BACKTRACKERMATCHINGDATA_CXX

#include "backtrackermatchingdata.h"

namespace larlite {

  //----------------------------------------------------------------------

  void backtrackermatchingdata::clear_data(){
    kcleanliness = 0 ;
    kcompleteness = 0 ;
    kideFraction = 0 ;
    kisMaxIDE = 0 ;
    kideNFraction = 0 ;
    kisMaxIDEN = 0 ;
    knumElectrons = 0 ;
    kenergy = 0 ;    
  }

  backtrackermatchingdata::backtrackermatchingdata(  double cleanliness,     
						     double completeness,     
						     float       ideFraction,     
						     int        isMaxIDE,     
						     float       ideNFraction,     
						     int         isMaxIDEN,     
						     float       numElectrons,     
						     float       energy) 
  {
    kcleanliness = cleanliness;
    kcompleteness = completeness;
    kideFraction = ideFraction;
    kisMaxIDE = isMaxIDE;
    kideNFraction = ideNFraction;
    kisMaxIDEN = isMaxIDEN;
    knumElectrons = numElectrons;
    kenergy = energy;
  }

  
  double backtrackermatchingdata::cleanliness()  const{
    return kcleanliness;}
  double backtrackermatchingdata::completeness()  const{ 
    return kcompleteness;}
  float backtrackermatchingdata::ideFraction()  const{ 
    return kideFraction;}
  int backtrackermatchingdata::isMaxIDE()  const{ 
    return kisMaxIDE;}
  float backtrackermatchingdata::ideNFraction()  const{ 
    return kideNFraction;}
  int backtrackermatchingdata::isMaxIDEN()  const{ 
    return kisMaxIDEN;}
  float backtrackermatchingdata::numElectrons()  const{ 
    return knumElectrons;}
  float backtrackermatchingdata::energy()  const{ 
    return kenergy;}
}

#endif
