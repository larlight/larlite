/**
 * \file roi.h
 *
 * 
 * \brief This data product is to encapuslate the roi surrounding a PiZero
 *
 * @author Joseph Zennamo, jaz8600@fnal.gov 
*/

#ifndef LARLITE_ROI_H
#define LARLITE_ROI_H

#include "TTree.h"
#include "Base/MCConstants.h" 
#include <vector>
#include <utility>
#include "data_base.h"

#include "vertex.h"

namespace larlite{
  
  class roi : public data_base
  {
    
  public:
    
    /// Default destructor
    virtual ~roi(){};
    
    roi();
    
    roi(const std::vector < std::pair< int, int > > Wire,
	const std::vector < std::pair< int, int > > Time);

    roi(const larlite::vertex& vtx, const double& radius);
    
  roi(const roi& orig)
    : _wire_range(orig._wire_range),
      _time_range(orig._time_range),
      _vtx(orig._vtx)
	{}
    
    void clear_data();
    
    // Here are the Setters    
    void SetROI(const std::vector < std::pair< int, int > > Wire,
		const std::vector < std::pair< int, int > > Time);
    
    void SetVertex( const std::vector< std::pair <int, int> > vtx);
    
    // Here are the Getters
    const std::vector < std::pair< int, int > >& GetVertex()  const;
    const std::vector < std::pair< int, int > >& GetWireROI() const;
    const std::vector < std::pair< int, int > >& GetTimeROI() const;


  private:
    
    std::vector < std::pair< int, int > > _wire_range; // size 3 planes, min then max
    std::vector < std::pair< int, int > > _time_range; // size 3 planes, min then max
    std::vector < std::pair <int, int > > _vtx; // size 3 plane, pair with tick, wire for vertex
    
  };

  /**
     \class event_roi
     A collection storage class of multiple rois.
  */
  class event_roi : public std::vector<larlite::roi>, 
	    public event_base {
    
  public:
    
    /// Default constructor
    event_roi(std::string name="noname");
    
    /// Default copy constructor
    event_roi(const event_roi& original) : std::vector<larlite::roi>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_roi(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
  };


}
  
#endif
