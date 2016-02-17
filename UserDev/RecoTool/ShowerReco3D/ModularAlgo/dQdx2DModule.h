/**
 * \file dQdx2DModule.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class dQdx2DModule
 *
 * @authors Joseph Zennamo, UChicago, jzennamo@uchicago.edu
 * @authors Brooke Russell, Yale U, brooke.russell@yale.edu
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef DQDX2DMODULE_H
#define DQDX2DMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
#include "TVector2.h"
/**
   \class dQdx2DModule : ShowerRecoModuleBase
   This is meant to compute the 2D dQdx along the start of the shower. 
 */
namespace showerreco {

  class dQdx2DModule : ShowerRecoModuleBase{

  public:

    /// Default constructor
    dQdx2DModule(){_name = "dQdx2DModule";}

    /// Default destructor
    ~dQdx2DModule(){}

    void do_reconstruction(const ProtoShower &, Shower_t &);

    void initialize();

  private:
    //This variable will contain the average dQdx from the start of a shower projected into the 3 planes.
    //it is inteded to be filled such that element 1 contains the average dQdx for the U-plane, 
    //element 2 contains the same for the V-plane, and element 3 will be for the Y-plane. 
    std::vector<double> dQdx; 
    std::vector<double> dQdx_smooth; 

    std::vector<double> HitDist_toStart;
    std::vector<double> HitCharge;
    std::vector<double> pitch;
    std::vector<int> IndexSort;
    
    std::vector< std::vector <double > > HitdeltaQdeltax_v;

    std::vector<double> Len;

     // ADC -> e- conversion factors
    double _fC_to_e;
    double _ADC_to_mV;
    double _shp_time;
    double _asic_gain;


    //Tree Variuable 
    std::vector< double > _HitdeltaQdeltax_v0;
    std::vector< double > _HitdeltaQdeltax_v1;
    std::vector< double > _HitdeltaQdeltax_v2;

    int _event;

    double _ADC_to_fC;

    double _length;
    double _dQdx0;
    double _dQdx1;
    double _dQdx2;
    double _Q0;
    double _Q1;
    double _Q2;
    double _StartW;
    double _StartT;
    double _ShowW;
    double _ShowT;
    double _BestdQdx;
    double _mean_dQdx;
    double _rms_dQdx;
    double _dQdx0_smooth;
    double _dQdx1_smooth;
    double _dQdx2_smooth;
    double _BestdQdx_smooth;    
    double _pitch;
    int _Nhits;
    int _Nhits_smooth;
    
  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

