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
#include "OpT0Finder/Base/FlashMatchFactory.h"
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

    enum QLLMode_t { kChi2, kLLHD, kLLR };

  private:
    /// Valid ctor hidden (singleton)
    QLLMatch(const std::string);
    
  public:
    
    /// Default ctor throws exception (singleton)
    QLLMatch();
    
    /// Default destructor
    ~QLLMatch(){}

    /// Singleton shared instance getter
    static QLLMatch* GetME()
    {
      if(!_me) _me = new QLLMatch("QLLMatch");
      return _me;
    }

    /// Core function: execute matching
    FlashMatch_t Match(const QCluster_t&, const Flash_t&);

    const Flash_t& ChargeHypothesis(const double);
    const Flash_t& Measurement() const;

    double QLL(const flashana::Flash_t&,
	       const flashana::Flash_t&);

    void Record(const double x)
    {
      if(_record) {
	_minimizer_record_chi2_v.push_back(_current_chi2);
	_minimizer_record_llhd_v.push_back(_current_llhd);
	_minimizer_record_x_v.push_back(x);
      }
    }

    double CallMinuit(const QCluster_t& tpc,
		      const Flash_t& pmt,
		      const bool init_x0=true);

    const std::vector<double>& HistoryLLHD() const { return _minimizer_record_llhd_v; }
    const std::vector<double>& HistoryChi2() const { return _minimizer_record_chi2_v; }
    const std::vector<double>& HistoryX()    const { return _minimizer_record_x_v;    }


  protected:

    void _Configure_(const Config_t &pset);
    
  private:

    FlashMatch_t PESpectrumMatch(const QCluster_t &pt_v, const Flash_t &flash, const bool init_x0);

    FlashMatch_t OnePMTMatch(const Flash_t &flash);

    static QLLMatch* _me;
    
    QLLMode_t _mode;   ///< Minimizer mode
    bool _record;      ///< Boolean switch to record minimizer history
    double _normalize; ///< Noramalize hypothesis PE spectrum

    std::vector<double>  _penalty_threshold_v;
    std::vector<double>  _penalty_value_v;

    flashana::QCluster_t _raw_trk;
    QPoint_t _raw_xmin_pt;
    QPoint_t _raw_xmax_pt;
    flashana::QCluster_t _var_trk;
    flashana::Flash_t    _hypothesis;  ///< Hypothesis PE distribution over PMTs
    flashana::Flash_t    _measurement; ///< Flash PE distribution over PMTs
    double               _current_xoffset; ///< Last Minuit value for x-offset parameter
    double               _xoffset_penalty_sigma; //< sigma used in x-offset penalty term
    bool                 _minuit_penalize_x0_deviations;

    double _current_chi2;
    double _current_llhd;
    std::vector<double> _minimizer_record_chi2_v; ///< Minimizer record chi2 value
    std::vector<double> _minimizer_record_llhd_v; ///< Minimizer record llhd value
    std::vector<double> _minimizer_record_x_v;    ///< Minimizer record X values

    double _reco_x_offset;     ///< reconstructed X offset (from wire-plane to min-x point)
    double _reco_x_offset_err; ///< reconstructed X offset w/ error
    double _qll;               ///< Minimizer return value

    TMinuit* _minuit_ptr;

    double _recox_penalty_threshold;
    double _recoz_penalty_threshold;

    double _onepmt_score_threshold;
    double _onepmt_xdiff_threshold;
    double _onepmt_pesum_threshold;
    double _onepmt_pefrac_threshold;
    
    // flags to indicate when cosmic discriminator corrections should be applied
    bool   _cosmic_disc_correction;
    bool   _apply_cosmic_disc_threshold;
    bool   _skip_nodata_bins;
    double _cosmic_disc_threshold;
    bool   _run_migrad;
    
  };

  /**
     \class flashana::QLLMatchFactory
  */
  class QLLMatchFactory : public FlashMatchFactoryBase {
  public:
    /// ctor
    QLLMatchFactory() { FlashMatchFactory::get().add_factory("QLLMatch",this); }
    /// dtor
    ~QLLMatchFactory() {}
    /// creation method
    BaseFlashMatch* create(const std::string instance_name) { return QLLMatch::GetME(); }
  };
  
}

#endif
/** @} */ // end of doxygen group 

