/**
 * \file ClusterInfo.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class ClusterInfo
 *
 * @author jhewes
 */

/** \addtogroup App

    @{*/

#ifndef LARLITE_CLUSTERINFO_H
#define LARLITE_CLUSTERINFO_H

#include "Analysis/ana_base.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"

namespace showerreco {
  /**
     \class ClusterInfo
     User custom analysis class made by jhewes
   */
  class ClusterInfo : public larlite::ana_base{
  
  public:

    /// Default constructor
    ClusterInfo(){ _name="ClusterInfo"; _fout=0;}

    /// Default destructor
    virtual ~ClusterInfo(){}

    virtual bool initialize();
    virtual bool analyze(larlite::storage_manager* storage);
    virtual bool finalize();
    
    void SetInputProducer(std::string s) {_input_producer = s;}
    void SetOutputProducer(std::string s) {_output_producer = s;}
    

  protected:

    ::cluster::CRUHelper _cru_helper;

    std::string _input_producer;
    std::string _output_producer;
    
    float _closing_angle_charge_wgt;
    std::vector<float> _fann;
    
    TTree * _tree;
    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
