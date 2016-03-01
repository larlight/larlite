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
    
    PiZeroROI(const PiZeroROI& orig) : _wire_range(orig._wire_range),
                                       _t_range(orig._t_range),
                                       _vtx(orig._vtx)
					 {}

    void clear_data();

    // Here are the Setters    
    void SetROI(const std::vector < std::pair< int, int > > Wire,
		const std::vector < std::pair< int, int > > Time);
    void SetVertex( const std::vector< std::pair <int, int> > vtx);

    // Here are the Getters
    std::vector < std::pair <int, int > > GetVertex();
    std::vector < std::pair< int, int > > GetWireROI();
    std::vector < std::pair< int, int > > GetTimeROI();
    

  private:
    
    std::vector < std::pair< int, int > > _wire_range; // size 3 planes, min then max
    std::vector < std::pair< int, int > > _t_range; // size 3 planes, min then max
    std::vector < std::pair <int, int > > _vtx; // size 3 plane, pair with tick, wire for vertex
    
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
  
