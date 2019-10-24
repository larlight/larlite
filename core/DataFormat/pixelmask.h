/**
 * \file pixelmask.h
 *
 * \ingroup DataFormat
 *
 * \brief Class def header for a class pixelmask
 *
 * @author T. Wongjirad translating ClusterMask LArCV2 class by JoshuaMills 2018/12
 */

/** \addtogroup DataFormat

    @{*/
#ifndef PIXELMASK_H
#define PIXELMASK_H

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "data_base.h"

namespace larlite {

  /**
     \class pixelmask
     Stores ordered pairs (x,y) with the intention of storing pixel locations
     Can provide a bounding box for an image if needed
     A hack to avoid larcv2 -> larcv1 translation
     
     One application we keep in mind, is the need to have to generate this truth information
       for a crop, or sub-region, of an image. This means, the bounding box and mask have to 
       be modified when the crop only contains a portion of the object.

     We allow the fact that there is a 2D coordinate system overlaid on the 2D array

     points provided are assumed to be (x,y) <--> (col,row)
     however, when returning dense arrays, the values are in (row,col) order
    
  */

  class pixelmask {
    
  public:
    pixelmask(); 
    pixelmask( int label, std::vector< std::vector<float> >& data_v,
	       float meta_xmin, float meta_ymin, float meta_xmax, float meta_ymax,
	       int ncols, int nrows, int dim_per_point=2, bool data_is_array_indices=false );
    ~pixelmask() {}


    int   len() const { return _data_v.size()/_dim_per_point; }; // number of points
    int   dim_per_point() const { return _dim_per_point; };
    int   cols() const { return _ncols; };
    int   rows() const { return _nrows; };
    bool  isempty() const { return _dim_per_point==0; };
    
    float pos_x( int col, bool checkbounds=false ) const;
    float pos_y( int row, bool checkbounds=false ) const;
    int   col( float x, bool checkbounds=false ) const;
    int   row( float y, bool checkbounds=false ) const;
    bool  contains( float x, float y ) const;
    bool  contains_index( int col, int row ) const;
    
    std::vector<float> point( int index )    const;
    std::vector<int>   pixelpos( int index ) const;

    // mask-generations: dense vector
    std::vector<float> as_vector_mask( float meta_xmin, float meta_ymin, float meta_xmax, float meta_ymax, int ncols, int nrows ) const; // mask provided image info
    std::vector<float> as_vector_mask() const; // dense mask embedded in image coords provided at constructor

    // returning the bounding boxes    
    std::vector<float> as_vector_bbox() const;
    std::vector<float> as_vector_bbox( float meta_xmin, float meta_ymin, float meta_xmax, float meta_ymax ) const; // bounding box constrained within crop
    
    std::vector<int>   as_vector_array_bounds() const;    
    std::vector<int>   as_vector_array_bounds( float meta_xmin, float meta_ymin, float meta_xmax, float meta_ymax ) const; // mask provided image info


    
  protected:

    int _dim_per_point;
    int _label;
    std::vector< float > _data_v; // unrolled
    std::vector< float>  _bbox_v; // (xmin, ymin, xmax, ymax)

    // meta: translation between overlaid coordinate system and image array
    // origin (xmin,ymin) is (0,0) in the data array
    float _meta_xmin;
    float _meta_ymin;
    float _meta_xmax;
    float _meta_ymax;
    int   _ncols;
    int   _nrows;
    float _pixel_width;
    float _pixel_height;
    

  };
  //End of Class


  /**
     \class event_pixelmask
     A collection storage class of pixels
  */
  class event_pixelmask : public std::vector<larlite::pixelmask>, public event_base {
  public:
      
    /// Default constructor
    event_pixelmask(std::string name="noname") : event_base(data::kPixelMask,name) { clear_data(); }
    
    
    /// Default copy constructor
    event_pixelmask(const event_pixelmask& original) : std::vector<larlite::pixelmask>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_pixelmask(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
  };
  
  
}
#endif
/** @} */ // end of doxygen group
