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

    mcstep(const TLorentzVector& p, const TLorentzVector& m)
      : _position(p), 
	_momentum(m)
    {}

    virtual ~mcstep(){}
    
    mcstep(const mcstep& orig) : _position(orig._position),
				 _momentum(orig._momentum)
    {}

    const TLorentzVector& Momentum() const {return _momentum;}

    const TLorentzVector& Position() const {return _position;}

    double X()  const {return _position.X();  }
    double Y()  const {return _position.Y();  }
    double Z()  const {return _position.Z();  }
    double T()  const {return _position.T();  }
    double Px() const {return _momentum.Px(); }
    double Py() const {return _momentum.Py(); }
    double Pz() const {return _momentum.Pz(); }
    double E() const  {return _momentum.E();  }

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
