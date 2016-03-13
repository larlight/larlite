/** ****************************************************************************
 * @file wire.cxx
 * @brief Definition of basic channel signal object.
 * @author brebel@fnal.gov
 * @see  wire.h
 * 
 * ****************************************************************************/

#include "wire.h"

// C/C++ standard libraries
#include <utility> // std::move()

namespace larlite{

  //----------------------------------------------------------------------
  wire::wire()
    : data_base(data::kWire)
    , fChannel(raw::InvalidChannelID)
    , fView(geo::kUnknown)
    , fSignalROI()
  {}

  //----------------------------------------------------------------------
  wire::wire(RegionsOfInterest_t const& sigROIlist,
	     raw::ChannelID_t channel,
	     geo::View_t view
	     )
    : data_base(data::kWire)
    , fChannel(channel)
    , fView(view)
    , fSignalROI(sigROIlist)
  {}

  //----------------------------------------------------------------------
  wire::wire(RegionsOfInterest_t&& sigROIlist,
	     raw::ChannelID_t channel,
	     geo::View_t view
	     )
    : data_base(data::kWire)
    , fChannel(channel)
    , fView(view)
    , fSignalROI(std::move(sigROIlist))
  {}

  //----------------------------------------------------------------------
  std::vector<float> wire::Signal() const {
    std::vector<float> res;
    if(!fSignalROI.size()) return res;
    auto const& rois = fSignalROI.get_ranges();
    res.resize(rois.back().begin_index()+rois.back().size());
    for(auto const& roi : rois) {
      for(size_t index=0; index<roi.size(); ++index)
	res[roi.begin_index() + index] = roi.data()[index];
    }
    return res;
  } // wire::Signal()

  //----------------------------------------------------------------------
  void wire::clear_data()
  { data_base::clear_data(); }

}
////////////////////////////////////////////////////////////////////////
