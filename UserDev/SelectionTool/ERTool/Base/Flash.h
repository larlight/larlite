/**
 * \file Flash.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class ertool::Flash
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_FLASH_H
#define ERTOOL_FLASH_H

#include "GeoAlgo/GeoVector.h"
#include "ERToolConstants.h"
namespace ertool {

  /**
     \class Flash
     @brief Data holder class to contain Flash data. This is contained within ertool::EventData instance.
     This class instance is created per optical flash, and stored within ertool::EventData.
     AlgoX and FilterX will use ertool::EventData instance for evaluation.
  */
  class Flash : public ::geoalgo::Point_t {
    friend class EventData;
  public:

    /// default ctor
    Flash();

    /// alternative ctor
    Flash(const ::geoalgo::Vector&);
    /// alternative ctor
    Flash(const ::geoalgo::Vector&,const std::vector<double>&);

    /// default dtor
    virtual ~Flash(){};

    /// Reset function
    void Reset();

    /// ID getter
    FlashID_t FlashID() const;

    /// optical PE distribution
    std::vector<double> _npe_v;

  private:

    /// Function to set an id
    void SetFlashID(const FlashID_t id);

    /// Unique flash id
    FlashID_t _id;

  };
}
#endif
/** @} */ // end of doxygen group 

