/**
 * \file QLLMatch.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class QLLMatch
 *
 * @author kazuhiro
 */

/** \addtogroup Algorithms

    @{*/
#ifndef QLLMATCH_H
#define QLLMATCH_H

#include <iostream>
#include "OpT0Finder/Base/BaseFlashMatch.h"
#include <TMinuit.h>
namespace flashana {
  /**
     \class QLLMatch
     User defined class QLLMatch ... these comments are used to generate
     doxygen documentation!
  */
  class QLLMatch : public BaseFlashMatch {
    
  public:
    
    /// Default constructor
    QLLMatch();
    
    /// Default destructor
    ~QLLMatch(){}

    static QLLMatch& GetME()
    {
      if(!_me) _me = new QLLMatch;
      return *_me;
    }

    FlashMatch_t Match(const QCluster_t&, const Flash_t&);

    void SetOpDetPositions( const std::vector<double>&,
			    const std::vector<double>&,
			    const std::vector<double>&);

    void Record(bool doit=true) { _record=doit;}

    void Normalize(bool doit=true) { _normalize=doit;}

    void UsePhotonLibrary(bool use=true);

    const std::vector<double>& ChargeHypothesis(const QCluster_t&);

    const  QCluster_t& VarTrack(double x);

    double QLL(const std::vector<double>&);

    void Record(const double fval, const double x)
    {
      if(_record) {
	_minimizer_record_fval_v.push_back(fval);
	_minimizer_record_x_v.push_back(x);
      }
    }

    double CallMinuit(const QCluster_t& tpc,
		      const Flash_t& pmt);

    const std::vector<double>& HistoryQLL() const { return _minimizer_record_fval_v; }
    const std::vector<double>& HistoryX()   const { return _minimizer_record_x_v;    }

  private:

    static QLLMatch* _me;

    bool _record;
    bool _normalize;
    std::vector<double> _pmt_x_v;
    std::vector<double> _pmt_y_v;
    std::vector<double> _pmt_z_v;

    QCluster_t _raw_trk;
    QCluster_t _var_trk;

    std::vector<double> _qll_hypothesis_v;
    std::vector<double> _flash_pe_v;

    std::vector<double> _minimizer_record_fval_v;
    std::vector<double> _minimizer_record_x_v;
    
    double _reco_x;
    double _reco_x_err;
    double _qll;

    TMinuit* _minuit_ptr;

    bool _use_library;

  };
}

#endif
/** @} */ // end of doxygen group 

