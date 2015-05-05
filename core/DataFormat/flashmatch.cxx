////////////////////////////////////////////////////////////////////////
// \version $Id: 
//
// \brief Definition of FlashMatch analysis object
//
// \author bjpjones@mit.edu
////////////////////////////////////////////////////////////////////////

//#include "AnalysisBase/FlashMatch.h"
//#include "messagefacility/MessageLogger/MessageLogger.h"
#include "flashmatch.h"

namespace larlite{

  //----------------------------------------------------------------------
  flashmatch::flashmatch() 
    : data_base(data::kFlashMatch)
    , fChi2(0)
    , fFlashID(0)
    , fSubjectID(0)
    , fInBeam(false)
  {
  }

  //----------------------------------------------------------------------
  flashmatch::flashmatch(double Chi2,
			 int FlashID,
			 int SubjectID,
			 bool InBeam)
    : data_base(data::kFlashMatch)
    , fChi2(Chi2)
    , fFlashID(FlashID)
    , fSubjectID(SubjectID)
    , fInBeam(InBeam)
  {

  }

  //----------------------------------------------------------------------
  // ostream operator.  
  //
  /*
  std::ostream& operator<< (std::ostream & o, flashmatch const& a)
  {
    o << "Flash match with Chi2: "    << a.fChi2
      << "\n from flash: "            << a.fFlashID
      << "\n to object: "             << a.fSubjectID
      <<std::endl;
    

    return o;
  }
  */  
}
