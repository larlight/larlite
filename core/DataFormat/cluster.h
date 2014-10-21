/**
 * \file cluster.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for cluster data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_CLUSTER_H
#define LARLITE_CLUSTER_H

#include "data_base.h"

namespace larlite{

  /**
     \class cluster
     LArSoft Cluster class equivalent data container
  */

  class cluster : public data_base {

  public:
    
    /// Default constructor
    cluster() : data_base(data::kCluster) { clear_data(); }
    
    /// Default destructor
    virtual ~cluster(){}
    
    void set_charge   (double q)      { fTotalCharge = q; }
    void set_dtdw     (double v)      { fdTdW=v; }
    void set_dqdw     (double v)      { fdQdW=v; }
    void set_dtdw_err (double v)      { fSigmadTdW=v; }
    void set_dqdw_err (double v)      { fSigmadQdW=v; }
    void set_id       (int i)         { fID = i; }
    void set_view     (geo::View_t v) { fView=v; }
    void set_start_vtx     (const std::vector<double> vtx) { fStartPos=vtx;      }
    void set_end_vtx       (const std::vector<double> vtx) { fEndPos=vtx;        }
    void set_start_vtx_err (const std::vector<double> vtx) { fSigmaStartPos=vtx; }
    void set_end_vtx_err   (const std::vector<double> vtx) { fSigmaEndPos=vtx;   }
    
    inline double       Charge()    const { return fTotalCharge;   }
    inline double       dTdW()      const { return fdTdW;          }
    inline double       dQdW()      const { return fdQdW;          }
    inline double       SigmadTdW() const { return fSigmadTdW;     }
    inline double       SigmadQdW() const { return fSigmadQdW;     }
    inline int          ID()        const { return fID;            }
    inline geo::View_t  View()      const { return fView;          }
    
    inline const std::vector<double>& StartPos()      const { return fStartPos;      }
    inline const std::vector<double>& EndPos()        const { return fEndPos;        }
    inline const std::vector<double>& SigmaStartPos() const { return fSigmaStartPos; }
    inline const std::vector<double>& SigmaEndPos()   const { return fSigmaEndPos;   }
    virtual void clear_data(){
      data_base::clear_data();
      fTotalCharge = -1;
      fdTdW = fdQdW = fSigmadTdW = fSigmadQdW = -1;
      fStartPos.resize(2,0);
      fEndPos.resize(2,0);
      fSigmaStartPos.resize(2,0);
      fSigmaEndPos.resize(2,0);
      fID = -1;
      fView = geo::kUnknown;
    }
    
  private:
    
    double              fTotalCharge;    ///< total charge in cluster
    double              fdTdW;           ///< slope of cluster in tdc vs wire
    double              fdQdW;           ///< slope of cluster in charge vs wire
    double              fSigmadTdW;      ///< slope of cluster in tdc vs wire
    double              fSigmadQdW;      ///< slope of cluster in charge vs wire
    std::vector<double> fStartPos;       ///< start of cluster in (wire, tdc) plane
    std::vector<double> fEndPos;         ///< start of cluster in (wire, tdc) plane
    std::vector<double> fSigmaStartPos;  ///< start of cluster in (wire, tdc) plane
    std::vector<double> fSigmaEndPos;    ///< start of cluster in (wire, tdc) plane
    int                 fID;             ///< cluster's ID
    geo::View_t           fView;           ///< View associated w/ this cluster
    
    ////////////////////////
    ClassDef(cluster,3)
    ////////////////////////
      
  };
  
  /**
     \class event_cluster
     A collection storage class of multiple clusters.
  */
  class event_cluster : public std::vector<larlite::cluster>,
			public event_base {
    
  public:
    
    /// Default constructor
    event_cluster(std::string name="noname") : event_base(data::kCluster,name) {clear_data();}
    
    /// Default copy constructor
    event_cluster(const event_cluster& original)
      : std::vector<larlite::cluster>(original), event_base(original)
    {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
    /// Default destructor
    ~event_cluster(){}

  private:
    
    ////////////////////////
    ClassDef(event_cluster,4)
    ////////////////////////
  };
}
#endif
  
  /** @} */ // end of doxygen group 
  
