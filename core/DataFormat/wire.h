/** ****************************************************************************
 * @file wire.h
 * @brief Declaration of basic channel signal object.
 * @author brebel@fnal.gov
 * @see  wire.cxx
 * 
 * Changes:
 * 20141211 Gianluca Petrillo (petrillo@fnal.gov)
 *   data architecture revision changes:
 *   - fSignalType and SignalType() removed
 *   - fRawDigit and RawDigit() removed
 *   - fChannel and Channel() added
 *   - constructors now take directly a RawDigit, not its art::Ptr
 * 
 * ****************************************************************************/

#ifndef LARLITE_WIRE_H
#define LARLITE_WIRE_H

#include "data_base.h"
#include <vector>
//#include "SimpleTypesAndConstants/RawTypes.h" // raw::ChannelID_t
//#include "SimpleTypesAndConstants/geo_types.h"
//#include "Utilities/sparse_vector.h"
//#include "Base/RawTypes.h"
#include "Base/RawConstants.h"
#include "Base/GeoTypes.h"
#include "Base/GeoConstants.h"
#include "sparse_vector.h"

// prototype declaration
namespace larlite { class RawDigit; }


/// Reconstruction base classes
namespace larlite {

  /**
   * @brief Class holding the deconvoluted signals from a channel
   * @note Despite the name, this class is associated to a readout channel, not
   *       just a wire
   *
   * The channel content is expected to have been filtered from noise and
   * corrected for electronics response.
   * The content is presented as calibrated ADC counts, pedestal removed, as
   * function of time in discrete TDC units (use TimeService to discover the
   * exact extent of each tick).
   * The content is organized as time intervals where some signal is present
   * ("regions of interest", RoI), outside which we assume no signal, i.e,
   * calibrated ADC counts of 0.
   * Strictly speaking, the definition of the regions of interest is a negative
   * one: we first define where we are sure no signal is present; the rest will
   * constitute regions of interest.
   * The identification of such regions is responsibility of the algorithm
   * creating the wire object. In the simple approach, the whole readout window
   * is stored in a single region of interest, meaning that we don't claim any
   * of the channel signal to be definitely signal free.
   * 
   * Algorithms using the regions of interest can access the channel signal
   * information either ignoring the regions of interest, and being potentially
   * flooded by zeroes from the non-signal regions:
   *     
   *     for (float ADCcount: wire.Signal()) ...
   *     
   * or they can analyze region by region:
   *     
   *     for (auto iROI = wire.begin_range(); wire.end_range(); ++iROI) {
   *       const datarange_t& ROI = *iROI;
   *       const int FirstTick = ROI.begin_index();
   *       const int EndTick = ROI.end_index();
   *       const float FirstADC = ROI[FirstTick]; // index access is by absolute tick number
   *       for (float ADC: ROI) // ... or you can just iterate through
   *       // ...
   *     } // for
   *     
   * Note that the indexed access is always by absolute tick number.
   * More examples of the use of SignalROI() return value are documented in
   * larlite::sparse_vector .
   * 
   * Each channel is associated with a raw::RawDigit object. These associations
   * should be stored together with larlite::wire by the producer in a art::Assns
   * data product.
   */
  class wire : public data_base {
  public:
    /// a region of interest is a pair (TDC offset, readings)
    typedef larlite::sparse_vector<float> RegionsOfInterest_t;

    /// Default constructor
    wire();

    void clear_data();

  private:
    raw::ChannelID_t    fChannel;   ///< ID of the associated channel
    geo::View_t         fView;      ///< View corresponding to the plane of this wire
    RegionsOfInterest_t fSignalROI; ///< Signal on the channel

    friend class wireCreator; // helper to create wires in art

  public:
    /**
     * @brief Constructor: uses specified signal in regions of interest
     * @param sigROIlist signal organized in regions of interest
     * @param channel the ID of the channel
     * @param view the view the channel belongs to
     *
     * The information used from the raw digit are the channel ID and the
     * length in samples (TDC ticks) of the original readout window.
     * 
     * Signal is copied into the wire object. If possible, use the other
     * constructor that moves the data instead.
     */
    wire(RegionsOfInterest_t const& sigROIlist,
	 raw::ChannelID_t channel,
	 geo::View_t view
	 );

    /**
     * @brief Constructor: uses specified signal in regions of interest
     * @param sigROIlist signal organized in regions of interest
     * @param channel the ID of the channel
     * @param view the view the channel belongs to
     *
     * The information used from the raw digit are the channel ID and the
     * length in samples (TDC ticks) of the original readout window.
     * 
     * Signal information is moved from sigROIlist, that becomes empty.
     */
    wire(RegionsOfInterest_t&& sigROIlist,
	 raw::ChannelID_t channel,
	 geo::View_t view
	 );

    ///@{
    ///@name Accessors

    /// Return a zero-padded full length vector filled with RoI signal
    std::vector<float>  Signal() const;

    /// Returns the list of regions of interest
    const RegionsOfInterest_t& SignalROI()  const;

    /// Returns the number of time ticks, or samples, in the channel
    size_t                     NSignal()    const;

    /// Returns the view the channel belongs to
    geo::View_t                View()       const;

    /// Returns the ID of the channel (or InvalidChannelID)
    raw::ChannelID_t           Channel()    const;
    ///@}

    /// Returns whether this channel ID is smaller than the other
    bool operator< (const wire& than) const;

  }; // class wire

  /**
     \class event_wire
     A collection storage class of multiple wires.
  */
  class event_wire : public std::vector<larlite::wire>,
		     public event_base {
    
  public:
    
    /// Default constructor
    event_wire(std::string name="noname") : event_base(data::kWire,name) {clear_data();}
    
    /// Default copy constructor
    event_wire(const event_wire& original) : std::vector<larlite::wire>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_wire(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  };
}

inline const larlite::wire::RegionsOfInterest_t&
larlite::wire::SignalROI()  const { return fSignalROI;        }
inline size_t                     larlite::wire::NSignal()    const { return fSignalROI.size(); }
inline larlite::geo::View_t       larlite::wire::View()       const { return fView;             }
inline larlite::raw::ChannelID_t  larlite::wire::Channel()    const { return fChannel;          }
inline bool                       larlite::wire::operator< (const wire& than) const
{ return Channel() < than.Channel(); }

#endif // WIRE_H

////////////////////////////////////////////////////////////////////////
