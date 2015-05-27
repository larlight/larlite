/**
 * \file CBAlgoMergeWithinBoundary.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoMergeWithinBoundary
 *
 * @author pandacruzer
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOMERGEWITHINBOUNDARY_H
#define RECOTOOL_CBALGOMERGEWITHINBOUNDARY_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include "LArUtil/GeometryUtilities.h"

/**
   \class CBAlgoMergeWithinBoundary
   User defined class CBAlgoMergeWithinBoundary ... these comments are used to generate
   doxygen documentation!
 */

namespace cmtool {

	class CBAlgoMergeWithinBoundary : public CBoolAlgoBase{

	public:

	  /// Default constructor
	  CBAlgoMergeWithinBoundary();

	  /// Default destructor
	  virtual ~CBAlgoMergeWithinBoundary(){}

      virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
	      const ::cluster::ClusterParamsAlg &cluster2);

  	  bool isBig(const ::cluster::ClusterParamsAlg &cluster);
      bool isSmall(const ::cluster::ClusterParamsAlg &cluster);
	  //float* inBoundary(const ::cluster::ClusterParamsAlg &cluster);
      float closestApproach(const ::cluster::ClusterParamsAlg &cluster1,
                          const ::cluster::ClusterParamsAlg &cluster2);

	private:

  	float _max_length;
  	float _min_length;
  	float _max_hits;
  	float _min_hits;
    float _max_dist;
    float _max_ep;

	};
}
#endif
/** @} */ // end of doxygen group 

