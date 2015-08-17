#ifndef RECOTOOL_CFALGOZOVERLAP_CXX
#define RECOTOOL_CFALGOZOVERLAP_CXX

#include "CFAlgoZOverlap.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoZOverlap::CFAlgoZOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
		_wire_ratio_cut = 0.1 ;  	//Preliminary cuts
  }

  //-----------------------------
  void CFAlgoZOverlap::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoZOverlap::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //----------------------------------------------------------------------------------------------
  {

    // Code-block by Kazu starts
    // This ensures the algorithm works only if # clusters is > 2 (and not =2)
    // You may take out this block if you want to allow matching using clusters from only 2 planes.
    if(clusters.size()==2) return -1;
    // Code-block by Kazu ends


	double wire_distance 		= 0;
	double ratio 				= 1;
	double max_wire_distance	= -1;

	//Recond the start/end points that retunr the maximum wire distance
	double max_start_w			= -1; 
	double max_end_w			= -1;
	//Record the plane that contains the maximum wire spacing
	int max_plane				= -1; 

	double start_w				= 0;
	double end_w				= 0;
	_verbose					= true ;
	

	for(auto const& c : clusters){

		//...start_point.w in planes 0 and 1 returns a distance in slanted wire space (perp to slanted wires).
		//Rotate this to properly compare to the other planes
		if(c->plane_id.Plane !=2){
				start_w  	= 0.5* c->start_point.w ;
				end_w		= 0.5* c->end_point.w  ;
				wire_distance 	= end_w - start_w 	;
				}
			else {			
				start_w  	=  c->start_point.w ;
				end_w		=  c->end_point.w  ;
				wire_distance   = c->end_point.w - c->start_point.w ;
				}

			if(wire_distance < 0)
				wire_distance *= -1 ;
	
			if(max_wire_distance < wire_distance){
				max_wire_distance = wire_distance;
				max_plane 	=	c->plane_id.Plane;		
				max_start_w =	start_w ;
				max_end_w	=	end_w ;
			   }
	 }


//Calculate maximum z range(accounting for the slant in UV). Then compare start points. Similar
//in this sense, to time.

 	for(auto const& c : clusters){
			
		if(c->plane_id.Plane !=2){
			start_w  	= 0.5* c->start_point.w ;
			end_w		= 0.5* c->end_point.w  ;
			wire_distance 	= end_w - start_w 	;
	  	  }
		else{
			start_w  	=  c->start_point.w ;
			end_w		=  c->end_point.w  ;
			wire_distance   = c->end_point.w - c->start_point.w ;
			}

		if(wire_distance < 0)
			wire_distance *= -1 ;
	
		if(start_w <= max_end_w) // && end_w+25 >=max_start_w )
 			ratio *= wire_distance / max_wire_distance ; 
		else 
			ratio *=0.1 ;	
	
		if(_verbose && ratio > _wire_ratio_cut){
			std::cout<<"\nThe wire distance for cluster in plane "<<c->plane_id.Plane<<" is: "<<wire_distance <<std::endl;
			std::cout<<"Max wire disatance is: "<<max_wire_distance<<std::endl ;	
			std::cout<<"Ratio is: "<<ratio<<std::endl;
			std::cout<<"Start and end points: "<<start_w<<",  "<<end_w<<std::endl;
			}		

		}
//	std::cout<<"******** ** ** **** END OF CLUSTER PERMUTAITON **** ** ** ***********" <<std::endl;	
	if(_verbose && ratio>_wire_ratio_cut)
		std::cout<<" FOOOOUUUUNNNND ONE WOooooooooooooooooooooooooooooooooooooooooooooooooo: "<<ratio<<std::endl;

	return(ratio > _wire_ratio_cut ? ratio : -1) ;

  }

  //------------------------------
  void CFAlgoZOverlap::Report()
  //------------------------------
  {

  }
    
}
#endif
