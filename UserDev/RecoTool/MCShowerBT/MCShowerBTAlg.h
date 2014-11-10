/**
 * \file MCShowerBTAlg.h
 *
 * \ingroup MCShowerBTAlg
 * 
 * \brief Class def header for a class MCShowerBTAlg
 *
 * @author littlejohn, kaleko, terao
 */

/** \addtogroup MCShowerBTAlg

    @{*/
#ifndef RECOTOOL_MCSHOWERBT_H
#define RECOTOOL_MCSHOWERBT_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "DataFormat/simch.h"
#include "DataFormat/mcshower.h"

/**
   \class MCShowerBTAlg
   MCShowerBTAlg is meant to back-track reco-ed hits/clusters to MCShower
   doxygen documentation!
 */

namespace larlite {

  struct WireRange_t {
    unsigned int ch;
    double start, end;
    WireRange_t() {
      ch    = std::numeric_limits<unsigned int>::max();
      start = end = std::numeric_limits<double>::max();
    }
  };

  class MCShowerBTAlg {
    
  public:
    
    /// Default constructor (min energy is 0.02GeV)
    MCShowerBTAlg(){ 
      _min_energy = 0.02; 
      _max_energy = 1.e3;
    };

    /// Default destructor
    virtual ~MCShowerBTAlg(){}

    /// Set maximum energy for MCShowers to be considered
    void SetMaxEnergyCut(double energy) { _max_energy = energy; }

    /// Set minimum energy for MCShowers to be considered
    void SetMinEnergyCut(double energy) { _min_energy = energy; }

    void BuildMap(const std::vector<mcshower>& mcshower_v,
		  const std::vector<simch>& simch_v);

    const std::vector<unsigned int>&  UniqueShowerID() const
    { return fShowerIndex_v; }

    /**
       Relate Hit => MCShower. 
       Returns a vector of double w/ length = # of relevant MCShowers + 1.
       Each entry is # drifted electrons from each relevant MCShower.
       The last element contains a sum of drifted electrons that do not belong
       to any of relevant MCShower.
    */
    std::vector<double> MCShowerQ(const WireRange_t &h) const;

    /**
       Relate Hit => MCShower. 
       Returns a vector of double w/ length = # of relevant MCShowers + 1.
       Each entry is a fraction of # drifted electrons within the specified time
       range from each relevant MCShower. The last element contains a sum of drifted 
       electrons that do not belong to any of relevant MCShower.
    */
    std::vector<double> MCShowerQFrac(const WireRange_t &h) const;

    /**
       Relate Cluster => MCShower. 
       Returns a vector of double w/ length = # of relevant MCShowers + 1.
       Each entry is # drifted electrons from each relevant MCShower.
       The last element contains a sum of drifted electrons that do not belong
       to any of relevant MCShower.
    */
    std::vector<double> MCShowerQ(const std::vector<larlite::WireRange_t> &hit_v) const;

    /**
       Relate Cluster => MCShower. 
       Returns a vector of double w/ length = # of relevant MCShowers + 1.
       Each entry is a fraction of # drifted electrons within the specified time
       range from each relevant MCShower. The last element contains a sum of drifted 
       electrons that do not belong to any of relevant MCShower.
    */
    std::vector<double> MCShowerQFrac(const std::vector<larlite::WireRange_t> &hit_v) const;


  protected:

    std::vector<const larlite::simch*> fSimCh_v;
    std::map<unsigned int, unsigned int> fShowerPartMap;
    std::vector<unsigned int> fShowerIndex_v;
    
    double _max_energy;
    double _min_energy;
  };
  
}
#endif
/** @} */ // end of doxygen group 

