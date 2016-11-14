/**
 * \file MCQCluster.h
 *
 * \ingroup App
 *
 * \brief Class def header for a class MCQCluster
 *
 * @author kazuhiro
 */

/** \addtogroup App

    @{*/
#ifndef MCINTERACTION_H
#define MCINTERACTION_H

#include <iostream>
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
#include "OpT0Finder/Base/BaseAlgorithm.h"
#include "OpT0Finder/Algorithms/LightPath.h"

namespace flashana {
  /**
     \class MCQCluster
     User defined class MCQCluster ... these comments are used to generate
     doxygen documentation!
  */
  class MCQCluster : public flashana::BaseAlgorithm {

  public:

    /// Default constructor
    MCQCluster(const std::string name="MCQCluster");

    /// Default destructor
    ~MCQCluster(){}

    void Construct(const flashana::QCluster_t& q);

    void Construct( const larlite::event_mctrack&,
                    const larlite::event_mcshower&,
		    const flashana::LightPath&);

    const std::vector<flashana::QCluster_t>& QClusters() const;

    const flashana::QCluster_t& QCluster(size_t) const;

    size_t MCTrack2QCluster(size_t) const;

    size_t MCShower2QCluster(size_t) const;

    void SetTriggerTime(const double time) { _trigger_time = time; }

    const flashana::MCSource_t& MCObjectID(size_t) const;

    #ifndef __CINT__
    void Swap(std::vector<flashana::QCluster_t>&&,
	      std::vector<flashana::MCSource_t>&&);
    #endif

  protected:

    void _Configure_(const ::fcllite::PSet &pset);

  private:

    flashana::MCSource_t Identify( const unsigned int,
				   const larlite::event_mctrack&,
				   const larlite::event_mcshower& ) const;

    void ExpandQCluster(const flashana::LightPath& lightpath,
			const larlite::mctrack& mct,
			flashana::QCluster_t& tpc_obj);

    bool _use_xshift;
    double _trigger_time;
    double _extension;
    std::vector<flashana::QCluster_t> _qcluster_v;
    std::vector<size_t> _mctrack_2_qcluster;
    std::vector<size_t> _mcshower_2_qcluster;
    std::vector<flashana::MCSource_t> _qcluster_2_mcobject;

    bool  _use_mc_dedx;
    float _op_RO_start ;
    float _op_RO_end ;

  };

  /**
     \class flashana::MCQClusterFactory
  */
  class MCQClusterFactory : public CustomAlgoFactoryBase {
  public:
    /// ctor
    MCQClusterFactory() { CustomAlgoFactory::get().add_factory("MCQCluster",this); }
    /// dtor
    ~MCQClusterFactory() {}
    /// creation method
    BaseAlgorithm* create(const std::string instance_name) { return new MCQCluster(instance_name); }
  };


}

#endif
/** @} */ // end of doxygen group
