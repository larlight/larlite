/**
 * \file GeometryHelper.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class GeometryHelper
 *
 * @author cadams
 */

/** \addtogroup LArUtil

    @{*/
#ifndef GEOMETRYHELPER_H
#define GEOMETRYHELPER_H

#include <iostream>
#include "PxUtils.h"
#include "Geometry.h"

/**
   \class GeometryHelper
   This class is meant as an aide to reconstruction by handling geometrical operations.  
   It includes functionality to go between 3D and 2D for points, lines, cones.

   In this class, all units are in centimeters and radians unless explicitly declared to be otherwise.
   To aide in doing the conversions, you may use the functions TimeToCm() and WireToCm() to convert wire/time
   to cm coordinates as such:
     wire [cm] = wire [Number] * WireToCmConversion (GeometryHelper::WireToCm() )

 */

namespace larutil{

  class GeometryHelper : public larlite::larlite_base {

  private:

    /**
     * @brief Constructor
     * @details Constructor is private.  To access this class, get a pointer using the GeometryHelper::GetME() function
     */
    GeometryHelper(){}

    /// Default destructor
    ~GeometryHelper(){}

    // Pointer to sole instance of this object
    static GeometryHelper* _me;

  public:
    
    /**
     * @brief Singleton Getter
     * @details Returns a pointer to the sole instance of this class
     * @return Pointer to GeometryHelper
     */
    static const GeometryHelper* GetME(){
      if(!_me) {
        _me = new GeometryHelper;
        _me -> Reconfigure();
      }
      return _me;
    }

    /**
     * @brief Reconfigures private parameters
     * @details Function to load private parameters necessary for conversions
     */
    void Reconfigure();

    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     * 
     * @param xyz TVector3 of the point in 3D to project into 2D
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     * 
     * @return Returns a PxPoint of the 2D projection into the plane
     */
    PxPoint Point_3Dto2D(const TVector3 & xyz, unsigned int plane) const;
    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     * 
     * @param xyz std::vector<float> of the point in 3D to project into 2D
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     * 
     * @return Returns a PxPoint of the 2D projection into the plane
     */
    PxPoint Point_3Dto2D(const std::vector<float> & xyz, unsigned int plane) const;

    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     * 
     * @param xyz std::vector<float> of the point in 3D to project into 2D
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     * 
     * @return Returns a PxPoint of the 2D projection into the plane
     */
    PxPoint Point_3Dto2D(const std::vector<double> & xyz, unsigned int plane) const;


    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     * 
     * @param xyz Pointer to float of the point in 3D to project into 2D.  Caller is responsible for creating and deleting this array
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     * 
     * @return Returns a PxPoint of the 2D projection into the plane
     */
    PxPoint Point_3Dto2D(float * xyz, unsigned int plane) const;

    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     * 
     * @param xyz Pointer to double of the point in 3D to project into 2D.  Caller is responsible for creating and deleting this array
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     * 
     * @return Returns a PxPoint of the 2D projection into the plane
     */
    PxPoint Point_3Dto2D(double * xyz, unsigned int plane) const;

 

    float TimeToCm() const {return fTimeToCm;}
    float WireToCm() const {return fWireToCm;}
  
  private:


    float fTimeToCm;
    float fWireToCm;

  };

}

#endif
/** @} */ // end of doxygen group 

