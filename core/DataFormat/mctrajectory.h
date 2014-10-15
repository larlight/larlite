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

#ifndef MCTRAJECTORY_H
#define MCTRAJECTORY_H

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

    mcstep(const TLorentzVector& p, const TLorentzVector& m)
      : _position(p), 
	_momentum(m)
    {}

    virtual ~mcstep(){}
    
    const TLorentzVector& Momentum() const {return _momentum;}

    const TLorentzVector& Position() const {return _position;}

    Double_t X()  const {return _position.X();  }
    Double_t Y()  const {return _position.Y();  }
    Double_t Z()  const {return _position.Z();  }
    Double_t T()  const {return _position.T();  }
    Double_t Px() const {return _momentum.Px(); }
    Double_t Py() const {return _momentum.Py(); }
    Double_t Pz() const {return _momentum.Pz(); }
    Double_t E() const  {return _momentum.E();  }

    void SetPosition(const TLorentzVector& v) { _position=v; };
    void SetMomentum(const TLorentzVector& v) { _momentum=v; };

  protected:

    TLorentzVector _position;
    TLorentzVector _momentum;

    ////////////////////////
    ClassDef(mcstep,1)
    ////////////////////////

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
    void push_back(const larlite::mcstep& step){ std::vector<larlite::mcstep>::push_back(step);}

    void push_back(const TLorentzVector p,
		   const TLorentzVector m)
    { std::vector<larlite::mcstep>::push_back(larlite::mcstep(p,m));}

  private:

    ////////////////////////
    ClassDef(mctrajectory,1)
    ////////////////////////    
      
  };
  
}
#endif

/** @} */ // end of doxygen group 
