#ifndef CFALGOSHOWERCOMPAT_CXX
#define CFALGOSHOWERCOMPAT_CXX

#include "CFAlgoShowerCompat.h"
#include "TTree.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoShowerCompat::CFAlgoShowerCompat() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    
    _fout_hax = 0; 
    _ana_tree = 0;

    if(!_fout_hax)
      _fout_hax = new TFile("fout_hax.root","RECREATE");

    
    if(!_ana_tree){
      _ana_tree = new TTree("ana_tree","ana_tree");
      _ana_tree->Branch("o_ang_avg",&_o_ang_avg,"o_ang_avg/D");
      _ana_tree->Branch("o_ang_rms",&_o_ang_rms,"o_ang_rms/D");
      _ana_tree->Branch("o_ang_wt_avg",&_o_ang_wt_avg,"o_ang_wt_avg/D");
      _ana_tree->Branch("o_ang_wt_rms",&_o_ang_wt_rms,"o_ang_wt_rms/D");
      _ana_tree->Branch("max_trackness",&_max_trackness,"max_trackness/D");
      _ana_tree->Branch("max_len_over_width",&_max_len_over_width,"max_len_over_width/D");
      _ana_tree->Branch("min_oa_over_len",&_min_oa_over_len,"min_oa_over_len/D");
      _ana_tree->Branch("max_poly_perim_over_A",&_max_poly_perim_over_A,"max_poly_perim_over_A/D");
      _ana_tree->Branch("min_modhitdens",&_min_modhitdens,"min_modhitdens/D");
    }
    
  }

  //-----------------------------
  void CFAlgoShowerCompat::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoShowerCompat::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
    _o_ang_avg = 0;
    _o_ang_rms = 0;
    _o_ang_wt_avg = 0;
    _o_ang_wt_rms = 0;
    _max_trackness = -9999.;
    _max_len_over_width = -9999.;
    _min_oa_over_len = 9999.;
    _max_poly_perim_over_A = -9999.;
    _min_modhitdens = 9999.;

    //Take the smallest and largest opening angles of clusters in this 
    //permutation and average them
    double min_OA = 99999.;
    double max_OA = -99999.;
    double min_OA_wt = 99999.;
    double max_OA_wt = -99999.;
    for(auto const& c : clusters){
      //      PrintClusterInfo(*c);
      double this_OA = c->opening_angle;
      if(this_OA > max_OA) max_OA = this_OA;
      if(this_OA < min_OA) min_OA = this_OA;
      double this_OA_wt = c->opening_angle_charge_wgt;
      if(this_OA_wt > max_OA) max_OA_wt = this_OA_wt;
      if(this_OA_wt < min_OA) min_OA_wt = this_OA_wt;
      double this_trackness = c->trackness;
      if(this_trackness > _max_trackness) _max_trackness = this_trackness;
      double this_L_over_W = c->length / c->width;
      if(this_L_over_W > _max_len_over_width) _max_len_over_width = this_L_over_W;
      double this_OA_over_L = this_OA/c->length;
      if(this_OA_over_L < _min_oa_over_len) _min_oa_over_len = this_OA_over_L;
      double this_poly_perim_over_A = c->PolyObject.Perimeter()/c->PolyObject.Area();
      if(this_poly_perim_over_A > _max_poly_perim_over_A) _max_poly_perim_over_A=this_poly_perim_over_A;
      double this_modhitdens = c->modified_hit_density;
      if(this_modhitdens < _min_modhitdens) _min_modhitdens = this_modhitdens;
    }
  
    _o_ang_avg = (min_OA + max_OA)/2;
    _o_ang_rms = pow( (pow(min_OA,2)+pow(max_OA,2))/2 , 0.5);
    _o_ang_wt_avg = (min_OA_wt + max_OA_wt)/2;
    _o_ang_wt_rms = pow( (pow(min_OA_wt,2)+pow(max_OA_wt,2))/2 , 0.5);
    
    _ana_tree->Fill();
       
    bool accept_match = true;
    //Reject match if it is very track-like
    if(_min_oa_over_len < 0.0007) accept_match = false;
    if(_o_ang_avg*_o_ang_rms < 0.01) accept_match = false;
    if(_max_len_over_width > 20) accept_match = false;
   
    return accept_match ? 1 : -1;
  }

  //------------------------------
  void CFAlgoShowerCompat::Report()
  //------------------------------
  {

  }

  void CFAlgoShowerCompat::PrintClusterInfo(const cluster::cluster_params &c){
    std::cout<<"    This cluster's info is as follows:"<<std::endl;
    std::cout<<"        Opening Angle: "<<c.opening_angle<<std::endl;
    //    std::cout<<"        Opening Angle Charge Weight: "<<c.opening_angle_charge_wgt<<std::endl;

  }
}
#endif
