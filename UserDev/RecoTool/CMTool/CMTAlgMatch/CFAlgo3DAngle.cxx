#ifndef RECOTOOL_CFALGO3DANGLE_CXX
#define RECOTOOL_CFALGO3DANGLE_CXX

#include "CFAlgo3DAngle.h"
#include "LArUtil/GeometryHelper.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgo3DAngle::CFAlgo3DAngle() : CFloatAlgoBase()
  //-------------------------------------------------------
  {

	SetDebug(false) ;
	SetThetaCut(30) ;
	SetPhiCut(40)  ;
	SetRatio(0.0005) ;

  }

  //-----------------------------
  void CFAlgo3DAngle::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgo3DAngle::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
    // Code-block by Kazu starts
    // This ensures the algorithm works only if # clusters is > 2 (and not =2)
    // You may take out this block if you want to allow matching using clusters from only 2 planes.
    if(clusters.size()==2 || clusters.size()==1) return -1;
    // Code-block by Kazu ends


	int plane_0 = clusters.at(0)->plane_id.Plane;
	int plane_1 = clusters.at(1)->plane_id.Plane;
	int plane_2 = clusters.at(2)->plane_id.Plane;
	double angle_2d_0 = clusters.at(0)->angle_2d;
	double angle_2d_1 = clusters.at(1)->angle_2d;
	double angle_2d_2 = clusters.at(2)->angle_2d;
	auto hits_0 = clusters.at(0)->N_Hits ;
	auto hits_1 = clusters.at(1)->N_Hits ;
	auto hits_2 = clusters.at(2)->N_Hits ;

	auto sumCharge0 = clusters.at(0)->sum_charge; 
	auto sumCharge1 = clusters.at(1)->sum_charge; 
	auto sumCharge2 = clusters.at(2)->sum_charge; 
	
	//Calculate angles theta and phi for cluster pairs across 2 planes
	//Theta goes from -90 to 90, phi from -180 to 180
	double phi_01    = 0;   	
	double theta_01  = 0;
	double phi_12    = 0;   
	double theta_12  = 0;
	double phi_02 	 = 0;
	double theta_02  = 0;

	double max_phi(0), middle_phi(0), min_phi(0);
	double max_theta(0), middle_theta(0), min_theta(0); 
	//When theta1, theta2, phi ratios are calcualted, find best 2
	double max_ratio(0), middle_ratio(0), min_ratio(0);

	//For ordering hits and rejecting pairs which have a large difference in hit number
	double max_hits(0), middle_hits(0), min_hits(0) ;
