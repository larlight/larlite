/**
 * \file CheckHitAlignment.h
 *
 * \ingroup MCComp
 * 
 * \brief Class def header for a class CheckHitAlignment
 *
 * @author david caratelli
 */

/** \addtogroup MCComp

    @{*/

#ifndef LARLITE_CHECKHITALIGNMENT_H
#define LARLITE_CHECKHITALIGNMENT_H

#include "Analysis/ana_base.h"
#include "DataFormat/hit.h"
#include "DataFormat/simch.h"

namespace larlite {
  /**
     \class CheckHitAlignment
     User custom analysis class made by SHELL_USER_NAME
   */
  class CheckHitAlignment : public ana_base{
  
  public:

    /// Default constructor
    CheckHitAlignment(){ _name="CheckHitAlignment"; _fout=0;}

    /// Default destructor
    virtual ~CheckHitAlignment(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void setHitProducer(std::string s) { _hit_producer = s; };

    // is this channel empty?
    bool hasHits(unsigned short ch) 
    { 
      if ( _chan_to_hit_map.find(ch) != _chan_to_hit_map.end() )
	return true;
      return false;
    }

    // get hits for a given channel
    std::vector<larlite::hit> getHits(unsigned short ch) { return _chan_to_hit_map[ch]; }
    // get ides for a given channel
    std::map<unsigned short, double > getIDEs(unsigned short ch) { return _chan_to_ide_map[ch]; }

  protected:

    // map connecting channel number to hit object index
    std::map<unsigned short, std::vector<larlite::hit> > _chan_to_hit_map;

    // map connecting channel number to map of TDC -> charge from IDEs
    std::map<unsigned short, std::map<unsigned short, double> > _chan_to_ide_map;
    
    // hit producer to be studied
    std::string _hit_producer;

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
