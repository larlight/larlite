/**
 * \file mcflux.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mcflux data container
 *
 * @author David C - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_MCFLUX_H
#define LARLITE_MCFLUX_H

#include "data_base.h"
#include <vector>
#include <cmath>
#include "Base/MCConstants.h"
namespace larlite{

  /**
     \class mcflux
     LArSoft MCFlux class equivalent data container
  */
  class mcflux : public data_base {
    
  public:
    
    /// Default constructor
    mcflux();
    
    /// Default destructor
    virtual ~mcflux(){}

    //variables
    int    frun;
    int    fevtno;
    double fndxdz;
    double fndydz;
    double fnpz;
    double fnenergy;
    double fndxdznea;
    double fndydznea;
    double fnenergyn;
    double fnwtnear;
    double fndxdzfar;
    double fndydzfar;
    double fnenergyf;
    double fnwtfar;
    int    fnorig;
    //Decay codes listed here: http://www.hep.utexas.edu/~zarko/wwwgnumi/v19/    
    int    fndecay; 
    // GNUMI decay codes (as in Ndecay)
    /* --------------------------
       Decay type	Decay code
       K0L -> nue pi- e+	1
       K0L -> nuebar pi+ e-	2
       K0L -> numu pi- mu+	3
       K0L -> numubar pi+ mu-	4
       K+ -> numu mu+	        5
       K+ -> nue pi0 e+	        6
       K+ -> numu pi0 mu+	7
       K- -> numubar mu-	8
       K- -> nuebar pi0 e-	9
       K- -> numubar pi0 mu-	10
       mu+ -> numubar nue e+	11
       mu- -> numu nuebar e-	12
       pi+ -> numu mu+	        13
       pi- -> numubar mu-	14
       -------------------------- */
    
    int    fntype;
    double fvx;
    double fvy;
    double fvz;
    double fpdpx;
    double fpdpy;
    double fpdpz;
    double fppdxdz;
    double fppdydz;
    double fpppz;
    double fppenergy;
    int    fppmedium;
    int    fptype;     // converted to PDG
    double fppvx;
    double fppvy;
    double fppvz;
    double fmuparpx;
    double fmuparpy;
    double fmuparpz;
    double fmupare;
    double fnecm;
    double fnimpwt;
    double fxpoint;
    double fypoint;
    double fzpoint;
    double ftvx;
    double ftvy;
    double ftvz;
    double ftpx;
    double ftpy;
    double ftpz;
    int    ftptype;   // converted to PDG
    int    ftgen;
    int    ftgptype;  // converted to PDG
    double ftgppx;
    double ftgppy;
    double ftgppz;
    double ftprivx;
    double ftprivy;
    double ftprivz;
    double fbeamx;
    double fbeamy;
    double fbeamz;
    double fbeampx;
    double fbeampy;
    double fbeampz;
    simb::flux_code_ fFluxType;
    double fgenx;     ///< origin of ray from flux generator
    double fgeny;
    double fgenz;
    double fdk2gen;   ///< distance from decay to ray origin
    double fgen2vtx;  ///< distance from ray origin to event vtx            

    void Reset();
    double Flux(int pdgcode,
                int which=0) const;
    void SetFluxPos(double nue,  double nuebar,
                    double numu, double numubar,
                    double nutau,double nutaubar);
    void SetFluxNeg(double nue,  double nuebar,
                    double numu, double numubar,
                    double nutau,double nutaubar);
    void SetFluxGen(double nue,  double nuebar,
                    double numu, double numubar,
                    double nutau,double nutaubar);

    void ReDecay(double &newE,
                 double &newW,
                 double x,
                 double y,
                 double z);

    void clear_data()
    { data_base::clear_data(); Reset(); }
    
  protected:
                                                                       
    float fFluxPos[6]; ///< e,ebar,mu,mubar,tau,taubar flux, +horn focus
    float fFluxNeg[6]; ///< Fluxes as aboce, for negative horn focus
    float fFluxGen[6]; ///< Fluxes as above, assumed by generator           

  private:
    
  };
  
  /**
     \class event_hit
     A collection storage class of multiple hits.
  */
  class event_mcflux : public std::vector<larlite::mcflux>, 
    public event_base {
      
  public:
    
      /// Default constructor
  event_mcflux(std::string name="noname") : event_base(data::kMCFlux,name) { clear_data(); }
      
      /// Default copy constructor
  event_mcflux(const event_mcflux& original) : std::vector<larlite::mcflux>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_mcflux(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:

  };
}
#endif

/** @} */ // end of doxygen group 
