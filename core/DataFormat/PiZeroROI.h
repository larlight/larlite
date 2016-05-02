/**
 * \file PiZeroROI.h
 *
 * 
 * \brief This data product is to encapuslate the ROI surrounding a PiZero
 *
 * @author Joseph Zennamo, jaz8600@fnal.gov 
*/

#ifndef LARLITE_PIZEROROI_H
#define LARLITE_PIZEROROI_H

#include "TTree.h"
#include "Base/MCConstants.h" 
#include <vector>
#include <utility>
#include "data_base.h"

namespace larlite{
  
  class PiZeroROI : public data_base
  {
    
  public:
    
    /// Default destructor
    virtual ~PiZeroROI(){};
    
  PiZeroROI() : data_base(data::kPiZeroROI)
      { clear_data(); }
    
    PiZeroROI(const std::vector < std::pair< int, int > > Wire,
	      const std::vector < std::pair< int, int > > Time);
    
    PiZeroROI(const std::vector < std::pair< int, int > > Wire,
	      const std::vector < std::pair< int, int > > Time,
	      const std::vector < std::pair< int, int > > PiZeroWire,
	      const std::vector < std::pair< int, int > > PiZeroTime);
    
  PiZeroROI(const PiZeroROI& orig) : _wire_range(orig._wire_range),
                                     _t_range(orig._t_range),
                                     _pi0_wire_range(orig._pi0_wire_range),
                                     _pi0_t_range(orig._pi0_t_range),      
                                     _muon_vtx(orig._muon_vtx),
                                     _nu_vtx(orig._nu_vtx),
                                     _vtx(orig._vtx),
                                     _trk_end(orig._trk_end)
	                             {}
    
    void clear_data();

    // Here are the Setters    
    void SetROI(const std::vector < std::pair< int, int > > Wire,
		const std::vector < std::pair< int, int > > Time);

    void SetROI(const std::vector < std::pair< int, int > > Wire,
		const std::vector < std::pair< int, int > > Time,
		const std::vector < std::pair< int, int > > PiZeroWire,
		const std::vector < std::pair< int, int > > PiZeroTime);
    
    
    void SetPiZeroROI(const std::vector < std::pair< int, int > > PiZeroWire,
		      const std::vector < std::pair< int, int > > PiZeroTime);

    void SetMuonVertex( const std::vector<float> vtx);
    void SetNeutrinoVertex( const std::vector<float> vtx);

    void SetVertex( const std::vector< std::pair <int, int> > vtx);

    void SetTrackEnd( const std::vector< std::pair <int, int> > trk_end);


    // Here are the Getters
    std::vector<float> GetMuonVertex() const;
    std::vector<float> GetNeutrinoVertex() const;
    std::vector < std::pair <int, int > > GetVertex() const;
    std::vector < std::pair <int, int > > GetTrackEnd() const;
    std::vector < std::pair< int, int > > GetWireROI() const;
    std::vector < std::pair< int, int > > GetTimeROI() const;
    std::vector < std::pair< int, int > > GetPiZeroWireROI() const;
    std::vector < std::pair< int, int > > GetPiZeroTimeROI() const;


  private:
    
    std::vector < std::pair< int, int > > _wire_range; // size 3 planes, min then max
    std::vector < std::pair< int, int > > _t_range; // size 3 planes, min then max
    std::vector < std::pair< int, int > > _pi0_wire_range; // size 3 planes, min then max
    std::vector < std::pair< int, int > > _pi0_t_range; // size 3 planes, min then max
    std::vector<float> _muon_vtx; // size 3 plane, pair with tick, wire for vertex
    std::vector<float> _nu_vtx; // size 3 plane, pair with tick, wire for vertex
    std::vector < std::pair <int, int > > _vtx; // size 3 plane, pair with tick, wire for vertex
    std::vector < std::pair <int, int > > _trk_end; // size 3 plane, pair with tick, wire for track-end
    
  };

  /**
     \class event_PiZeroROI
     A collection storage class of multiple PiZeroROIs.
  */
  class event_PiZeroROI : public std::vector<larlite::PiZeroROI>, 
			public event_base {
    
  public:
    
    /// Default constructor
    event_PiZeroROI(std::string name="noname") : event_base(data::kPiZeroROI,name) { clear_data(); }
    
    /// Default copy constructor
    event_PiZeroROI(const event_PiZeroROI& original) : std::vector<larlite::PiZeroROI>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_PiZeroROI(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
  };


}
  
#endif
  
