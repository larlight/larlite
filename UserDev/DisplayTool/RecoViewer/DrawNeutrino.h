/**
 * \file DrawNeutrino.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawNeutrino
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef EVD_DRAWNEUTRINO_H
#define EVD_DRAWNEUTRINO_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "DrawTrack.h"
#include "DrawVertex.h"
#include "RecoBase.h"


namespace evd {

  class Neutrino2D {
    
  public:
    
    Vertex2D _vtx;
    Track2D  _trk;
    
  };
  
  class DrawNeutrino : public larlite::ana_base, public RecoBase<Neutrino2D> {
    
  public:
    
    /// Default constructor
    DrawNeutrino();
    
    /// Default destructor
    ~DrawNeutrino(){}
    
    virtual bool initialize();

    virtual bool analyze(larlite::storage_manager* storage);

    virtual bool finalize();

  private:
    
    
  };
  
} // evd

#endif
/** @} */ // end of doxygen group

