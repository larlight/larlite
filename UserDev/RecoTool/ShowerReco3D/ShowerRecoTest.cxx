#ifndef RECOTOOL_SHOWERRECOTEST_CXX
#define RECOTOOL_SHOWERRECOTEST_CXX

#include "ShowerRecoTest.h"

namespace showerreco {
  
  ShowerRecoTest::ShowerRecoTest() : ShowerRecoAlgBase(), fGSer(nullptr)
  {
    
    if(!fGSer) fGSer = (larutil::GeometryUtilities*)(larutil::GeometryUtilities::GetME());
    
    fcalodEdxlength=1000;
    fdEdxlength=2.4;
    fUseArea = true;
    fVerbosity = true;
    _Ecorrection = true;
  }


  ::larlite::shower ShowerRecoTest::RecoOneShower(const std::vector< ::showerreco::ShowerCluster_t>& clusters)
  {
    
    ::larlite::shower result;
    //
    // Reconstruct and store
    //
    std::vector < larutil::PxPoint > fStartPoint;    // for each plane
    std::vector < larutil::PxPoint > fEndPoint;    // for each plane
    std::vector < double > fOmega2D;    // for each plane
    
    std::vector < double > fEnergy;    // for each plane
    std::vector < double > fMIPEnergy;    // for each plane
    std::vector < double > fdEdx;      
    std::vector <unsigned char> fPlaneID;
    
    // First Get Start Points
    for(auto const & cl : clusters)
      {
        fStartPoint.push_back(cl.start_point);    // for each plane
	fEndPoint.push_back(cl.end_point);    // for each plane
        fOmega2D.push_back(cl.angle_2d);
	fPlaneID.push_back(cl.plane_id);
	/*
	if(fVerbosity) {
	  std::cout << " planes : " << cl.plane_id
		    << " " << cl.start_point.t 
		    << " " << cl.start_point.w 
		    << " " << cl.end_point.t 
		    << " " << cl.end_point.w 
		    <<" angle2d "<<  cl.angle_2d  
		    << std::endl;   
		    }*/
      }
    
    
    //decide best two planes. for now, using length in wires - flatter is better.
    
    int worst_plane=-1,best_plane=-1;
    double min_length=9999999;
    double max_length=0;
    for (size_t ip=0;ip<fPlaneID.size();ip++)
      {
	if(fabs( fEndPoint[ip].w - fStartPoint[ip].w ) < min_length )
	  {
	    min_length=fabs( fEndPoint[ip].w - fStartPoint[ip].w );
	    worst_plane=fPlaneID.at(ip);
	  }

	if(fabs( fEndPoint[ip].w - fStartPoint[ip].w ) > max_length )
	  {
	    max_length=fabs( fEndPoint[ip].w - fStartPoint[ip].w );
	    best_plane=fPlaneID.at(ip);
	  }      
      }
    
    if(fVerbosity)
      std::cout << " worst plane is : " << worst_plane << " best : " << best_plane   << std::endl;
    
    int index_to_use[2]={0,1};  // for a two plane detector we have no choice.
    if(fPlaneID.size()>2)
      {
	int local_index=0;
	for(size_t ip=0;ip<fPlaneID.size();ip++) 
	  {
	    if(fStartPoint[ip].plane!=worst_plane) 
	      index_to_use[local_index++]=ip;
	  }
	
      }
               
    
    // Second Calculate 3D angle and effective pitch and start point 
    
    double xphi=0,xtheta=0;
    
    
    fGSer->Get3DaxisN(index_to_use[0],
		      index_to_use[1],
		      fOmega2D[index_to_use[0]]*TMath::Pi()/180.,
		      fOmega2D[index_to_use[1]]*TMath::Pi()/180.,
		      xphi,
		      xtheta);
    /*    
    if(fVerbosity)
      std::cout << " new angles: " << xphi << " " << xtheta << std::endl; 
    */
    //std::cout << std::setprecision(2) << std::fixed;    
    
    double xyz[3];
    // calculate start point here?
    fGSer-> GetXYZ(&(fStartPoint[index_to_use[0]]),&(fStartPoint[index_to_use[1]]),xyz);

    // Calculate all 3 start point candidates:
    double start_UV[3], start_UY[3], start_VY[3];
    fGSer-> GetXYZ(&(fStartPoint[0]),&(fStartPoint[1]),start_UV);
    fGSer-> GetXYZ(&(fStartPoint[0]),&(fStartPoint[2]),start_UY);
    fGSer-> GetXYZ(&(fStartPoint[1]),&(fStartPoint[2]),start_VY);
    double end_UV[3], end_UY[3], end_VY[3];
    fGSer-> GetXYZ(&(fEndPoint[0]),&(fEndPoint[1]),end_UV);
    fGSer-> GetXYZ(&(fEndPoint[0]),&(fEndPoint[2]),end_UY);
    fGSer-> GetXYZ(&(fEndPoint[1]),&(fEndPoint[2]),end_VY);
    // Calculate distance between start points:
    geoalgo::Point_t s_UV(start_UV[0],start_UV[1],start_UV[2]);
    geoalgo::Point_t s_UY(start_UY[0],start_UY[1],start_UY[2]);
    geoalgo::Point_t s_VY(start_VY[0],start_VY[1],start_VY[2]);
    double dist_UV_UY = s_UV.Dist(s_UY);
    double dist_UV_VY = s_UV.Dist(s_VY);
    double dist_UY_VY = s_UY.Dist(s_VY);
    if (fVerbosity) {
      std::cout << "S Point UV: [" << s_UV[0] << ", " << s_UV[1] << ", " << s_UV[2] << "]  -->  "
		<< "End Point UV: [" << end_UV[0] << ", " << end_UV[1] << ", " << end_UV[2] << "]" << std::endl;
      std::cout << "S Point UY: [" << s_UY[0] << ", " << s_UY[1] << ", " << s_UY[2] << "]  -->  "
		<< "End Point UV: [" << end_UY[0] << ", " << end_UY[1] << ", " << end_UY[2] << "]" << std::endl;
      std::cout << "S Point VY: [" << s_VY[0] << ", " << s_VY[1] << ", " << s_VY[2] << "]  -->  "
		<< "End Point UV: [" << end_VY[0] << ", " << end_VY[1] << ", " << end_VY[2] << "]" << std::endl;
      std::cout << "Distance UV-UY: " << dist_UV_UY << std::endl;
      std::cout << "Distance UV-VY: " << dist_UV_VY << std::endl;
      std::cout << "Distance UY-VY: " << dist_UY_VY << std::endl;
    }
 

    // Find a way to choose the best start point
    // for now find minimum of the 3 distances. Start point should be half-way
    double dMin = 1036;
    geoalgo::Point_t startPoint(3);
    if ( (dist_UV_UY < dist_UV_VY) && (dist_UV_UY < dist_UY_VY) ){
      dMin = dist_UV_UY;
      startPoint = (s_UV + s_UY)/2. ;
    }
    else if ( dist_UV_VY < dist_UY_VY ){
      dMin = dist_UV_VY;
      startPoint = (s_UV + s_VY)/2. ;
    }
    else{
      dMin = dist_UY_VY;
      startPoint = (s_UY + s_VY)/2. ;
    }
    

    xyz[0] = startPoint[0];
    xyz[1] = startPoint[1];
    xyz[2] = startPoint[2];
    
    if (fVerbosity) { std::cout << "Reco Start Point: [" << startPoint[0] << ", " << startPoint[1] << ", " << startPoint[2] << "]" << std::endl; }
	  
    
    // Third calculate dE/dx and total energy for all planes, because why not?
    //for(auto const &clustit : clusters)
    for(size_t cl_index=0; cl_index < fPlaneID.size(); ++cl_index) 
      {
	int plane = fPlaneID.at(cl_index);
	double newpitch=fGSer->PitchInView(plane,xphi,xtheta);
	/*
	if(fVerbosity)
	  std::cout << std::endl << " Plane: " << plane << std::endl;
	*/
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
	  //double Bcorr_half;
	  //double dEdx_sub;
	  // double dEdx_MIP;
	  
	  //    dEdx_new = fCaloAlg.dEdx_AREA(theHit, newpitch );
	  //Bcorr_half = 2.*fCaloAlg.dEdx_AREA(theHit->Charge()/2.,theHit->PeakTime(), newpitch, plane); ; 
	  //dEdx_sub = fCaloAlg.dEdx_AREA(theHit->Charge()-PION_CORR,theHit->PeakTime(), newpitch, plane); ; 
	  // dEdx_MIP = fCaloAlg.dEdx_AREA_forceMIP(theHit, newpitch ); 
	  if(!fUseArea)
	    {
	      dEdx_new = fCaloAlg.dEdx_AMP(theHit.peak / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
	      hitElectrons = fCaloAlg.ElectronsFromADCPeak(theHit.peak, plane);
	    }
	  else
	    {
	      dEdx_new = fCaloAlg.dEdx_AREA(theHit.charge / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
	      hitElectrons = fCaloAlg.ElectronsFromADCArea(theHit.charge, plane);
	    }

	  hitElectrons *= fCaloAlg.LifetimeCorrection(theHit.t / fGSer->TimeToCm());
	  
	  totEnergy += hitElectrons * 1.e3 / (::larutil::kGeVToElectrons);

	  //totNewCnrg+=dEdx_MIP;
	//  if(dEdx_new < 3.5 && dEdx_new >0 )
	//    {
	//      totLowEnergy +=dEdx_new*newpitch;
	//      totMIPEnergy += dEdx_new*newpitch;
	//    }
	//  else
	 //   {
	 //     totHighEnergy += dEdx_new*newpitch;
	      int multiplier=1;
	      if(plane < 2) multiplier =2 ;
	      if(!fUseArea){
		totMIPEnergy += theHit.peak *  0.0061*multiplier;
	      }
	      else{
		totMIPEnergy += theHit.charge *  0.00336*multiplier;
	      }
	  //  }
	  /*
	   if(fVerbosity && dEdx_new >1.9 && dEdx_new <2.1)
	    std::cout << "dEdx_new " << dEdx_new << " " <<dEdx_new/theHit.charge*newpitch << " "<< theHit.charge *0.0033*multiplier/newpitch << std::endl;
	  */
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
	  /*
	  if(fVerbosity && dEdx_new < 3.5 )
	    std::cout << " CALORIMETRY:" 
		      << " Pitch " <<newpitch 
		      << " dist: " << wdist 
		      << " dE/dx: " << dEdx_new << "MeV/cm "  
		      << " average: " <<  totEnergy  
		      << "hit: wire, time " << theHit.w/fGSer->WireToCm() << " " << theHit.t/fGSer->TimeToCm() 
		      << "total energy" << totEnergy 
		      << std::endl;
	  */
	  //   if( (fabs(wdist)<fcalodEdxlength)&&(fabs(wdist)>0.2)){  
	  if( (wdist<fcalodEdxlength)&&(wdist>0.2)){    
	    
	    //vdEdx.push_back(dEdx_new);
	    //vresRange.push_back(fabs(wdist));
	    //vdQdx.push_back(theHit->Charge(true)/newpitch);
	    //Trk_Length=wdist;
	    
	    //fTrkPitchC=fNPitch[set][plane];
	    //Kin_En+=dEdx_new*newpitch;
	    //npoints_calo++;
	    //sum+=dEdx_new;
	    
	    
	    
	    //fDistribChargeADC[set].push_back(ch_adc);  //vector with the first De/Dx points
	    //     fDistribChargeMeV[set].push_back(dEdx_new);  //vector with the first De/Dx points
	    //     fDistribHalfChargeMeV[set].push_back(Bcorr_half);
	    //     fDistribChargeposition[set].push_back(wdist);  //vector with the first De/Dx points' positions 
	    //     fDistribChargeMeVMIPsub[set].push_back(dEdx_sub);
	    // 	
	    
	    
	    //first pass at average dE/dx
	    if(wdist<fdEdxlength 
	       //take no hits before vertex (depending on direction)
	       && ((direction==1 && theHit.w > fStartPoint.at(cl_index).w) || (direction==-1 && theHit.w < fStartPoint.at(cl_index).w) )
	       && ortdist<3.5 
	       && linedist < fdEdxlength ){
	      
	      dEdx_av+= dEdx_new;
	      local_hitlist.push_back(theHit);
	      
	      npoints_first++;
	      /*
	      if(fVerbosity)
		std::cout << " CALORIMETRY:" << " Pitch " <<newpitch 
			  << " dist: " << wdist 
			  << " dE/dx: " << dEdx_new << "MeV/cm "  
			  << " average: " <<  dEdx_av/npoints_first  
			  << " hit: wire, time " << theHit.w << " " << theHit.t 
			  << " line,ort " << linedist << " " << ortdist
			  << " direction " << direction 
			  << std::endl;
	      */
	    }
	    
	  }//end inside range if statement
	  
	}// end first loop on hits.
	
	
	
	double mevav2cm=0;
	double fRMS_corr=0;
	
	//calculate average dE/dx
	if(npoints_first>0)	{
	  mevav2cm=dEdx_av/npoints_first;
	  
	}
	
	//loop only on subset of hits
	for(auto const & theHit : local_hitlist){
	  double dEdx=0;
	  if(fUseArea)
	    { 
	      dEdx = fCaloAlg.dEdx_AREA(theHit.charge / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
	    }
	  else  //this will hopefully go away, once all of the calibration factors are calculated.
	    {
	      dEdx = fCaloAlg.dEdx_AMP(theHit.peak / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
	    }
	  //fDistribAfterMin[set].push_back(MinBefore);
	  //fDistribBeforeMin[set].push_back(MinAfter);
	  //  auto position = hitIter - local_hitlist.begin() ;	
	  
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
	      dEdx = fCaloAlg.dEdx_AREA(theHit.charge / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
	    }
	  else  //this will hopefully go away, once all of the calibration factors are calculated.
	    {
	      dEdx = fCaloAlg.dEdx_AMP(theHit.peak / newpitch, theHit.t / fGSer->TimeToCm(), theHit.plane);
	    }
	  //fDistribAfterMin[set].push_back(MinBefore);
	  //fDistribBeforeMin[set].push_back(MinAfter);
	  
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
	/*
	if(fVerbosity){ 
	  ////std::cout << " total ENERGY, birks: " << fTotChargeMeV[set] << " MeV " << " assumeMIPs:  " << fTotChargeMeV_MIPs[set] << "MeV " <<  std::endl;
	  std::cout << " total ENERGY, birks: " << totEnergy << " MeV "  
		    << " |average:  " << dedx_final  << std::endl
		    << " Energy:  lo:" << totLowEnergy << " hi: " << totHighEnergy 
		    << " MIP corrected " << totMIPEnergy << std::endl;
	}
	*/
	// if Energy correction factor to be used
	// then get it from ShowerCalo.h
	if (_Ecorrection) { totEnergy *= showerreco::energy::DEFAULT_ECorr; }
	fEnergy.push_back(totEnergy);    // for each plane
	fMIPEnergy.push_back(totMIPEnergy);
	fdEdx.push_back(dedx_final); 
    
	// break;
      } // end loop on clusters

    result.set_total_MIPenergy(fMIPEnergy);
    result.set_total_best_plane(best_plane);
    result.set_total_energy(fEnergy);
    //result.set_total_energy_err  (std::vector< Double_t > q)            { fSigmaTotalEnergy = q;        }
    
    double dirs[3]={0};
    fGSer->GetDirectionCosines(xphi,xtheta,dirs);
    TVector3 vdirs(dirs[0],dirs[1],dirs[2]);
    
    TVector3 vxyz(xyz[0], xyz[1], xyz[2]);
    
    result.set_direction(vdirs); 
    //result.set_direction_err (TVector3 dir_e)      { fSigmaDCosStart = dir_e; }
    result.set_start_point(vxyz); 
    //result.set_start_point_err (TVector3 xyz_e)      { fSigmaXYZstart = xyz_e; }
    result.set_dedx  (fdEdx);
    //result.set_dedx_err  (std::vector< Double_t > q)            { fSigmadEdx = q;        }

    // done
    return result;
    }

}

#endif
