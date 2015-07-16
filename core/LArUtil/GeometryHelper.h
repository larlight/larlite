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

 
    /**
     * @brief Take a line (start point and direction) and project that into 2D
     * @details This function will return the start point and direction of a line segment projected into 2D
     *  It works by using the Point_3Dto2D function to project the start point into 2D.  Then, it finds a second
     *  point along the line by using startPoint + direction3D.  If that point is not in the TPC, it halves the distance and
     *  tries that point.  Then, once it has two points in the TPC, it projects both into the plane, normalizes the direction,
     *  and returns them as PxPoints by reference
     * 
     * @param startPoint3D TVector3 describing the 3D start point (in detector coordinates) 
     * @param direction3D TVector3 describing the direction that the line emanates from the start point.  Need not be normalized  
     * @param plane The plane in which the projection needs to be done.
     * @param startPoint2D Returned by reference: A PxPoint representing the projection of the start point into the plane.
     * @param direction2D Returned by reference: A PxPoint representing the projection of the direction into the plane, normalized.
     */
    void Line_3Dto2D( const TVector3 & startPoint3D, const TVector3 & direction3D, unsigned int plane,
                      PxPoint & startPoint2D, PxPoint & direction2D) const;

    /**
     * @brief deterimine if a point is in the TPC
     * @details Checks this point against the geometry parameters
     * 
     * @param pointIn3D The point to be evaluated, units in cm.
     * @return true if the point is in the TPC, false if otherwise
     */
    bool Point_isInTPC(const TVector3 & pointIn3D) const;

    /**
     * @brief return the conversion from time tick to centimeters
     * @return Floating point conversion in the units [cm / time tick]
     */
    float TimeToCm() const {return fTimeToCm;}
    
    /**
     * @brief return conversion from wires to centimeters
     * @return Floating point conversion in the units [ wire / cm ]
     */
    float WireToCm() const {return fWireToCm;}
  
  private:


    float fTimeToCm;
    float fWireToCm;

  };

}

#endif
/** @} */ // end of doxygen group 

