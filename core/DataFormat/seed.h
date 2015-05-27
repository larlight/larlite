/**
 * \file seed.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for seed data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_SEED_H
#define LARLITE_SEED_H

#include "spacepoint.h"
#include <vector>
#include "TMath.h"
#include "TVector3.h"
#include <algorithm>
#include <cstdlib>
namespace larlite{
  /**
     \class seed
     LArSoft Seed class equivalent data container
  */
  class seed : public data_base {
    
  public:
    
    /// Default constructor
    seed();

    seed(double* Pt, 
	 double * Dir, 
	 double* PtErr, 
	 double* DirErr);
    
    /// Default destructor
    virtual ~seed(){}

    void clear_data() { 
      data_base::clear_data(); 
    }

  private:
    double fSeedPoint[3];
    double fSeedDirection[3];
    double fSeedPointError[3];
    double fSeedDirectionError[3];
    bool   fIsValid;
    
  public:

    // Getters and setters
    void GetDirection(double* Dir, double* Err) const;
    void GetPoint(double*     Pt,  double* Err) const;
    double GetLength()                          const;

    void SetDirection(double* Dir, double* Err);
    void SetPoint(double *    Pt,  double* Err);


    double GetAngle(                ::larlite::seed const & AnotherSeed ) const;
    double GetProjDiscrepancy (     ::larlite::seed const & AnotherSeed ) const;
    double GetProjAngleDiscrepancy( ::larlite::seed const & AnotherSeed ) const;
    double GetDistance(             ::larlite::seed const & AnotherSeed ) const;

    ::larlite::seed Reverse();
    void GetVectorBetween(          ::larlite::seed const & AnotherSeed, double * xyz ) const;

    double GetDistanceFrom(         ::larlite::spacepoint const & SomePoint) const;

    int GetPointingSign(            ::larlite::seed const & AnotherSeed ) const;

    // Methods for errorless seeds
    seed(double* Pt, double * Dir);
    void SetDirection(double* Dir);
    void SetPoint(double *    Pt);

    // Seed validity checks
    bool IsValid() const;
    void SetValidity(bool Validity);
    
    void CrossProd(double, double, double,
		   double, double, double,
		   double&, double&, double&) const;
    
  private:
    
  };
  
  /**
     \class event_seed
     A collection storage class of multiple seeds.
  */
  class event_seed : public std::vector<larlite::seed>, 
		     public event_base {
    
  public:
    
    /// Default constructor
    event_seed(std::string name="noname") : event_base(data::kSeed,name) { clear_data(); }
    
    /// Default copy constructor
    event_seed(const event_seed& original) : std::vector<larlite::seed>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_seed(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
