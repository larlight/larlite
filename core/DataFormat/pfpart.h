/**
 * \file pfpart.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for pfpart data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_PFPART_H
#define LARLITE_PFPART_H

#include "data_base.h"
#include <vector>
#include <utility>
#include <iomanip>

namespace larlite{
  /**
     \class pfpart
     LArSoft PFParticle class equivalent data container
  */
  class pfpart : public data_base {

  public:
    /// Define index to signify primary particle
    static const size_t kPFParticlePrimary = std::numeric_limits<size_t>::max();
    //static constexpr size_t kPFParticlePrimary = std::numeric_limits<size_t>::max();

  public:

    /// Default constructor necessary for gccxml - not really for public use 
    pfpart();
    
    /// Primary constructor                                                                                                                       
    pfpart(int pdgCode, size_t self, size_t parent, const std::vector<size_t>& daughters);

    pfpart(int pdgCode, size_t self, size_t parent, std::vector<size_t>&& daughters);
    
    /// Default destructor
    virtual ~pfpart(){}

  private:

    int                 fPdgCode;   ///< A preliminary estimate of the PFParticle type using the PDG code                                       
    size_t              fSelf;      ///< Self reference                                                                                         
    size_t              fParent;    ///< Index into PFParticle collection for parent                                                            
    std::vector<size_t> fDaughters; ///< Vector of indices into PFParticle Collection for daughters 

  public:

    /// method to clear data member
    virtual void clear_data();

    /// Accessors                                                                                                                                 
    int    PdgCode()      const;
    bool   IsPrimary()    const;
    int    NumDaughters() const;
    size_t Self()         const;
    size_t Parent()       const;
    //size_t                     Daughter(size_t idx) const;
    const std::vector<size_t>& Daughters() const;

    friend std::ostream& operator << (std::ostream& o, const pfpart& c);
    friend bool          operator <  (const pfpart& a, const pfpart& b);

  private:
    
  };
  
  /**
     \class event_pfpart
     A collection storage class of multiple pfpart
  */
  class event_pfpart : public std::vector<larlite::pfpart>, 
		       public event_base {
    
  public:
    
    /// Default constructor
    event_pfpart(std::string name="noname") : event_base(data::kPFParticle,name) 
    { clear_data(); }
    
    /// Default copy constructor
    event_pfpart(const event_pfpart& original) : std::vector<larlite::pfpart>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_pfpart(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
