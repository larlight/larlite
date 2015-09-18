/**
 * \file CFAlgoTimeProf.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CFAlgoTimeProf
 *
 * @author ryan
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CFALGOTIMEPROF_H
#define RECOTOOL_CFALGOTIMEPROF_H

#include "CMTool/CMToolBase/CFloatAlgoBase.h"
#include "LArUtil/LArUtil-TypeDef.h"
namespace cmtool {
  /**
     \class CFAlgoTimeProf
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoTimeProf : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoTimeProf();
    
    /// Default destructor
    virtual ~CFAlgoTimeProf();

    //
    // Author should be aware of 3 functions at least: Float, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    virtual float Float(const std::vector<const cluster::cluster_params*> &clusters);

    virtual void Report();
    
    /// Function to reset the algorithm instance, called together with manager's Reset()
    virtual void Reset();

    /**
       Optional function: called at the beginning of 1st iteration. This is called per event.
     */
    //virtual void EventBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

    /**
       Optional function: called at the end of event ... after the last merging iteration is over.
     */
    //virtual void EventEnd();
 
    /**
       Optional function: called at the beggining of each iterative loop.
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    //virtual void IterationBegin(const std::vector<cluster::ClusterParamsAlgNew> &clusters);

    /**
       Optional function: called at the end of each iterative loop.
     */
    //virtual void IterationEnd();

   private:
	
    float TProfCompare(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb);	


    /*
(Form("sig_a"),Form("sig_a"),nts,0,nts);
    TH1D* siginta(Form("sigint_a"),Form("sigint_a"),nts,0,nts);
    TH1D* sigb(Form("sig_b"),Form("sig_b"),nts,0,nts);
    TH1D* sigintb(Form("sigint_b"),Form("sigint_b"),nts,0,nts);
    */	

  };
}
#endif
/** @} */ // end of doxygen group 

