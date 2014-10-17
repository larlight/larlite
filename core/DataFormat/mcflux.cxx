#ifndef LARLITE_MCFLUX_CXX
#define LARLITE_MCFLUX_CXX

#include "mcflux.h"

namespace larlite {

  mcflux::mcflux() : data_base(data::kMCFlux)
		   , frun(-999)
		   , fevtno(-999)
		   , fndxdz(-999.)
		   , fndydz(-999.)
		   , fnpz(-999.)
		   , fnenergy(-999.)
		   , fndxdznea(-999.)
		   , fndydznea(-999.)
		   , fnenergyn(-999.)
		   , fnwtnear(-999.)
		   , fndxdzfar(-999.)
		   , fndydzfar(-999.)
		   , fnenergyf(-999.)
		   , fnwtfar(-999.)
		   , fnorig(-999)
		   , fndecay(-999)
		   , fntype(-999)
		   , fvx(-999.)
		   , fvy(-999.)
		   , fvz(-999.)
		   , fpdpx(-999.)
		   , fpdpy(-999.)
		   , fpdpz(-999.)
		   , fppdxdz(-999.)
		   , fppdydz(-999.)
		   , fpppz(-999.)
		   , fppenergy(-999.)
		   , fppmedium(-999)
		   , fptype(-999)     // converted to PDG
		   , fppvx(-999.)
		   , fppvy(-999.)
		   , fppvz(-999.)
		   , fmuparpx(-999.)
		   , fmuparpy(-999.)
		   , fmuparpz(-999.)
		   , fmupare(-999.)
		   , fnecm(-999.)
		   , fnimpwt(-999.)
		   , fxpoint(-999.)
		   , fypoint(-999.)
		   , fzpoint(-999.)
		   , ftvx(-999.)
		   , ftvy(-999.)
		   , ftvz(-999.)
		   , ftpx(-999.)
		   , ftpy(-999.)
		   , ftpz(-999.)
		   , ftptype(-999)   // converted to PDG 
		   , ftgen(-999)
		   , ftgptype(-999)  // converted to PDG
		   , ftgppx(-999.)
		   , ftgppy(-999.)
		   , ftgppz(-999.)
		   , ftprivx(-999.)
		   , ftprivy(-999.)
		   , ftprivz(-999.)
		   , fbeamx(-999.)
		   , fbeamy(-999.)
		   , fbeamz(-999.)
		   , fbeampx(-999.)
		   , fbeampy(-999.)
		   , fbeampz(-999.)
		   , fFluxType(simb::kGenerator)
		   , fgenx(-999.)
		   , fgeny(-999.)
		   , fgenz(-999.)
		   , fdk2gen(-999.)
		   , fgen2vtx(-999.)
  {
    for (int i=0; i<6; ++i) fFluxGen[i]=fFluxPos[i]=fFluxNeg[i]= 0;
  }

  void mcflux::Reset(){
    clear_data();
    frun      = -999;
    fevtno    = -999;
    fndxdz    = -999.;
    fndydz    = -999.;
    fnpz      = -999.;
    fnenergy  = -999.;
    fndxdznea = -999.;
    fndydznea = -999.;
    fnenergyn = -999.;
    fnwtnear  = -999.;
    fndxdzfar = -999.;
    fndydzfar = -999.;
    fnenergyf = -999.;
    fnwtfar   = -999.;
    fnorig    = -999;
    fndecay   = -999;
    fntype    = -999;
    fvx       = -999.;
    fvy       = -999.;
    fvz       = -999.;
    fpdpx     = -999.;
    fpdpy     = -999.;
    fpdpz     = -999.;
    fppdxdz   = -999.;
    fppdydz   = -999.;
    fpppz     = -999.;
    fppenergy = -999.;
    fppmedium = -999;
    fptype    = -999;     // converted to PDG
    fppvx     = -999.;
    fppvy     = -999.;
    fppvz     = -999.;
    fmuparpx  = -999.;
    fmuparpy  = -999.;
    fmuparpz  = -999.;
    fmupare   = -999.;
    fnecm     = -999.;
    fnimpwt   = -999.;
    fxpoint   = -999.;
    fypoint   = -999.;
    fzpoint   = -999.;
    ftvx      = -999.;
    ftvy      = -999.;
    ftvz      = -999.;
    ftpx      = -999.;
    ftpy      = -999.;
    ftpz      = -999.;
    ftptype   = -999;   // converted to PDG
    ftgen     = -999;
    ftgptype  = -999;  // converted to PDG
    ftgppx    = -999.;
    ftgppy    = -999.;
    ftgppz    = -999.;
    ftprivx   = -999.;
    ftprivy   = -999.;
    ftprivz   = -999.;
    fbeamx    = -999.;
    fbeamy    = -999.;
    fbeamz    = -999.;
    fbeampx   = -999.;
    fbeampy   = -999.;
    fbeampz   = -999.;
    fgenx     = -999.;
    fgeny     = -999.;
    fgenz     = -999.;
    fdk2gen   = -999.;
    fgen2vtx  = -999.;

    return;
  }