//	double max_hits1(0), middle_hits1(0), min_hits1(0) ;

	//Calculate phi and theta from first 2 planes; check if third plane is consistent
	larutil::GeometryHelper::GetME()->Get3DAxisN(plane_0,plane_1,angle_2d_0,angle_2d_1,phi_01,theta_01);	
	larutil::GeometryHelper::GetME()->Get3DAxisN(plane_1,plane_2,angle_2d_1,angle_2d_2,phi_12,theta_12);	
	larutil::GeometryHelper::GetME()->Get3DAxisN(plane_2,plane_0,angle_2d_2,angle_2d_0,phi_02,theta_02);

	//Adjust the range of phis/thetas that are bigger than 360 or less than 0.
	FixPhiTheta(phi_01,theta_01);
	FixPhiTheta(phi_12,theta_12);
	FixPhiTheta(phi_02,theta_02);
	
	//Order phi and theta for ratio calculation later
	SetMaxMiddleMin(phi_01,phi_12,phi_02,max_phi,middle_phi,min_phi);
	SetMaxMiddleMin(theta_01,theta_12,theta_02,max_theta,middle_theta,min_theta);

	//Order hits from most to least 	
	SetMaxMiddleMin(hits_0,hits_1,hits_2,max_hits,middle_hits,min_hits);

	//Ratio for hits	
	double ratio_max_min    = 1;
	double ratio_max_middle = 1;

	//Ratio for theta angles
	double ratio_theta1 = 1;
	double ratio_theta2 = 1; 

	//Ratio for phi--only 1 ratio because 2 of the angles come from collection plane 2
	//and are thus the same. 
	double ratio_phi   = 1;

	//Total ratio
	double ratio 		= 1;		

	//This takes into account the fact that 0 and 360 having the same relative value (for phi; 0 and 180 for theta)
	for(int i=0; i<2 ; i++){
		while(min_phi + 360 < max_phi +_phi_cut && min_phi +360 > max_phi - _phi_cut)
    	  {	
			min_phi +=360 ;
			SetMaxMiddleMin(max_phi, middle_phi, min_phi,max_phi,middle_phi,min_phi);
		  }
	  }

	ratio_theta2 = min_theta / max_theta;
	ratio_theta1 = middle_theta / max_theta; 
	ratio_phi = min_phi / max_phi ;

	//Get the biggest ratios for the thetas and phi
	SetMaxMiddleMin(ratio_phi, ratio_theta1, ratio_theta2, max_ratio, middle_ratio, min_ratio);
	
	ratio_max_min = min_hits / max_hits ;
	ratio_max_middle = middle_hits / max_hits ;	

	ratio =  max_ratio * sumCharge1* sumCharge2 * sumCharge0; //*  ; //* ratio_phi ; //* ratio_max_middle ; 

	//Test to make sure that max hits is not too much bigger than min
	if( ratio_max_min <0.3)
		ratio *= ratio_max_min ;

	//GeometryUtilities returns theta=-999 when 2d Angle=0--Don't know
	//how else to deal with that. This seems to work.
	if( theta_01 == -999 || theta_12 == -999 || theta_02 == -999)
		return -1 ;


	if(_debug && ratio > _ratio_cut ){
		std::cout<<"\nNhits planes 0, 1, 2: " <<clusters.at(0)->N_Hits<<", "<<clusters.at(1)->N_Hits
				 <<", "<<clusters.at(2)->N_Hits ;
		std::cout<<"\nTheta1 , Theta2 : "<<ratio_theta1<<", "<<ratio_theta2;
		std::cout<<"\nPhi Ratio: "<<ratio_phi;
		std::cout<<"\nHits ratio mid : "<<ratio_max_middle ;
	//	std::cout<<"\nHits ratio min : "<<ratio_max_min ;
		std::cout<<"\nTotal is: " <<ratio<<" ***************";


		std::cout<<"\n\n0: 2dAngle: "<<clusters.at(0)->angle_2d<<std::endl;
		std::cout<<"1: 2dAngle: "<<clusters.at(1)->angle_2d<<std::endl;
		std::cout<<"2: 2dAngle: "<<clusters.at(2)->angle_2d<<std::endl;
		std::cout<<"\nTheta,Phi MaxMM : "<<max_theta<<", "<<middle_theta<<", "<<min_theta<<"\n\t\t"
				 <<max_phi<<", "<<middle_phi<<", "<<min_phi;
		
		std::cout<<"\nStart End Points:  "<<clusters.at(0)->start_point.t<<", "<<clusters.at(0)->end_point.t<<"\n\t\t"
		<<clusters.at(1)->start_point.t<<", "<<clusters.at(1)->end_point.t<<"\n\t\t "
		<<clusters.at(2)->start_point.t<<", "<<clusters.at(2)->end_point.t;

/*		std::cout<<"\nFor planes 0 and 1: "<<std::endl ;
		std::cout<<"\tPhi : "<<phi_01<<std::endl;
		std::cout<<"\tTheta : "<<theta_01<<std::endl ;
		std::cout<<"For planes 1 and 2: "<<std::endl ;
		std::cout<<"\tPhi : "<<phi_12<<std::endl;
		std::cout<<"\tTheta : "<<theta_12<<std::endl ;
		std::cout<<"For plane 0 and 2: "<<std::endl ;
		std::cout<<"\tPhi : "<<phi_02<<std::endl ;
		std::cout<<"\tTheta : "<<theta_02<<std::endl; */
	//	std::cout<<"\nNEW CLUSTERS PAIRS NOW\n\n\n"<<std::endl<<std::endl;
		std::cout<<"\n\n\n";
	}
	
	return(ratio > _ratio_cut ? ratio : -1) ;
  }

 //--------------------------------
  void CFAlgo3DAngle::FixPhiTheta(double &phi, double &theta)
 //--------------------------------	
   {
	//	while(phi <= 0)
	//		phi += 360 ;
	//	while(phi > 720)
	//		phi -= 360 ;
//		if(phi <=0)

		phi+=360;

		if(theta != -999)
			theta += 180 ;

	}	


  //------------------------------
  void CFAlgo3DAngle::SetMaxMiddleMin(const double first, const double second, const double third, double &max, double &middle, double &min) 
  //------------------------------
  {

	if(first > second && first > third){
		max = first;
		}
	else if (first > second && first < third){
		max = third ;
		middle = first ;
		min = second ;
		}
	else if (first > third && first < second){
		max = second ;
		middle = first ;
		min = third ; 
		}
	else if(first <second && first <third)
		min = first ;
	

	if (max == first && second > third){
		middle = second ; 
		min = third    ;
		}
	else if (max ==first && third > second){
		middle = third ;	
		min = second 	;
		}

	if(min ==first && second > third){
		middle = third ;
		max	= second;
		}
	else if(min ==first && third > second){
		middle = second ;
		max = third ;
	    }


	//Very rarely, the angles(or hits) may be equal
	if( first == second && first > third ){
		max = first;
		middle = second ;
		min = third ;
		}
	else if( first == second && first < third){
		max = third;
		middle = first ;
		min = second ;
		}
	
	else if( first ==third && first > second){
		max = first;
		middle = third; 	
		min = second; 
		}
	
	else if( first == third && first < second){
		max = second ;
		middle = first;
		min = third ;
		}

	else if( second ==third && second < first){
		max = first;
		middle = third; 	
		min = second; 
		}
	
	else if( second == third && second > first){
		max = second;
		middle = third;
		min = first ;
		}
	

}


  //------------------------------
  void CFAlgo3DAngle::Report()
  //------------------------------
  {

  }
    
}
#endif
