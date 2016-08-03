#ifndef RECOTOOL_CBALGOHIGHQLINECOMPAT_CXX
#define RECOTOOL_CBALGOHIGHQLINECOMPAT_CXX

#include "CBAlgoHighQLineCompat.h"

namespace cmtool {

  CBAlgoHighQLineCompat::CBAlgoHighQLineCompat() : CBoolAlgoBase() {

    //Sets default values
    _debug = false;
    _min_n_hits = 10;
    _max_angle_sep = 10; //in degrees
    _frac_hits_to_keep = .50;
    _tge = new TGraphErrors();

    _debug_histo = new TH1F("debug_histo","Angle Differences between Clusters",180,0,180); //1 degree bins

  } //end constructor

  bool CBAlgoHighQLineCompat::Bool(const ::cluster::cluster_params &cluster1,
                                   const ::cluster::cluster_params &cluster2)
  {

    size_t hits1     = cluster1.hit_vector.size();
    size_t hits2     = cluster2.hit_vector.size();
    //if don't make hit cut return false
    if ( (hits1 < _min_n_hits) or (hits2 < _min_n_hits) )
      return false;

    auto high_q_hits1 = GetHighQHits(cluster1.hit_vector,_frac_hits_to_keep);
    auto high_q_hits2 = GetHighQHits(cluster2.hit_vector,_frac_hits_to_keep);
    

    //If for some reason slope can't be computed (fitter fails), just assume these
    //two clusters are not compatible
    double slope1, slope2;
    try{ 
      slope1 = ComputeSlope(high_q_hits1);
      slope2 = ComputeSlope(high_q_hits2);
    }
    catch (int e){
      if(_debug){
        std::cout<< "Note: ComputeSlope failed ("<<e<<") so these clusters will not be merged. "
                 << "Try keeping a higher fraction of the hits." << std::endl;
      }
      return false;
    }

    if (_debug){
      std::cout << "Cluster1:" << std::endl;
      std::cout << "\t Start: (" << cluster1.start_point.w << ", " << cluster1.start_point.t << ")" << std::endl; 
      std::cout << "\t Slope: "<<slope1<<std::endl;
      std::cout << "Cluster2:" << std::endl;
      std::cout << "\t Start: (" << cluster2.start_point.w << ", " << cluster2.start_point.t << ")" << std::endl; 
      std::cout << "\t Slope: "<<slope2<<std::endl;
      std::cout << std::endl;
    }

    double anglediff = (atan(slope1)-atan(slope2))*180./3.14159265358979323846264338327950; //lol
    _debug_histo->Fill(abs(anglediff));

    return abs(anglediff) < _max_angle_sep ? true : false;
    
  } // end Merge function 
  

  double CBAlgoHighQLineCompat::ComputeSlope(const std::vector<larutil::Hit2D> &pxhit_vector){
    
    double slope = -1.;
    //Clear all points from the TGraphErrors
    _tge->Set(0);
    size_t pt_ctr = 0;
    // Loop over the input pxhits
    for(auto const& pxh : pxhit_vector){
      // Add the pxhit (wire, time) to the graph
      _tge->SetPoint(pt_ctr,pxh.w,pxh.t);
      // Make the error on this point 1/charge (in both x- and y- dirs)
      //      _tge->SetPointError(pt_ctr++,1/pxh.charge,1/pxh.charge);

      if(_debug)
        std::cout<<"Set point "<<pt_ctr<<" to ("<<pxh.w<<","<<pxh.t<<")."<<std::endl;
      pt_ctr++;
    }

    // Fit the graph with a line (note: if 0 or 1 points in the graph, this fails)
    if(pt_ctr <= 1)
      throw 1;

    // S: Save the fit result
    // Q: Quiet mode
    // N: Do not store the graphics function, don't draw
    // 0: Don't plot the result of the fit
    // C: Don't calculate chisquare (saves time)
    _frp = _tge->Fit("pol1","SN0CQ");
    Int_t fitstatus = _frp;

    if(fitstatus) 
      throw fitstatus;


    slope =_frp->Value(1);
    //double slope_err = _frp->ParError(1);
 
    return slope;
    
  }

  const std::vector<larutil::PxHit> CBAlgoHighQLineCompat::GetHighQHits(const std::vector<larutil::PxHit> &pxhit_vector, double frac){
    /// This is super inefficient. I copy the hit vector, sort it, then make ANOTHER hit vector w/ reduced size
    /// Can definitely be rewritten faster. I don't care enough right now.

    // Make a copy of the input pxhit vector
    std::vector<larutil::PxHit> myvec = pxhit_vector;

    // Sort the copy by charge (highest charge hits first)
    std::sort(myvec.begin(),myvec.end(),CBAlgoHighQLineCompat::SortPxHitsMethod);

    // Then make my own pxhit vector of the highest "frac" fraction, then return it
    size_t nhits_reduced = size_t(myvec.size()*frac);
    return std::vector<larutil::PxHit>(myvec.begin(),myvec.begin()+nhits_reduced);

  }

  bool CBAlgoHighQLineCompat::SortPxHitsMethod(const larutil::PxHit &h1, const larutil::PxHit &h2){
    return h1.charge > h2.charge;
  }

}//end namespace cmtool
#endif