  //......................................................................                                                                     
  ///                                                                                                                                          
  /// Flux of neutrinos at the same energy                                                                                                     
  ///                                                                                                                                          
  /// @param pdgcode : PDG code of neutrino flavor                                                                                             
  /// @param which   : Which flux (generated, plus focus, minus focus)                                                                         
  /// @returns       : #neutrinos/cm^2/10^20 POT                                                                                               
  ///
  double mcflux::Flux(int pdgcode, int which) const
  {
    const float* flux = 0;
    if (which==simb::kGenerator)      flux = fFluxGen;
    if (which==simb::kHistPlusFocus)  flux = fFluxPos;
    if (which==simb::kHistMinusFocus) flux = fFluxNeg;
    if (pdgcode== 12) return flux[0];
    if (pdgcode==-12) return flux[1];
    if (pdgcode== 14) return flux[2];
    if (pdgcode==-14) return flux[3];
    if (pdgcode== 16) return flux[4];
    if (pdgcode==-16) return flux[5];
    return 0.0;
  }

  //......................................................................                                                                     
  void mcflux::SetFluxPos(double nue,  double nuebar,
                          double numu, double numubar,
                          double nutau,double nutaubar)
  {
    fFluxPos[0] = nue;   fFluxPos[1] = nuebar;
    fFluxPos[2] = numu;  fFluxPos[3] = numubar;
    fFluxPos[4] = nutau; fFluxPos[5] = nutaubar;
  }


  //......................................................................                                                                     

  void mcflux::SetFluxNeg(double nue,  double nuebar,
                          double numu, double numubar,
                          double nutau,double nutaubar)
  {
    fFluxNeg[0] = nue;   fFluxNeg[1] = nuebar;
    fFluxNeg[2] = numu;  fFluxNeg[3] = numubar;
    fFluxNeg[4] = nutau; fFluxNeg[5] = nutaubar;
  }

  //......................................................................                                                                     
  void mcflux::SetFluxGen(double nue,  double nuebar,
                          double numu, double numubar,
                          double nutau,double nutaubar)
  {
    fFluxGen[0] = nue;   fFluxGen[1] = nuebar;
    fFluxGen[2] = numu;  fFluxGen[3] = numubar;
    fFluxGen[4] = nutau; fFluxGen[5] = nutaubar;
  }

