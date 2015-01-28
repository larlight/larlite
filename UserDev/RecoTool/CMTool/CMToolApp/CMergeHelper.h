/**
 * \file CMergeHelper.h
 *
 * \ingroup CMToolApp
 * 
 * \brief Class def header for a class CMergeHelper
 *
 * @author kazuhiro
 */

/** \addtogroup CMToolApp

    @{*/
#ifndef CMERGEHELPER_H
#define CMERGEHELPER_H

#include <iostream>
#include "CMTool/CMToolBase/CMergeManager.h"

namespace cmtool {
  /**
     \class CMergeHelper
     User defined class CMergeHelper ... these comments are used to generate
     doxygen documentation!
  */
  class CMergeHelper{
    
  public:
    
    /// Default constructor
    CMergeHelper(){}
    
    /// Default destructor
    virtual ~CMergeHelper(){}

    CMergeManager& GetManager(size_t mgr_id);

    void SetAnaFile(TFile* fout);

    void Process(const std::vector<std::vector< ::larutil::PxHit> >& clusters);

    size_t size() const { return _mgr_v.size(); }

    const CMergeBookKeeper& GetResult() const { return _bk; }

    const std::vector< ::cluster::ClusterParamsAlg>& GetClusters() const;

  protected:

    std::vector< ::cmtool::CMergeManager> _mgr_v;

    CMergeBookKeeper _bk;
    
  };
}

#endif
/** @} */ // end of doxygen group 

