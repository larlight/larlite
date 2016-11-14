/**
 * \file LLMatch.h
 *
 * \ingroup Algorithms
 *
 * \brief Class def header for a class LLMatch
 *
 * @author echurch
 */

/** \addtogroup Algorithms

    @{*/
#ifndef LLMATCH_H
#define LLMATCH_H

#include <iostream>
#include "OpT0Finder/Base/BaseFlashMatch.h"
#include <TMinuit.h>
namespace flashana {
  /**
     \class LLMatch
     User defined class LLMatch ... these comments are used to generate
     doxygen documentation!
  */
  class LLMatch : public BaseFlashMatch {

  public:

    /// Default constructor
    LLMatch(const std::string="LLMatch");

    /// Default destructor
    ~LLMatch(){}

    void Configure(const ::fcllite::PSet &pset);

    /// Singleton shared instance getter
    static LLMatch& GetME()
    {
      if(!_me) _me = new LLMatch;
      return *_me;
    }

    /// Core function: execute matching
    FlashMatch_t Match(const QCluster_t&, const Flash_t&);

    const Flash_t& ChargeHypothesis(const double);
    const Flash_t& Measurement() const;

    std::vector<double> LL(const flashana::Flash_t&,
			   const flashana::Flash_t&);

    void Record(const double fval, const double x)
    {
      if(_record) {
	_minimizer_record_fval_v.push_back(fval);
	_minimizer_record_x_v.push_back(x);
      }
    }

    double CallMinuit(const QCluster_t& tpc,
		      const Flash_t& pmt);

    const std::vector<double>& HistoryLL() const { return _minimizer_record_fval_v; }
    const std::vector<double>& HistoryX()   const { return _minimizer_record_x_v;    }

    const double Bestx() { return _reco_x_offset; }
    const double Bestx_err() { return _reco_x_offset_err; }

  protected:

    void _Configure_(const Config_t &pset);

  private:

    static LLMatch* _me;

    bool _record;      ///< Boolean switch to record minimizer history
    double _normalize; ///< Noramalize hypothesis PE spectrum

    flashana::QCluster_t _raw_trk;
    flashana::QCluster_t _var_trk;
    flashana::Flash_t    _hypothesis;  ///< Hypothesis PE distribution over PMTs
    flashana::Flash_t    _measurement; ///< Flash PE distribution over PMTs

    std::vector<double> _minimizer_record_fval_v; ///< Minimizer record charge likelihood value
    std::vector<double> _minimizer_record_x_v;    ///< Minimizer record X values

    double _reco_x_offset;     ///< reconstructed X offset (from wire-plane to min-x point)
    double _reco_x_offset_err; ///< reconstructed X offset w/ error
    double _ll;               ///< Charge likelihood value

    TMinuit* _minuit_ptr;

  };
}

#endif
/** @} */ // end of doxygen group
