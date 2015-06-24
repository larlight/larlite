/**
 * \file ToyDataMaker.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class ToyDataMaker
 *
 * @author kazuhiro
 */

/** \addtogroup App

    @{*/
#ifndef TOYDATAMAKER_H
#define TOYDATAMAKER_H

#include <iostream>
#include "ERTool/Base/Shower.h"
#include "ERTool/Base/Track.h"
namespace ertool {
  /**
     \class ToyDataMaker
     User defined class ToyDataMaker ... these comments are used to generate
     doxygen documentation!
  */
  class ToyDataMaker{
    
  public:
    
    /// Default constructor
    ToyDataMaker(){}
    
    /// Default destructor
    ~ToyDataMaker(){}

    /// Track maker
    void MakeTrack(double x,  double y,  double z,
		   double px, double py, double pz,
		   int pdg_code);
    
    /// Shower adder
    void MakeShower(double x, double y, double z,
		    double px, double py, double pz,
		    int pdg_code);

    /// Trajectory parameter
    void SetTrajectoryParams(const double step_size=5,
			     const double step_sigma=1,
			     const double angle_sigma=5,
			     const double dedx=2);

    /// Shower parameter
    void SetShowerParams(const double length,
			 const double dedx=2);

    /// Clear added elements
    void Clear();

  private:

    std::vector< ertool::Shower > _shower_v;
    std::vector< ertool::Track  > _track_v;
    double _vtx_sigma;
    double _track_step_size;
    double _track_step_sigma;
    double _track_step_angle_sigma;
    double _track_step_dedx;
    double _shower_dedx;
    
  };
}
#endif
/** @} */ // end of doxygen group 

