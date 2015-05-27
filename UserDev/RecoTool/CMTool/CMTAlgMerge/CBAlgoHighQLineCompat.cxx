#ifndef RECOTOOL_CBALGOHIGHQLINECOMPAT_CXX
#define RECOTOOL_CBALGOHIGHQLINECOMPAT_CXX

#include "CBAlgoHighQLineCompat.h"

namespace cmtool {

  CBAlgoHighQLineCompat::CBAlgoHighQLineCompat() : CBoolAlgoBase() {

    //Sets default values
    _debug = false;
    _min_n_hits = 10;
    _max_slope_sep = 10; //in time (cm)/wire (cm)
    _frac_hits_to_keep = .10;
    _tge = new TGraphErrors();

  } //end constructor

  bool CBAlgoHighQLineCompat::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			      const ::cluster::ClusterParamsAlg &cluster2)
  {

    size_t hits1     = cluster1.GetHitVector().size();
    size_t hits2     = cluster2.GetHitVector().size();
    //if don't make hit cut return false
    if ( (hits1 < _min_n_hits) or (hits2 < _min_n_hits) )
      return false;

    auto high_q_hits1 = GetHighQHits(cluster1.GetHitVector(),_frac_hits_to_keep);
    auto high_q_hits2 = GetHighQHits(cluster2.GetHitVector(),_frac_hits_to_keep);
    
    double slope1 = ComputeSlope(high_q_hits1);
    double slope2 = ComputeSlope(high_q_hits2);

    if (_debug){
      std::cout << "Cluster1:" << std::endl;
      std::cout << "\t Start: (" << cluster1.GetParams().start_point.w << ", " << cluster1.GetParams().start_point.t << ")" << std::endl; 
      std::cout << "\t Slope: "<<slope1<<std::endl;
      std::cout << "Cluster2:" << std::endl;
      std::cout << "\t Start: (" << cluster2.GetParams().start_point.w << ", " << cluster2.GetParams().start_point.t << ")" << std::endl; 
      std::cout << "\t Slope: "<<slope2<<std::endl;
      std::cout << std::endl;
    }


    bool compatible = false;

    return compatible;
    
  } // end Merge function 
  

  double CBAlgoHighQLineCompat::ComputeSlope(const std::vector<larutil::PxHit> &pxhit_vector){
    
    double slope = -1.;
    //Clear all points from the TGraphErrors
    _tge->Set(0);
    size_t pt_ctr = 0;
    // Loop over the input pxhits
    for(auto const& pxh : pxhit_vector){
      // Add the pxhit (wire, time) to the graph
      _tge->SetPoint(pt_ctr,pxh.w,pxh.t);
      // Make the error on this point 1/charge (in both x- and y- dirs)
      _tge->SetPoint(pt_ctr++,1/pxh.charge,1/pxh.charge);
    }

    // Fit the graph with a line
    // S: Save the fit result
    // Q: Quiet mode
    // N: Do not store the graphics function, don't draw
    // 0: Don't plot the result of the fit
    // C: Don't calculate chisquare (saves time)
    _frp = _tge->Fit("pol1","SQN0C");
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
