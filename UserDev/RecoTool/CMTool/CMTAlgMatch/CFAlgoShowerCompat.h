/**
 * \file CFAlgoShowerCompat.h
 *
 * \ingroup CMTAlgMatch
 * 
 * \brief This algo only matches clusters if they are not track-like.
 * This is implemented in an algo because it allows the comparison of
 * cluster parameters across planes, rather than individually.
 * It is intended to be added as the last matching algo, with the
 * "kLastAlgo" mode.
 *
 * @author davidkaleko_NAME
 */

/** \addtogroup CMTAlgMatch

    @{*/
#ifndef CFALGOSHOWERCOMPAT_HH
#define CFALGOSHOWERCOMPAT_HH

#include "CMTool/CMToolBase/CFloatAlgoBase.h"
#include "TFile.h"
#include "TTree.h"

namespace cmtool {
  /**
     \class CFAlgoShowerCompat
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoShowerCompat : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoShowerCompat();
    
    /// Default destructor
    virtual ~CFAlgoShowerCompat(){};

    //
    // Author should be aware of 3 functions at least: Float, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    /**
       Core function: given a set of CPANs, return a float which indicates 
       the compatibility the cluster combination.
    */
    virtual float Float(const std::vector<const cluster::cluster_params*> &clusters);

    /**
       Optional function: called after each iterative approach if a manager class is
       run with verbosity level <= kPerIteration. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance, called together with manager's Reset()
    virtual void Reset();

    void PrintClusterInfo(const cluster::cluster_params &c);
    
   void WriteHaxFile()
    {
      _fout_hax->cd();
      _ana_tree->Write();
      _fout_hax->Close();
    };
    

    /**
       Optional function: called at the beginning of 1st iteration. This is called per event.
     */
    //virtual void EventBegin(const std::vector<cluster::ClusterParamsAlg> &clusters);

    /**
       Optional function: called at the end of event ... after the last merging iteration is over.
     */
    //virtual void EventEnd();
 
    /**
       Optional function: called at the beggining of each iterative loop.
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    //virtual void IterationBegin(const std::vector<cluster::ClusterParamsAlg> &clusters);

    /**
       Optional function: called at the end of each iterative loop.
     */
    //virtual void IterationEnd();

  private:

    TTree* _ana_tree;
    double _o_ang_avg;
    double _o_ang_rms;
    double _o_ang_wt_avg;
    double _o_ang_wt_rms;
    double _max_trackness;
    double _max_len_over_width;
    double _min_oa_over_len;
    double _max_poly_perim_over_A;
    double _min_modhitdens;

    TFile* _fout_hax;

  };
}
#endif
/** @} */ // end of doxygen group 

