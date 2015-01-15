/**
 * \file SPAFilterCosmic.h
 *
 * \ingroup SPAFilter
 * 
 * \brief Class def header for a class SPAFilterCosmic
 *
 * @author kazuhiro
 */

/** \addtogroup SPAFilter

    @{*/

#ifndef SELECTIONTOOL_SPAFILTERCOSMIC_H
#define SELECTIONTOOL_SPAFILTERCOSMIC_H

#include "SPTBase/SPAFilterBase.h"

namespace sptool {

  /**
     \class SPAFilterCosmic
     User custom SPAFilter class made by kazuhiro
   */
  class SPAFilterCosmic : public SPAFilterBase {
  
  public:

    /// Default constructor
    SPAFilterCosmic();

    /// Default destructor
    virtual ~SPAFilterCosmic(){};

    /// Select function
    virtual SPAOrder Select (const SPAData &data);

	void SetCosmicScoreCut (double score) { _score = score ; }	

   private:
   	
	double _score ;

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
