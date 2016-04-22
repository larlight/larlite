#ifndef RECOTOOL_SHOWERRECOALG_CXX
#define RECOTOOL_SHOWERRECOALG_CXX

#include "ShowerRecoAlg.h"

namespace showerreco {
  
  ShowerRecoAlg::ShowerRecoAlg() : ShowerRecoAlgBase(), fGSer(nullptr)
  {
    
    if(!fGSer) fGSer = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());
    
    fcalodEdxlength=1000;
    fdEdxlength  =2.4;
    fUseArea     = true;
    _linearE     = false;
    _verbosity   = true;
    _Ecorrection = true;

    _linearE   = false;
  }


  Shower_t ShowerRecoAlg::RecoOneShower(const ProtoShower& proto_shower)
  {
   
    auto clusters = proto_shower.params();
    
    Shower_t result;
    //
    // Reconstruct and store
    //
    std::vector < larutil::PxPoint > fStartPoint;    // for each plane
    std::vector < larutil::PxPoint > fEndPoint;    // for each plane
    std::vector < double > fOmega2D;    // for each plane
    std::vector < double > fOpeningAngles; // for each plane
    std::vector < double > fEnergy   (fGSer->Nplanes(),-1);    // for each plane
    std::vector < double > fMIPEnergy(fGSer->Nplanes(),-1);    // for each plane
    std::vector < double > fdEdx     (fGSer->Nplanes(),-1);      
    std::vector <unsigned char> fPlaneID;
    
    // First Get Start Points
    for(auto const & cl : clusters)
      {
        fStartPoint.push_back(cl.start_point);    // for each plane
        fEndPoint.push_back(cl.end_point);    // for each plane
        fOmega2D.push_back(cl.angle_2d);
        fOpeningAngles.push_back(cl.opening_angle);
        fPlaneID.push_back(cl.plane_id.Plane);
        if(_verbosity) {
          std::cout << " planes : " << cl.plane_id.Plane
                    << " " << cl.start_point.t 
                    << " " << cl.start_point.w 
                    << " " << cl.end_point.t 
                    << " " << cl.end_point.w 
                    <<" angle2d "<<  cl.angle_2d  
                    << std::endl;   
        }
      }
    
    
    //decide best two planes. for now, using length in wires - flatter is better.
    int index_to_use[2]={0,1}; 
    int best_plane=-1;
    int good_plane=-1;
    double best_length=0;
    double good_length=0;
    double best_opening_angle=0;
    for (size_t ip=0;ip<fPlaneID.size();ip++)
      {
        double dist = fabs( fEndPoint[ip].w - fStartPoint[ip].w );
        if(dist > best_length )
          {
            good_length = best_length;
            good_plane  = best_plane;
            index_to_use[1] = index_to_use[0];

            best_length = dist;
            best_plane = fPlaneID.at(ip);
            index_to_use[0] = ip;
          }
        else if(dist > good_length)
          {
            good_length = dist;
            good_plane  = fPlaneID.at(ip);
            index_to_use[1] = ip;
          }
      }
    
    // For now, pick the "best plane" and save the 2d opening angle as the shower's 3d opening angle
    // This needs to be improved... 2d angles are not 2d angles!
    best_opening_angle = fOpeningAngles[index_to_use[0]];
    
    // Second Calculate 3D angle and effective pitch and start point 
    double xphi=0,xtheta=0;
    
    fGSer->Get3DaxisN(fPlaneID[index_to_use[0]],
                      fPlaneID[index_to_use[1]],
                      fOmega2D[index_to_use[0]]*TMath::Pi()/180.,
                      fOmega2D[index_to_use[1]]*TMath::Pi()/180.,
                      xphi,
                      xtheta);
    
    if(_verbosity)
      std::cout << " new angles: " << xphi << " " << xtheta << std::endl; 
    
    
    double xyz[3];
    // calculate start point here?
    fGSer-> GetXYZ(&(fStartPoint[index_to_use[0]]),&(fStartPoint[index_to_use[1]]),xyz);
    
    if(_verbosity)
      std::cout << " XYZ:  " << xyz[0] << " " << xyz[1] << " " << xyz[2] << std::endl;
    
    // Third calculate dE/dx and total energy for all planes, because why not?
    //for(auto const &clustit : clusters)
    for(size_t cl_index=0; cl_index < fPlaneID.size(); ++cl_index) 
      {
        
        // reset TTree variables
        _pl = -1;
        _dedx_v.clear();
        _dedx = -1;

        int plane = fPlaneID.at(cl_index);
        double newpitch=fGSer->PitchInView(plane,xphi,xtheta);
        if(plane == best_plane) best_length *= newpitch / fGSer->WireToCm();

        if(_verbosity)
          std::cout << std::endl << " Plane: " << plane << std::endl;
        
        double totEnergy=0;
        double totLowEnergy=0;
        double totHighEnergy=0;
        double totMIPEnergy=0;
        int direction=-1;
        //double RMS_dedx=0;
        double dEdx_av=0,dedx_final=0;
        int npoints_first=0, npoints_sec=0;
        
        //override direction if phi (XZ angle) is less than 90 degrees 
        // this is shady, and needs to be rethought.
        if(fabs(fOmega2D.at(cl_index)) < 90)
          direction=1;
        
        auto const& hitlist = clusters.at(cl_index).hit_vector;
        std::vector<larutil::PxHit> local_hitlist;
        local_hitlist.reserve(hitlist.size());

        for(const auto& theHit : hitlist){
          
          double dEdx_new=0;
          double hitElectrons = 0;

          if(!fUseArea)
            {
              dEdx_new = fCaloAlg->dEdx_AMP(theHit.peak / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
              hitElectrons = fCaloAlg->ElectronsFromADCPeak(theHit.peak, plane);
            }
          else
            {
              dEdx_new = fCaloAlg->dEdx_AREA(theHit.charge / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
              hitElectrons = fCaloAlg->ElectronsFromADCArea(theHit.charge, plane);
            }
          
          // if we want to use a linear dQ->dE enrgy conversion instead of Box or Birks models
          if (_linearE){
            hitElectrons *= fCaloAlg->LifetimeCorrection(theHit.t / fGSer->TimeToCm());
            // convert to MeV energy scale
            totEnergy += hitElectrons * 1.e3 / (::larutil::kGeVToElectrons);
          }
          else {
            // make sure we aren't adding a crazy amount
            // this is totally possible due to non-linear
            // birks / box model formulas
            if (dEdx_new > 10.) { dEdx_new = 10.; }
            totEnergy += dEdx_new*newpitch;
          }
          
          int multiplier=1;
          if(plane < 2) multiplier =2 ;
          if(!fUseArea){
            totMIPEnergy += theHit.peak *  0.0061*multiplier;
          }
          else{
            totMIPEnergy += theHit.charge *  0.00336*multiplier;
          }
          
          if(_verbosity && dEdx_new >1.9 && dEdx_new <2.1)
            std::cout << "dEdx_new " << dEdx_new << " " <<dEdx_new/theHit.charge*newpitch << " "<< theHit.charge *0.0033*multiplier/newpitch << std::endl;
          
          larutil::PxPoint OnlinePoint; 
          // calculate the wire,time coordinates of the hit projection on to the 2D shower axis
          fGSer->GetPointOnLine(fOmega2D.at(cl_index),
                                &(fStartPoint.at(cl_index)),
                                &theHit,
                                OnlinePoint);
          
          double ortdist=fGSer->Get2DDistance(&OnlinePoint,&theHit);
          double linedist=fGSer->Get2DDistance(&OnlinePoint,&(fStartPoint.at(cl_index)));
          
          //calculate the distance from the vertex using the effective pitch metric 
          double wdist=((theHit.w-fStartPoint.at(cl_index).w)*newpitch)*direction;  //wdist is always positive
          
          if(_verbosity && dEdx_new < 3.5 )
            std::cout << " CALORIMETRY:" 
                      << " Pitch " <<newpitch 
                      << " dist: " << wdist 
                      << " dE/dx: " << dEdx_new << "MeV/cm "  
                      << " average: " <<  totEnergy  
                      << "hit: wire, time " << theHit.w/fGSer->WireToCm() << " " << theHit.t/fGSer->TimeToCm() 
                      << "total energy" << totEnergy 
                      << std::endl;
          
          //   if( (fabs(wdist)<fcalodEdxlength)&&(fabs(wdist)>0.2)){  
          if( (wdist<fcalodEdxlength)&&(wdist>0.2)){    
            
            //first pass at average dE/dx
            if(wdist<fdEdxlength 
               //take no hits before vertex (depending on direction)
               && ((direction==1 && theHit.w > fStartPoint.at(cl_index).w) || (direction==-1 && theHit.w < fStartPoint.at(cl_index).w) )
               && ortdist<3.5 
               && linedist < fdEdxlength ){
              
              dEdx_av+= dEdx_new;
              local_hitlist.push_back(theHit);
              
              npoints_first++;
              if(_verbosity)
                std::cout << " CALORIMETRY:" << " Pitch " <<newpitch 
                          << " dist: " << wdist 
                          << " dE/dx: " << dEdx_new << "MeV/cm "  
                          << " average: " <<  dEdx_av/npoints_first  
                          << " hit: wire, time " << theHit.w << " " << theHit.t 
                          << " line,ort " << linedist << " " << ortdist
                          << " direction " << direction 
                          << std::endl;
            }
            
          }//end inside range if statement
          
        }// end first loop on hits.
        
        
        
        double mevav2cm=0;
        double fRMS_corr=0;
        
        //calculate average dE/dx
        if(npoints_first>0)        {
          mevav2cm=dEdx_av/npoints_first;
          
        }
        
        //loop only on subset of hits
        for(auto const & theHit : local_hitlist){
          double dEdx=0;
          if(fUseArea)
            { 
              dEdx = fCaloAlg->dEdx_AREA(theHit.charge / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
            }
          else  //this will hopefully go away, once all of the calibration factors are calculated.
            {
              dEdx = fCaloAlg->dEdx_AMP(theHit.peak / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
            }
          fRMS_corr+=(dEdx-mevav2cm)*(dEdx-mevav2cm); 
        }
        
        if(npoints_first>0)        
          {
            fRMS_corr=TMath::Sqrt(fRMS_corr/npoints_first);
          }
        
        /// third loop to get only points inside of 1RMS of value.      
        //loop only on subset of hits
        
        for(auto const & theHit : local_hitlist){
          double dEdx=0;
          if(fUseArea)
            { 
              dEdx = fCaloAlg->dEdx_AREA(theHit.charge / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
              _dedx_v.push_back(dEdx);
            }
          else  //this will hopefully go away, once all of the calibration factors are calculated.
            {
              dEdx = fCaloAlg->dEdx_AMP(theHit.peak / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
              _dedx_v.push_back(dEdx);
            }
          
          if( ((dEdx > (mevav2cm-fRMS_corr) ) && (dEdx < (mevav2cm+fRMS_corr) )) 
              || 
              (newpitch > 0.3*fdEdxlength ) 
              ) {
            dedx_final+= dEdx; 
            npoints_sec++;
          }
        }
        
        if(npoints_sec>0){
          dedx_final/=npoints_sec;
        }
        
        if(_verbosity){ 
          std::cout << " total ENERGY, birks: " << totEnergy << " MeV "  
                    << " |average:  " << dedx_final  << std::endl
                    << " Energy:  lo:" << totLowEnergy << " hi: " << totHighEnergy 
                    << " MIP corrected " << totMIPEnergy << std::endl;
        }
        // if Energy correction factor to be used
        // then get it from ShowerCalo.h
        if (_Ecorrection) { totEnergy *= showerreco::energy::DEFAULT_ECorr; }
        fEnergy[plane]    = totEnergy;    // for each plane
        fMIPEnergy[plane] = totMIPEnergy;
        fdEdx[plane]      = dedx_final; 

        _pl = plane;
        _dedx = dedx_final;
    
        // break;
      } // end loop on clusters

    result.fTotalMIPEnergy_v  = fMIPEnergy;
    result.fBestPlane.Plane = best_plane;
    result.fLength          = best_length;
    result.fOpeningAngle    = best_opening_angle;
    result.fTotalEnergy_v     = fEnergy;

    double dirs[3]={0};
    fGSer->GetDirectionCosines(xphi,xtheta,dirs);
    TVector3 vdirs(dirs[0],dirs[1],dirs[2]);
    
    TVector3 vxyz(xyz[0], xyz[1], xyz[2]);
    
    result.fDCosStart = vdirs;
    result.fXYZStart  = vxyz;
    result.fdEdx_v      = fdEdx;
    
    // done
    return result;
    }

}

#endif
