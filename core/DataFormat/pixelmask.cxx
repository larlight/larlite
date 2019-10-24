#ifndef PIXELMASK_CXX
#define PIXELMASK_CXX

#include <limits>

#include "pixelmask.h"

namespace larlite {

  pixelmask::pixelmask()
    : _dim_per_point(0),
      _label(-1),      
      _bbox_v(4,0),
      _meta_xmin(0),
      _meta_ymin(0),
      _meta_xmax(0),
      _meta_ymax(0),
      _ncols(0),
      _nrows(0)
  {
    
  }

  pixelmask::pixelmask( int label, std::vector< std::vector<float> >& data_v,
			float meta_xmin, float meta_ymin, float meta_xmax, float meta_ymax,
			int ncols, int nrows, int dim_per_point, bool data_is_array_indices ) 
    : _dim_per_point(dim_per_point),
      _label(label),
      _meta_xmin(meta_xmin),
      _meta_ymin(meta_ymin),
      _meta_xmax(meta_xmax),
      _meta_ymax(meta_ymax),
      _ncols(ncols),
      _nrows(nrows)
  {

    // sanity checks
    if ( _meta_xmin>_meta_xmax ) throw std::runtime_error( "pixelmask::pixelmask(...) input xmin>xmax!" );
    if ( _meta_ymin>_meta_ymax ) throw std::runtime_error( "pixelmask::pixelmask(...) input ymin>ymax!" );
    if ( _ncols<=0 ) throw std::runtime_error( "pixelmask::pixelmask(...) ncols<=0!" );
    if ( _nrows<=0 ) throw std::runtime_error( "pixelmask::pixelmask(...) nrows<=0!" );
    if ( _dim_per_point<2 ) throw std::runtime_error( "pixelmask::pixelmask(...) dim_per_point needs to be >=2" );
    
    // pixel dimensions
    _pixel_width  = (_meta_xmax-_meta_xmin)/float(_ncols);
    _pixel_height = (_meta_ymax-_meta_ymin)/float(_nrows);
    
    // initialize bounding box
    // (xmin,ymin,xmax,ymax)
    _bbox_v = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
		std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };

