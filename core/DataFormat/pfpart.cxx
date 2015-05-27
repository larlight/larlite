#ifndef LARLITE_PFPART_CXX
#define LARLITE_PFPART_CXX

#include "pfpart.h"

namespace larlite {

  pfpart::pfpart()
    : data_base(data::kPFParticle)
    , fPdgCode(0)
    , fSelf(0)
    , fParent(pfpart::kPFParticlePrimary)
  {clear_data();}

  void pfpart::clear_data() 
  {
    fPdgCode = 0;
    fSelf = 0;
    fParent = pfpart::kPFParticlePrimary;
  }

  pfpart::pfpart(int pdgCode, size_t self, size_t parent, const std::vector<size_t>& daughters)
    : fPdgCode(pdgCode)
    , fSelf(self)
    , fParent(parent)
    , fDaughters(daughters)
  {}

  pfpart::pfpart(int pdgCode, size_t self, size_t parent, std::vector<size_t>&& daughters)
    : fPdgCode(pdgCode)
    , fSelf(self)
    , fParent(parent)
    , fDaughters(std::move(daughters))
  {}

  /// Accessors                                                                                                                                 
  int    pfpart::PdgCode()      const {return fPdgCode;}
  bool   pfpart::IsPrimary()    const {return fParent == kPFParticlePrimary;}
  int    pfpart::NumDaughters() const {return fDaughters.size();}
  size_t pfpart::Self()         const {return fSelf;}
  size_t pfpart::Parent()       const {return fParent;}
  //size_t                     Daughter(size_t idx) const;
  const std::vector<size_t>& pfpart::Daughters() const {return fDaughters;}

  //----------------------------------------------------------------------                                                                        
  // ostream operator.                                                                                                                            
  //                                                                                                                                              
  std::ostream& operator<< (std::ostream& o, const pfpart& c)
  {
    o << std::setiosflags(std::ios::fixed)  << std::setprecision(2);
    o << "PFParticle hypothesis PDG Code "    << std::setw(8)  << std::right       << c.PdgCode()
      << ", is primary = " << c.IsPrimary()
      << ", # Daughters "  << std::setw(5)  << std::right    << c.NumDaughters() << std::endl;
    
    return o;
  }
  
  //----------------------------------------------------------------------                                                                        
  // less than operator - basically sort in order of index into main collection                                                                   
  bool operator< (const pfpart& a, const pfpart& b)
  {
    return a.Self() < b.Self();
  }
  
}
#endif
  