  //......................................................................                                                                     
  void mcflux::ReDecay(double &newE, double &newW, double x, double y, double z)
  {
    //note x,y,z are assumed to be in cm                                                                                                       
    //x,y,z are also assumed to be in the beam reference frame                                                                                 
    //i.e. 0,0,0 are at the target,                                                                                                            
    //z points along the beamline                                                                                                              
    //right handed coords, x points to the left if you are looking down the beam                                                               

    //these constants should probably be defined elsewhere.                                                                                    
    //I'll put them here until I figure out where they should properly go                                                                      
    const double pimass=.13957;
    const double mumass=0.105658389;
    const double kmass=0.49368;
    const double k0mass=0.49767;
    double mass=pimass;

    //determine parent particle mass based on fptype PDG code                                                                                  
    if(fptype==13||fptype==-13){
      mass = mumass;
    }
    else if(fptype==211||fptype==-211){
      mass = pimass;
    }
    else if(fptype==321||fptype==-321){
      mass = kmass;
    }
    else if(fptype==130){
      mass = k0mass;
    }
    else{
      std::cerr<<"dont know ptype "<<fptype<<"can not compute new decay weight"<<std::endl;
      return;
    }

    //compute kinematics of parent particle at decay point                                                                                     
    double p=sqrt(1.*fpdpx*fpdpx+1.*fpdpy*fpdpy+1.*fpdpz*fpdpz);
    double Eplab=sqrt(1.*fpdpx*fpdpx+1.*fpdpy*fpdpy+1.*fpdpz*fpdpz+1.*mass*mass);
    double gamma = Eplab/mass;
    double beta = sqrt((gamma*gamma-1)/(gamma*gamma));

    //compute components of vector between decay point                                                                                         
    //and the point you're aiming at                                                                                                           
    double rnx=1.*(x-fvx);
    double rny=1.*(y-fvy);
    double rnz=1.*(z-fvz);
    double rn=sqrt(rnx*rnx+rny*rny+rnz*rnz);

    //compute angle between parent momentum                                                                                                    
    //and where we want the neutrino to go                                                                                                     
    double rndotp = (rnx*fpdpx+rny*fpdpy+rnz*fpdpz);
    double costhetan = rndotp/(rn*p);

    //do some checking of the calculation                                                                                                      
    if(std::abs(costhetan)>1){
      if(costhetan>0){
        costhetan = 1;
      }
      else{
        costhetan=-1;
      }
    }

    //now compute the weights                                                                                                                  
    double MN=1.;
    if(p>0){//if it didn't decay at rest                                                                                                       
      //boost                                                                                                                                  
      MN=1./(gamma*(1-beta*costhetan));
    }


    newE = MN*fnecm;
    //  std::cout<<"New E "<<newE<<" old ecm "<<fnecm<<" mn "<<MN<<" gamma "<<gamma<<" beta "<<beta<<" costhetan "<<costhetan<<std::endl;      

    //solid angle                                                                                                                              
    // small angle approximation: // double san = 10000./(4*rn*rn);                                                                            
    // Alex Radovic's removal of small angle approximation                                                                                     
    const double kRDET = 100.;
    double san = (1.0-cos(atan( kRDET / rn )))/2.0;

    //  std::cout<<"san "<<san<<" fvz-z "<<fvz-z<<std::endl;                                                                                   
    newW = san*MN*MN;
    //  std::cout<<"mn "<<MN<<std::endl;                                                                                                       


    //if its a (polarized) muon decay, we have modify the weight                                                                               
    if(fptype==13||fptype==-13){
      double betav[3]={0.};
      double partialn=0.;
      double partial=0.;
      double P_nun[3]={0.};
      double P_dcm_nun[4]={0.};
      double P_pcm_mp[4]={0.};

      betav[0] = fpdpx/Eplab;
      betav[1] = fpdpy/Eplab;
      betav[2] = fpdpz/Eplab;

      P_nun[0] = rnx*newE/rn;
      P_nun[1] = rny*newE/rn;
      P_nun[2] = rnz*newE/rn;

      partialn =gamma*(betav[0]*P_nun[0]+betav[1]*P_nun[1]+betav[2]*P_nun[2]);
      partialn = newE - partialn /(gamma+1.);

      P_dcm_nun[0] = P_nun[0] - betav[0]*gamma*partialn;
      P_dcm_nun[1] = P_nun[1] - betav[1]*gamma*partialn;
      P_dcm_nun[2] = P_nun[2] - betav[2]*gamma*partialn;
      P_dcm_nun[3] = sqrt(pow(P_dcm_nun[0],2)
                          +pow(P_dcm_nun[1],2)
                          +pow(P_dcm_nun[2],2));


      gamma = fppenergy/mass;
      betav[0] = fppdxdz*fpppz/fppenergy;
      betav[1] = fppdydz*fpppz/fppenergy;
      betav[2] = fpppz/fppenergy;

      partial = gamma*(betav[0]*fmuparpx +
                       +betav[1]*fmuparpy + betav[2]*fmuparpz);
      partial = fmupare-partial/(gamma+1.);
      P_pcm_mp[0] = fmuparpx - betav[0]*gamma*partial;
      P_pcm_mp[1] = fmuparpy - betav[1]*gamma*partial;
      P_pcm_mp[2] = fmuparpz - betav[2]*gamma*partial;
      P_pcm_mp[3] = sqrt(pow(P_pcm_mp[0],2)+
                         pow(P_pcm_mp[1],2)+
                         pow(P_pcm_mp[2],2));

      //calc new  decay angle w.r.t. (anti)spin direction                                                                                      
      double costhn  = 0.;
      if(P_dcm_nun[3]!=0&&P_pcm_mp[3]!=0){
        costhn = ( P_dcm_nun[0]*P_pcm_mp[0]+
                   P_dcm_nun[1]*P_pcm_mp[1]+
                   P_dcm_nun[2]*P_pcm_mp[2])/(P_dcm_nun[3]*P_pcm_mp[3]);
      }

      if(std::abs(costhn)>1){
        if(costhn>0){
          costhn = 1;
        }
        else{
          costhn=-1;
        }
      }

      double wt_ration;
      if(fntype==14||fntype==-14){
        double xnu = 2.*fnecm/mass;
        wt_ration = ( (3.-2.*xnu) - (1.-2.*xnu)*costhn ) / (3.-2.*xnu);
      }
      else{
        wt_ration=1.-costhn;
      }

      newW*=wt_ration;
    }

    return;
  }

}
#endif
  
