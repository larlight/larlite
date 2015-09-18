/**
 * \file CPAlgoIgnoreTracks.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CPAlgoIgnoreTracks
 *
 * @author davidkaleko
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CPALGOIGNORETRACKS_H
#define RECOTOOL_CPALGOIGNORETRACKS_H

#include "CMTool/CMToolBase/CPriorityAlgoBase.h"

namespace cmtool {
  /**
     \class CPAlgoIgnoreTracks
     User implementation for CPriorityAlgoBase class
     doxygen documentation!
  */
  class CPAlgoIgnoreTracks : public CPriorityAlgoBase {
    
  public:
    
    /// Default constructor
    CPAlgoIgnoreTracks();
    
    /// Default destructor
    virtual ~CPAlgoIgnoreTracks(){};

    //
    // Author should be aware of 3 functions at least: Priority, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    /**
       Core function: given the CPAN input, return a float which indicates 
       the user-defined priority for analysis.
    */
    virtual float Priority(const ::cluster::cluster_params &cluster);

    /**
       Optional function: called after each iterative approach if a manager class is
       run with verbosity level <= kPerIteration. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance, called together with manager's Reset()
    virtual void Reset();

    /**
       Optional function: called at the beginning of 1st iteration. This is called per event.
     */
    //virtual void EventBegin(const std::vector<cluster::cluster_params> &clusters);

    /**
       Optional function: called at the end of event ... after the last merging iteration is over.
     */
    //virtual void EventEnd();
 
    /**
       Optional function: called at the beggining of each iterative loop.
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    //virtual void IterationBegin(const std::vector<cluster::cluster_params> &clusters);

    /**
       Optional function: called at the end of each iterative loop.
     */
    //virtual void IterationEnd();

    /// Function to check if cluster is a track
    bool IsTrack(const ::cluster::cluster_params &cluster);

    /// Setter for track-like parameters
    void SetMinHits(size_t BAKA) { _min_hits = BAKA; }
    void SetMinModHitDens(Double_t AHO) { _min_mod_hit_dens = AHO; }
    void SetMinMHitWires(Double_t BOKE) { _min_multihit_wires = BOKE; }
    void SetMinPrincipal(Double_t HEKOKI) { _min_principal = HEKOKI; }


  protected:
    
    size_t _min_hits;
    Double_t _min_mod_hit_dens;
    Double_t _min_multihit_wires;
    Double_t _min_principal;

  };
}
#endif
/** @} */ // end of doxygen group 

