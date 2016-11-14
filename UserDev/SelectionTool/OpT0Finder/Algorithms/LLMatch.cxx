#ifndef LLMATCH_CXX
#define LLMATCH_CXX

#include "LLMatch.h"
#include "Base/messenger.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include <TMinuit.h>
#include <TMath.h> // for poisson
#include <cmath>
#include <numeric>

namespace flashana {

    LLMatch *LLMatch::_me = nullptr;

    void MIN_vtx_ll(Int_t &, Double_t *, Double_t &, Double_t *, Int_t);

    LLMatch::LLMatch(const std::string name)
            : BaseFlashMatch(name), _record(false), _normalize(false), _minuit_ptr(nullptr) { }

    void LLMatch::Configure(const ::fcllite::PSet &pset) {
        _record = pset.get<bool>("RecordHistory");
        _normalize = pset.get<bool>("NormalizeHypothesis");
	std::cout << "LLMatch.cxx: _normalize is " << _normalize << std::endl;
    }

    void LLMatch::_Configure_(const Config_t &pset){
      this->Configure(pset);
    }

    FlashMatch_t LLMatch::Match(const QCluster_t &pt_v, const Flash_t &flash) {


        this->CallMinuit(pt_v, flash);

        // Estimate position
        FlashMatch_t res;
        if (isnan(_ll)) return res;
        res.tpc_point.x = res.tpc_point.y = res.tpc_point.z = 0;
	//res.tpc_point_err.clear();
	//res.tpc_point_err.resize(3);
	res.tpc_point_err.x = res.tpc_point_err.y = res.tpc_point_err.z = 0;
        res.score = 1. / _ll;



        double weight = 0;
        for (size_t pmt_index = 0; pmt_index < NOpDets(); ++pmt_index) {

            res.tpc_point.x += OpDetX(pmt_index) * _hypothesis.pe_v[pmt_index];
            res.tpc_point.y += OpDetY(pmt_index) * _hypothesis.pe_v[pmt_index];
            res.tpc_point.z += OpDetZ(pmt_index) * _hypothesis.pe_v[pmt_index];

            weight += _hypothesis.pe_v[pmt_index];
        }

	res.tpc_point.x /= weight;
        res.tpc_point.y /= weight;
        res.tpc_point.z /= weight;
        res.tpc_point.q = weight;

	/*
	   Why do the above for x? We went to the trouble to float x and determine its location. The above
	   writes over that effort.
	 */


	//  This is the TPC x, even if the y,z are for a Flash location
	res.tpc_point.x = _reco_x_offset;
	res.tpc_point_err.x = _reco_x_offset_err;

       	//	std::cout << "LLMatch::Match() res.x, res.dx are " << res.tpc_point.x << ", " << res.tpc_point_err[0] << std::endl;

        return res;
    }

    const Flash_t &LLMatch::ChargeHypothesis(const double xoffset) {
        if (_hypothesis.pe_v.empty()) _hypothesis.pe_v.resize(NOpDets(), 0.);
        if (_hypothesis.pe_v.size() != NOpDets())
          larlite::Message::get()->send( larlite::msg::kERROR, "Hypothesis vector length != PMT count");

	//	std::cout << "ChargeHypothesis: xoffset: " << xoffset << std::endl;

        for (auto &v : _hypothesis.pe_v) v = 0;

        // Apply xoffset
        _var_trk.resize(_raw_trk.size());
        for (size_t pt_index = 0; pt_index < _raw_trk.size(); ++pt_index) {
	  //            std::cout << "x point : " << _raw_trk[pt_index].x << "\t offset : " << xoffset << std::endl;

	  // This means xoffset is the actual trk doca from the x=0 plane, since we've moved it to x=0 previously.
            _var_trk[pt_index].x = _raw_trk[pt_index].x + xoffset;
            _var_trk[pt_index].y = _raw_trk[pt_index].y;
            _var_trk[pt_index].z = _raw_trk[pt_index].z;
            _var_trk[pt_index].t = _raw_trk[pt_index].t;
            _var_trk[pt_index].q = _raw_trk[pt_index].q;
        }

        FillEstimate(_var_trk, _hypothesis);
	OOBCheck(_var_trk, _hypothesis, _measurement);

        if (_normalize) {
            double qsum = std::accumulate(std::begin(_hypothesis.pe_v),
                                          std::end(_hypothesis.pe_v),
                                          0.0);
            for (auto &v : _hypothesis.pe_v) v /= qsum;
        }

        return _hypothesis;
    }