    _data_v.resize( dim_per_point*data_v.size(), 0.0 );    
    if ( !data_is_array_indices ) {
      // data provided in coordinate values (not array positions)
      for ( size_t ipt=0; ipt<data_v.size(); ipt++ ) {
	const std::vector<float>& ptdata = data_v[ipt];
	if ( (int)ptdata.size()>dim_per_point )
	  throw std::runtime_error( "pixelmask::pixelmask(...) dimension of point is > dim_per_point" );

	if ( ptdata[0]<_bbox_v[0] ) _bbox_v[0] = ptdata[0]; // xmin
	if ( ptdata[1]<_bbox_v[1] ) _bbox_v[1] = ptdata[1]; // ymin
	if ( ptdata[0]>_bbox_v[2] ) _bbox_v[2] = ptdata[0]; // xmax
	if ( ptdata[1]>_bbox_v[3] ) _bbox_v[3] = ptdata[1]; // ymax
	
	size_t start = ipt*_dim_per_point;
	std::copy( ptdata.begin(), ptdata.end(), _data_v.begin()+start );
      }
    }
    else {
      // data provided as array indices. so we need to convert to image coordinates
      for ( size_t ipt=0; ipt<data_v.size(); ipt++ ) {
	const std::vector<float>& ptdata = data_v[ipt];
	if ( (int)ptdata.size()>dim_per_point )
	  throw std::runtime_error( "pixelmask::pixelmask(...) dimension of point is > dim_per_point" );
	
	size_t start = ipt*_dim_per_point;
	
	// get the x,y
	float x = pos_x( (int)ptdata[0] );
	float y = pos_y( (int)ptdata[1] );
	_data_v.at(start)   = x;
	_data_v.at(start+1) = y;

	if ( x<_bbox_v[0] ) _bbox_v[0] = x; // xmin
	if ( y<_bbox_v[1] ) _bbox_v[1] = y; // ymin
	if ( x>_bbox_v[2] ) _bbox_v[2] = x; // xmax
	if ( y>_bbox_v[3] ) _bbox_v[3] = y; // ymax
	
	// store values (values beyond first 2 dimensions)
	if ( _dim_per_point>2 )
	  std::copy( ptdata.begin()+2, ptdata.end(), _data_v.begin()+start+2 );
      }//end of input pt loop   
    }//end of if input is array indices
    
  }

  // ==================================================================================
  // Coordinate transforms
  // ---------------------

  float pixelmask::pos_x( int col, bool checkbounds ) const {
    float x = _meta_xmin + _pixel_width*col;
    if ( checkbounds && ( col<0 || col>=cols() || x<_meta_xmin || x>=_meta_xmax ) ) throw std::runtime_error("pixelmask::pos_x(col) out of bounds");
    return x;
  }

  float pixelmask::pos_y( int row, bool checkbounds ) const {
    float y = _meta_ymin + _pixel_height*row;
    if ( checkbounds && ( row<0 || row>=rows() || y<_meta_ymin || y>=_meta_xmax ) ) throw std::runtime_error("pixelmask::pos_y(row) out of bounds");
    return y;
  }
  
  int pixelmask::col( float x, bool checkbounds ) const {
    int c = (x-_meta_xmin)/_pixel_width;
    if ( checkbounds && ( x<_meta_xmin || x>=_meta_xmax || c<0 || c>=cols() ) ) throw std::runtime_error("pixelmask::col(x) out of bounds");
    return c;
  }

  int pixelmask::row( float y, bool checkbounds ) const {
    int r = (y-_meta_ymin)/_pixel_height;
    if ( checkbounds && ( y<_meta_ymin || y>=_meta_ymax || r<0 || r>=rows() ) ) throw std::runtime_error("pixelmask::row(y) out of bounds");
    return r;
  }
  
  bool pixelmask::contains( float x, float y ) const {
    if ( x<_meta_xmin || x>=_meta_xmax || y<_meta_ymin || y>=_meta_ymax ) return false;
    return true;
  }

  bool pixelmask::contains_index( int col, int row ) const { 
    if ( col<0 || col>=cols() || row<0 || row>=rows() )  return false;
    return true;
  }

  std::vector<float> pixelmask::point( int index ) const {
    std::vector<float> out( _data_v.begin()+index*_dim_per_point, _data_v.begin()+(index+1)*_dim_per_point );
    return out;
  }
  
  std::vector<int>   pixelmask::pixelpos( int index ) const {
    std::vector<float> out( _data_v.begin()+index*_dim_per_point, _data_v.begin()+(index+1)*_dim_per_point );
    std::vector<int> array(2);
    array[0] = col( out[0] );
    array[1] = row( out[1] );
    return array;
  }
  
  // =============================================================================
  // Mask generation
  // ---------------
  
  std::vector<float> pixelmask::as_vector_mask( float meta_xmin, float meta_ymin,
						float meta_xmax, float meta_ymax,
						int ncols, int nrows ) const // mask provided image info
  {
    // provides an unrolled (row,col) vector which represents the mask points densly
    
    float out_pixheight = (meta_ymax-meta_ymin)/float(nrows);
    float out_pixwidth  = (meta_xmax-meta_xmin)/float(ncols);
    
    // pixel mask for a given crop
    std::vector<float> mask( ncols*nrows, 0.0);
    for (int index=0; index<len(); index++ ) {
      const std::vector<float>& pt = point(index);

      // get the col,row position in the new image meta
      int out_col = (pt[0]-meta_xmin)/out_pixwidth;
      int out_row = (pt[1]-meta_ymin)/out_pixheight;
      if ( out_col<0 || out_col>=ncols ) continue;
      if ( out_row<0 || out_row>=nrows ) continue;
      if ( _dim_per_point==2 ) mask[ out_row*nrows + out_col ] = 1.0;
      else mask[ out_row*nrows + out_col ] = pt[2]; // set value from 3rd element
    }
    
    return mask;
  }

  std::vector<float> pixelmask::as_vector_mask() const {
    // provides an unrolled (row,col) vector which represents the mask points densly
    // provided in the embedding image
    return as_vector_mask( _meta_xmin, _meta_ymin, _meta_xmax, _meta_ymax, cols(), rows() );
  }

  // =====================================================
  // return bbox
  // -----------

  std::vector<float> pixelmask::as_vector_bbox() const { return _bbox_v; };

  std::vector<float> pixelmask::as_vector_bbox( float meta_xmin, float meta_ymin,
						float meta_xmax, float meta_ymax ) const {
    // bounding box constrained within crop
    std::vector<float> bbox_out = _bbox_v;
    if ( bbox_out[0] < meta_xmin ) bbox_out[0] = meta_xmin;
    if ( bbox_out[1] < meta_ymin ) bbox_out[1] = meta_ymin;
    if ( bbox_out[2] > meta_xmax ) bbox_out[2] = meta_xmax;
    if ( bbox_out[3] > meta_ymax ) bbox_out[3] = meta_ymax;
    return bbox_out;
  }

  std::vector<int> pixelmask::as_vector_array_bounds( float meta_xmin, float meta_ymin, float meta_xmax, float meta_ymax ) const {
    std::vector<float> bbox = as_vector_bbox( meta_xmin, meta_ymin, meta_xmax, meta_ymax ); // crop-adjusted bbox
    std::vector<int> bbox_out(4);
    bbox_out[0] = col( bbox[0] );
    bbox_out[1] = row( bbox[1] );
    bbox_out[2] = col( bbox[2] );
    bbox_out[3] = row( bbox[3] );
    return bbox_out;
  }

  std::vector<int> pixelmask::as_vector_array_bounds() const {
    return as_vector_array_bounds( _meta_xmin, _meta_ymin, _meta_xmax, _meta_ymax );
  }
    

}
#endif
