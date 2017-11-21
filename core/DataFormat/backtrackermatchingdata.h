////////////////////////////////////////////////////////////////////////////
// \brief Definition of backtrackermatchingdata object for LArSoft
//
// \author A. Furmanski
//         W. Ketchum
//         Y. Tse
//         Translated by J. Zennamo
////////////////////////////////////////////////////////////////////////////
#ifndef LARLITE_BACKTRACKERMATCHINGDATA_H
#define LARLITE_BACKTRACKERMATCHINGDATA_H

#include "TTree.h"
#include "Base/MCConstants.h"
#include <vector>
#include <utility>
#include "data_base.h"

namespace larlite{
  
  /*
     \class backtrackermatchingdata
     backtracker container for matching hits
  */  
  class backtrackermatchingdata : public data_base {
    
  public: 
    
     backtrackermatchingdata() : data_base(data::kBackTrackerMatchingData) {
      clear_data();
    }
    
    virtual ~backtrackermatchingdata() {}

    void clear_data();
    
    backtrackermatchingdata(double Cleanliness,     
			    double Completeness,     
			    float  IdeFraction,     
			    int    IsMaxIDE,     
			    float  IdeNFraction,     
			    int    IsMaxIDEN,     
			    float  NumElectrons,     
			    float  Energy);
    

    double       cleanliness()  const;
    double       completeness()  const;
    float        ideFraction()  const;
    int          isMaxIDE()  const;
    float        ideNFraction()  const;
    int          isMaxIDEN()  const;
    float        numElectrons()  const;
    float        energy()  const;


  private: 
     double      fcleanliness;
     double      fcompleteness;
     float       fideFraction;
     int         fisMaxIDE;
     float       fideNFraction;
     int         fisMaxIDEN;
     float       fnumElectrons;
     float       fenergy;
     
  };
  


  class event_backtrackermatchingdata
      : public std::vector<larlite::backtrackermatchingdata>, public event_base {

  public:

    /// Default constructor
    event_backtrackermatchingdata(std::string name="noname")
        : event_base(data::kBackTrackerMatchingData, name) {
      clear_data();
    }

    /// Default copy constructor
    event_backtrackermatchingdata(const event_backtrackermatchingdata& original)
        : std::vector<larlite::backtrackermatchingdata>(original), event_base(original) {}

    /// Default destructor
    ~event_backtrackermatchingdata() {}

    /// Method to clear currently held data contents in the buffer
    virtual void clear_data() {
      event_base::clear_data();
      clear();
    }

  };

}

#endif