    const Flash_t &LLMatch::Measurement() const { return _measurement; }

    std::vector<double> LLMatch::LL(const Flash_t &hypothesis,
				  const Flash_t &measurement) {

      std::vector<double> result (2,0.0);
      double nvalid_pmt = 0;

        double PEtot_Hyp = 0;
        for (auto const &pe : hypothesis.pe_v)
            PEtot_Hyp += pe;
        double PEtot_Obs = 0;

	// EC, 19-May-2016. We will add to the neg llhd a term which captures the overall probability of
	// fluctuating from the total pe_hyp to total pe_meas, and weight that by the total pe_meas.
	// The reason for this is that it is possible that tracks are shoved to large x values where the probability of a robust track
	// producing a couple photon prediction at each tube gives a probability of observation at each tube that is really not so
	// very unlikely. These bkgd or late-late pmt hits forming small flashes may lead to a flash-match that is (barely) preferred over the true match.
	// However, if we penalize the overall excursion from say 30 hypothesized pe's  to a few measured and reward the 210 excursion to 190, e.g., we may
	// hope to capture the right flash-match.

        for (auto const &pe : measurement.pe_v)
	  {
            PEtot_Obs += pe;
	  }

        if (measurement.pe_v.size() != hypothesis.pe_v.size())
            throw OpT0FinderException("Cannot compute LL for unmatched length!");
        for (size_t pmt_index = 0; pmt_index < hypothesis.pe_v.size(); ++pmt_index) {

	  // if (hypothesis.pe_v[pmt_index] < 0.01) continue; // This seems to be always true of tubes 1 and 30 of 0-31.
	  //	  if ( pmt_index==1) continue; // Is Observed always 0 for this tube?, EC, 14-Feb-2016

            nvalid_pmt += 1;

            auto O = measurement.pe_v[pmt_index]; // observation
            auto H = hypothesis.pe_v[pmt_index];  // hypothesis

	    result.at(0) += std::pow((O - H), 2)/(O + H)  ; // will maximize 1/this
	    result.at(1) +=  -std::log10(TMath::Poisson(O,H))  ; // will maximize 1/this
	    if (isnan(result.at(1)) || isinf(result.at(1))) result.at(1) = 1.E50;
	    // could instead use discrete std::poisson_distribution
	    //	    std::cout << "LL(): ii chi2, llhd, O, H:" << pmt_index << ", " << std::pow((O - H), 2)/(O + H)  << ", " << -std::log10(TMath::Poisson(O,H)) << ", " << O << ", " << H << std::endl;
        }

	//	std::cout << "PE hyp : " << PEtot_Hyp << "\tPE Obs : " << PEtot_Obs << "\t Chi^2, LLHD : " << result.at(0) << ", " << result.at(1) << std::endl;
	//	result.at(1) +=  -std::log10(TMath::Poisson(PEtot_Obs,PEtot_Hyp))*nvalid_pmt   ; // will maximize 1/this
	//	std::cout << "For which the neg llhd of fluctuatation is " <<  -std::log10(TMath::Poisson(PEtot_Obs,PEtot_Hyp)) << std::endl;
	//	result.at(0) += pow(PEtot_Obs-PEtot_Hyp,2.)/(PEtot_Obs+PEtot_Hyp);

	result.at(0) /= (nvalid_pmt ); // *2
        result.at(1) /= (nvalid_pmt + 1); // + 1 for the extra total PE prob
        return result;
    }

    void MIN_vtx_ll(Int_t & /*Npar*/,
                     Double_t * /*Grad*/,
                     Double_t &Fval,
                     Double_t *Xval,
                     Int_t /*Flag*/) {

      //              std::cout << "minuit offset : " << Fval << std::endl;
      //              std::cout << "minuit Xval?? : " << *Xval << std::endl;

        auto const &hypothesis = LLMatch::GetME().ChargeHypothesis(*Xval);
        auto const &measurement = LLMatch::GetME().Measurement();
        Fval = LLMatch::GetME().LL(hypothesis, measurement)[1]; // index 1 is the neg llhd, 0 is the chi2.

        LLMatch::GetME().Record(Fval, Xval[0]);

        return;
    }

