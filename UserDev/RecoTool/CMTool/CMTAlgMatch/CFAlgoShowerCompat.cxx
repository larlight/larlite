#ifndef CFALGOSHOWERCOMPAT_CXX
#define CFALGOSHOWERCOMPAT_CXX

#include "CFAlgoShowerCompat.h"
#include "TTree.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoShowerCompat::CFAlgoShowerCompat() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    /*
    _fout_hax = 0; 
    _ana_tree = 0;

    if(!_fout_hax)
      _fout_hax = new TFile("fout_hax.root","RECREATE");

    
    if(!_ana_tree){
      _ana_tree = new TTree("ana_tree","ana_tree");
      _ana_tree->Branch("o_ang_avg",&_o_ang_avg,"o_ang_avg/D");
      _ana_tree->Branch("o_ang_rms",&_o_ang_rms,"o_ang_rms/D");
    }
    */
  }

  //-----------------------------
  void CFAlgoShowerCompat::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoShowerCompat::Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
    
    _o_ang_avg = 0;
    _o_ang_rms = 0;
    //    std::cout<<" ---- Begin of CFAlgoShowerCompat::Float ---- "<<std::endl;
    for(auto const& c : clusters){
      //      PrintClusterInfo(*c);
      _o_ang_avg += c->GetParams().opening_angle;
      _o_ang_rms += pow(c->GetParams().opening_angle,2);
    }
    _o_ang_avg /= clusters.size();
    _o_ang_rms /= clusters.size();
    _o_ang_rms = pow(_o_ang_rms,0.5);
    
    //_ana_tree->Fill();
    
   
    //Reject match if it is very track-like
    return (_o_ang_avg*_o_ang_rms > 0.01 ? 1 : -1);
  }

  //------------------------------
  void CFAlgoShowerCompat::Report()
  //------------------------------
  {

  }

  void CFAlgoShowerCompat::PrintClusterInfo(const cluster::ClusterParamsAlg &c){
    std::cout<<"    This cluster's info is as follows:"<<std::endl;
    std::cout<<"        Opening Angle: "<<c.GetParams().opening_angle<<std::endl;
    //    std::cout<<"        Opening Angle Charge Weight: "<<c.GetParams().opening_angle_charge_wgt<<std::endl;

  }
}
#endif
