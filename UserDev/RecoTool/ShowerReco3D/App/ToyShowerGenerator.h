/**
 * \file ToyShowerGenerator.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class ToyShowerGenerator
 *
 * @author cadams
 */

/** \addtogroup App

    @{*/

#ifndef LARLITE_TOYSHOWERGENERATOR_H
#define LARLITE_TOYSHOWERGENERATOR_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class ToyShowerGenerator
     User custom analysis class made by cadams

     This class produces toy sets of showers, along with fake hit lists and clusters associated with them.
     It goes like this:
      1) Select a 3D vertex isotropically in the Detector, and there is option to impose a range (default all) 
      2) Select a 3D direction.  Default is isotropic but can also set the direction range of phi, theta
      3) Select an energy, drawn isotropically between a range (default is 200 MeV to 3 GeV)
        a) Based on energy, length is drawn from a function that goes as 1.5m * log(e/1 GeV) * smearing
        b) width is drawn randomly from a gaussian
      4) With energy, axis, and width, 3D points along the axis are defined
        a) First x cm, drawn at random between 1 and 3, energy dep is linear
        b) After that, deposit half the energy along the axis with some smearing
        c) Other half is along the axis with bigger smear (sigma = width)
        d) total energy in points is equal to shower energy
      5) Each point is projected into each plane and a hit is created with a fixed width.
        a) The set of hits is turned into a cluster
      6) The shower, hits, and clusters are all saved to the event


   */
  class ToyShowerGenerator : public ana_base{
  
  public:

    /// Default constructor
    ToyShowerGenerator();

    /// Default destructor
    virtual ~ToyShowerGenerator(){}

    /** IMPLEMENT in ToyShowerGenerator.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ToyShowerGenerator.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ToyShowerGenerator.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void setOutProducerName(std::string s){_out_producer_name = s;}

    /**
     * @brief set the energy range
     * @details 
     * 
     * @param min min energy
     * @param max max energy
     */
    void setEnergyRange(float min, float max){_e_min = min; _e_max = max;}

    void setXRange(float min, float max){_x_min = min; _x_max = max;}
    void setYRange(float min, float max){_y_min = min; _y_max = max;}
    void setZRange(float min, float max){_z_min = min; _z_max = max;}

    void setThetaRange(float min, float max){_theta_min = min; _theta_max = max;}
    void setPhiRange(float min, float max){_phi_min = min; _phi_max = max;}

    void setLengthSmearing(float smear){_length_smearing = smear;}

    void setWidthSmearing(float width){_width_smearing = width;}

    void printParams() const;

  protected:

    std::string _out_producer_name;

    float _e_min, _e_max;

    float _x_min, _x_max;
    float _y_min, _y_max;
    float _z_min, _z_max;

    float _theta_min, _theta_max;
    float _phi_min, _phi_max;
    
    float _length_smearing;
    float _width_smearing;

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
