#ifndef SIMCH_CXX
#define SIMCH_CXX

#include "simch.h"

namespace larlite {

  //--------------------------------------------------
  void ide::clear_data()
  //--------------------------------------------------
  {
    TObject::Clear();
    trackID      = data::kINVALID_INT;
    numElectrons = data::kINVALID_DOUBLE;
    energy       = data::kINVALID_DOUBLE;

    x = data::kINVALID_DOUBLE;
    y = data::kINVALID_DOUBLE;
    z = data::kINVALID_DOUBLE;
  }

  //--------------------------------------------------
  void simch::clear_data() 
  //--------------------------------------------------
  {
    data_base::clear_data();
    fChannel = data::kINVALID_USHORT;
    fTDCIDEs.clear();
  }

  //--------------------------------------------------------------------------
  std::vector<larlite::ide> simch::TrackIDsAndEnergies(UShort_t startTDC,
						       UShort_t endTDC) const
  //--------------------------------------------------------------------------
  {
    // make a map of track ID values to larlite::ide objects
    std::map<UInt_t, larlite::ide> idToIDE;

    std::vector<larlite::ide> ides;
    if(startTDC > endTDC) return ides;

    std::map<UShort_t, std::vector<larlite::ide> >::const_iterator mitr;
    std::map<UShort_t, std::vector<larlite::ide> >::const_iterator start = fTDCIDEs.lower_bound(startTDC);
    std::map<UShort_t, std::vector<larlite::ide> >::const_iterator end   = fTDCIDEs.upper_bound(endTDC);

    for(mitr = start; mitr != end; mitr++){

      // grab the vector of IDEs for this tdc
      const std::vector<larlite::ide> &idelist = (*mitr).second;
      std::vector<larlite::ide>::const_iterator itr = idelist.begin();
      // now loop over them and add their content to the map
      while( itr != idelist.end() ){

	if( idToIDE.find((*itr).trackID) != idToIDE.end() ){
          Double_t nel1   = idToIDE[(*itr).trackID].numElectrons;
          Double_t nel2   = (*itr).numElectrons;
          Double_t en1    = idToIDE[(*itr).trackID].energy;
          Double_t en2    = (*itr).energy;
          Double_t energy = en1+en2;
          Double_t weight = nel1 + nel2;
          // make a weighted average for the location information
          idToIDE[(*itr).trackID].x            = ((*itr).x*nel2 + idToIDE[(*itr).trackID].x*nel1)/weight;
          idToIDE[(*itr).trackID].y            = ((*itr).y*nel2 + idToIDE[(*itr).trackID].y*nel1)/weight;
          idToIDE[(*itr).trackID].z            = ((*itr).z*nel2 + idToIDE[(*itr).trackID].z*nel1)/weight;
          idToIDE[(*itr).trackID].numElectrons = weight;
          idToIDE[(*itr).trackID].energy = energy;
        } // end if the track id for this one is found
        else{
          larlite::ide temp(*itr);
          idToIDE[(*itr).trackID] = temp;
        }

	itr++;
      } // end loop over vector
    } // end loop over tdc values


    ides.reserve(idToIDE.size());
    // now fill the vector with the ides from the map
    for(auto itr = idToIDE.begin(); itr != idToIDE.end(); itr++)

      ides.push_back((*itr).second);


    return ides;
  }
    
  //-----------------------------------------------------------------------------
  Double_t simch::Charge(UShort_t tdc) const
  //-----------------------------------------------------------------------------
  {
    auto const tdc_itr = fTDCIDEs.find(tdc);
    if(tdc_itr == fTDCIDEs.end()) return 0;

    Double_t charge = 0;
    for(auto const this_ide : (*tdc_itr).second)

      charge += this_ide.numElectrons;
    
    return charge;
  }

  //-----------------------------------------------------------------------------
  Double_t simch::Energy(UShort_t tdc) const
  //-----------------------------------------------------------------------------
  {
    auto const tdc_itr = fTDCIDEs.find(tdc);
    if(tdc_itr == fTDCIDEs.end()) return 0;

    Double_t energy = 0;
    for(auto const this_ide : (*tdc_itr).second)

      energy += this_ide.energy;
    
    return energy;
  }

  //-----------------------------------------------------------------------------
  void simch::add_ide(UShort_t tdc, ide in)
  //-----------------------------------------------------------------------------
  {
    auto tdc_itr = fTDCIDEs.find(tdc);

    if(tdc_itr == fTDCIDEs.end()) {
      // case 1: new tdc
      fTDCIDEs.insert(std::pair<UShort_t,std::vector<larlite::ide> >(tdc,std::vector<larlite::ide>(1,in)));
      return;
    }

    // case 2: combining w/ existing ide from the same track id
    for(auto i_ide : (*tdc_itr).second) {
      
      if(i_ide.trackID == in.trackID) {
	
	Double_t weight = i_ide.numElectrons + in.numElectrons;

	i_ide.x = (i_ide.x * i_ide.numElectrons + in.x * in.numElectrons)/weight;
	i_ide.y = (i_ide.y * i_ide.numElectrons + in.y * in.numElectrons)/weight;
	i_ide.z = (i_ide.z * i_ide.numElectrons + in.z * in.numElectrons)/weight;
	i_ide.numElectrons = weight;
	i_ide.energy += in.energy;

	return;
      }

    }
    
    // case 3: inserting this ide as the 1st one from this track id
    (*tdc_itr).second.push_back(in);
    return;

  }

}
#endif