    double LLMatch::CallMinuit(const QCluster_t &tpc, const Flash_t &pmt) {

        if (_measurement.pe_v.empty()) {
            _measurement.pe_v.resize(NOpDets(), 0.);
        }
        if (_measurement.pe_v.size() != pmt.pe_v.size())
            throw OpT0FinderException("PMT dimension has changed!");

        //
        // Prepare TPC
        //
        _raw_trk.resize(tpc.size());

        double min_x = 1e9;
        for (size_t i = 0; i < tpc.size(); ++i) {
            auto const &pt = tpc[i];
            _raw_trk[i] = pt;
            if (pt.x < min_x) min_x = pt.x;
        }

	// This shifts the track so that it touches x=0 plane
        for (auto &pt : _raw_trk) pt.x -= min_x;

        //
        // Prepare PMT
        //
        double max_pe = 1.;

        // Debug: Print out expected PE spectrum
        //for(size_t i=0; i<pmt.pe_v.size(); ++i) {
        //std::cout << "PE meas: " << i << " " << pmt.pe_v[i] << std::endl;
        //}

        if (_normalize) {
            max_pe = 0;
            for (auto const &v : pmt.pe_v) if (v > max_pe) max_pe = v;
        }


        for (size_t i = 0; i < pmt.pe_v.size(); ++i)
	  _measurement.pe_v[i] = pmt.pe_v[i] / max_pe;

        _minimizer_record_fval_v.clear();
        _minimizer_record_x_v.clear();

        if (!_minuit_ptr) _minuit_ptr = new TMinuit(4);

        double reco_x = 128.; // 128.;
        double reco_x_err = 0;
        double MinFval;
        int ierrflag, npari, nparx, istat;
        double arglist[4], error[4], Fmin, Fedm, Errdef;
        ierrflag = npari = nparx = istat = 0;

        _minuit_ptr->SetPrintLevel(-1);
        arglist[0] = 2.0;
        _minuit_ptr->mnexcm("SET STR", arglist, 1, ierrflag);

        _minuit_ptr->SetFCN(MIN_vtx_ll);

        _minuit_ptr->DefineParameter(0, "X", reco_x, 10, 0.0, 255.);

        _minuit_ptr->Command("SET NOW");


        // use Migrad minimizer
        arglist[0] = 5000;
        _minuit_ptr->mnexcm("MIGRAD", arglist, 1, ierrflag);

        //arglist[0]   = 5.0e+2;
        //arglist[1]   = 1.0e-6;
        //_minuit_ptr->mnexcm ("simplex",arglist,2,ierrflag);

        _minuit_ptr->GetParameter(0, reco_x, reco_x_err);

        _minuit_ptr->mnstat(Fmin, Fedm, Errdef, npari, nparx, istat);


        // use this for debugging, maybe uncomment the actual minimzing function (MIGRAD / simplex calls)
        // scanning the parameter set
        //arglist[0] = 0;       // Parameter No (in our case x is the only and therefore the 0th parameter)
        //arglist[1] = 500;    // Number of points
        //arglist[2] = 0;       // Start point of scan
        //arglist[3] = 256;     // End point of scan
        //_minuit_ptr->mnexcm("scan", arglist,4, ierrflag);

        MinFval = Fmin;
        double *grad = 0;
        int nPar = 1;
        double fValue[1];
        fValue[0] = reco_x;
        // Transfer the minimization variables:
        MIN_vtx_ll(nPar, grad, Fmin,
                    fValue, ierrflag);
        //static bool show = true;
        /*
        if(show){
          if(Fmin!=MinFval)std::cout<<"Fmin "<<Fmin<<" not equall to "<<MinFval<<std::endl;
          show=false;
        }
        */

        // Transfer the minimization variables:
        _reco_x_offset = reco_x;
        _reco_x_offset_err = reco_x_err;
        _ll = MinFval;

        // Clear:
        _minuit_ptr->mnexcm("clear", arglist, 0, ierrflag);

        if (_minuit_ptr) delete _minuit_ptr;
        _minuit_ptr = 0;

        return _ll;
    }

}
#endif
