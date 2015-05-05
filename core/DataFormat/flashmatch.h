////////////////////////////////////////////////////////////////////////////
// \version 
//
// \brief Definition of data product to hold FlashMatch information
//
// \author bjpjones@mit.edu
//
////////////////////////////////////////////////////////////////////////////
#ifndef LARLITE_ANAB_FLASHMATCH_H
#define LARLITE_ANAB_FLASHMATCH_H

#include <vector>
#include "data_base.h"
namespace larlite {

  class flashmatch : public data_base {
  public:
    
    flashmatch();
    
    double fChi2;
    int    fFlashID;
    int    fSubjectID;    
    bool   fInBeam;    

  public:

    flashmatch(double Chi2, int FlashID, int SubjectID, bool InBeam);

    //friend std::ostream& operator << (std::ostream &o, flashmatch const& a);

    inline const double& Chi2()      const { return fChi2;        } 
    inline const int&    FlashID()   const { return fFlashID;     }
    inline const int&    SubjectID() const { return fSubjectID;     }
    inline const bool&   InBeam()    const { return fInBeam;     }
    
  };


  /**
     \class event_flashmatch
     A collection storage class of multiple flashmatchs.
  */
  class event_flashmatch : public std::vector<larlite::flashmatch>,
			   public event_base {
    
  public:
    
    /// Default constructor
    event_flashmatch(std::string name="noname") : event_base(data::kFlashMatch,name) {clear_data();}
    
    /// Default copy constructor
    event_flashmatch(const event_flashmatch& original)
      : std::vector<larlite::flashmatch>(original), event_base(original)
    {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
    /// Default destructor
    ~event_flashmatch(){}

  private:
    
  };

}

#endif //ANAB_FLASHMATCH
