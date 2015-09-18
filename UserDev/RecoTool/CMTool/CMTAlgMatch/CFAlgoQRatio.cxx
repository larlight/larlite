#ifndef RECOTOOL_CFALGOQRATIO_CXX
#define RECOTOOL_CFALGOQRATIO_CXX

#include "CFAlgoQRatio.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoQRatio::CFAlgoQRatio() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _qratio_cut = 0.1 ;// Preliminary cuts
  
  }

  //-----------------------------
  void CFAlgoQRatio::Reset()
  //-----------------------------
  {

  }

  //-----------------------------------------------------------------------------------------
  float CFAlgoQRatio::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //-----------------------------------------------------------------------------------------
  {

    // Code-block by Kazu starts
    // This ensures the algorithm works only if # clusters is > 2 (and not =2)
    // You may take out this block if you want to allow matching using clusters from only 2 planes.
    if(clusters.size()==2) return -1;
    // Code-block by Kazu ends
    
    double q_max = -1;
    float ratio = 1;

    // Search for max charge & return if any cluster has negative charge
    for(auto const& c : clusters) {

      //Ignore all clusters with less than 40 hits 

      if(c->sum_charge < 0) {
        if(_verbose) 
          std::cout 
            << "\033[00m Found a cluster with negative charge!\033[00m ... aborting " 
            << __FUNCTION__ <<std::endl;
        return -1;
      }


      if(q_max < c->sum_charge)
        q_max = c->sum_charge;

   }

    // Compute ratio
    for(auto const& c : clusters){
          ratio *= c->sum_charge / q_max;
  
      if(_verbose){
        std::cout<<"Sum Charge: "<<c->sum_charge ;
        std::cout<<"Q max : "<<q_max<<std::endl ;
        std::cout<<"Ratio is: "<<ratio<<std::endl;  
        }
  }
  
         if(_verbose) {
            if( ratio > _qratio_cut ) std::cout << " ... pass!" << std::endl;
           else std::cout << " ... below cut value: " << _qratio_cut << std::endl;
        }

     return ( ratio > _qratio_cut ? ratio : -1 );
 }
  

  //------------------------------
  void CFAlgoQRatio::Report()
  //------------------------------
  {

  }
    
}
#endif
