#include <iostream>
#include <algorithm>

#ifndef __FORLARLITE__
// -- LARSOFT --
#include "Flash.hh"

#ifdef __BUILD_ROOT_DICT__
ClassImp( subevent::Flash )
#endif

namespace subevent {

#else
  // -- LARLITE --
  #include "tflash.h"

namespace larlite {
#endif

  Flash::Flash() :
    area( 0.0 ), area30( 0.0 ), fcomp_gausintegral( 0.0 )
  {
    claimed = false;
    expectation.clear();
    waveform.clear();
  }

  Flash::Flash( int ch_, int tstart_, int tend_, int tmax_, float maxamp_, std::vector< double >& expectation_, std::vector< double >& waveform_ ) :
    ch (ch_), tstart( tstart_ ), tend( tend_ ), tmax( tmax_ ), maxamp( maxamp_) {

    storeWaveform( waveform_ );
    storeExpectation( expectation_ );
    area = 0.0;
    area30 = 0.0;
    fcomp_gausintegral = 0.0;
  }

  Flash::Flash( const Flash& orig ) :
    ch( orig.ch ), tstart( orig.tstart ), tend( orig.tend ), tmax( orig.tmax ), maxamp( orig.maxamp ), 
    area( orig.area ), area30( orig.area30 ), fcomp_gausintegral( orig.fcomp_gausintegral), claimed( orig.claimed )
  {
    // copy constructor
    storeWaveform( orig.waveform );
    storeExpectation( orig.expectation );
  }

  Flash::~Flash() {
    expectation.clear();
    waveform.clear();
  }

  void Flash::storeWaveform( const std::vector< double >& waveform_ ) {
    waveform.clear();
    waveform.reserve( waveform_.size() );

    for ( std::vector< double >::const_iterator it=waveform_.begin(); it!=waveform_.end(); it++ ) {
      waveform.push_back( (double)*it );
    }
  }

  void Flash::storeExpectation( const std::vector< double >& expectation_ ) {
    expectation.clear();
    expectation.reserve( expectation_.size() );    
    for ( std::vector< double >::const_iterator it=expectation_.begin(); it!=expectation_.end(); it++ ) {
      //std::cout << *it << " ";
      expectation.push_back( *it );
    }    
    //std::cout << std::endl;
  }
  
}
