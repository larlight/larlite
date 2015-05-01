/**
 * \file mctrajectory.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mctrajectory data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef LARLITE_MCTRAJECTORY_H
#define LARLITE_MCTRAJECTORY_H

#include "data_base.h"
#include <set>
#include <TVector3.h>
#include <TLorentzVector.h>

namespace larlite {
  /**
     \class mcstep
     Stores a single step of a particle
  */
  
  class mcstep {

  public:

    mcstep() {};

    mcstep(const TLorentzVector& p, const TLorentzVector& m);

    virtual ~mcstep(){}
    
    mcstep(const mcstep& orig);

    const TLorentzVector& Momentum() const;

    const TLorentzVector& Position() const;

    double X()  const;
    double Y()  const;
    double Z()  const;
    double T()  const;
    double Px() const;
    double Py() const;
    double Pz() const;
    double E()  const;
    void SetPosition(const TLorentzVector& v) { _position=v; };
    void SetMomentum(const TLorentzVector& v) { _momentum=v; };

  protected:

    TLorentzVector _position;
    TLorentzVector _momentum;

  };

  /**
     \class mctrajectory
     Equivalent of MCTrajectory in LArSoft
  */
    
  class mctrajectory : public std::vector<larlite::mcstep>,
		       public data_base {
    
  public:

    mctrajectory() : data_base(data::kMCTrajectory) {clear_data();}

    virtual ~mctrajectory(){}

    ///--- Utility ---///
    virtual void clear_data() {data_base::clear_data(); clear();}
    
    ///--- Setter/Adders ---//
    void push_back(const larlite::mcstep& step);

    void push_back(const TLorentzVector p,const TLorentzVector m);

  private:

  };
  
}
#endif

/** @} */ // end of doxygen group 
