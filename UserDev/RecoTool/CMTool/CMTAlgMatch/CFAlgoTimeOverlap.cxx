#ifndef RECOTOOL_CFALGOTIMEOVERLAP_CXX
#define RECOTOOL_CFALGOTIMEOVERLAP_CXX

#include "CFAlgoTimeOverlap.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoTimeOverlap::CFAlgoTimeOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    SetRatioCut(0.001) ; //(0.095) ;  
    SetStartTimeCut(10) ;
    SetDebug(false) ;
    SetVerbose(false) ;
    RequireThreePlanes(true) ;
  }

  //-----------------------------
  void CFAlgoTimeOverlap::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoTimeOverlap::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
    
    // Code-block by Kazu starts
    // This ensures the algorithm works only if # clusters is > 2 (and not =2)
    // You may take out this block if you want to allow matching using clusters from only 2 planes.
    if(_require_3planes && clusters.size()==2) return -1;
    // Code-block by Kazu ends

    double ratio               = 1;
    double time_difference     = 0;
    double max_time_difference = 0;
    double max_charge          = 0;
    double charge_ratio        = 1;        

    //Preserve location in time space. Cut clusters that have similar time differences,
    // but hit wires at very different times. 
    double start_t      = 0;
    double end_t        = 0;
    double prev_start_t = 0;
    double prev_end_t   = 0;

    double max_hits_1 = 0;
    double max_hits_2 = 0;
    
    for(auto const& c : clusters){
      
      auto charge = c->sum_charge ;                
      
      time_difference  = c->start_point.t - c->end_point.t ; 
      
      if(time_difference < 0) 
        time_difference *= -1; 
      
      if(max_time_difference < time_difference)
        max_time_difference = time_difference ;
      
      if(max_charge < charge)
        max_charge = charge ;

      if(c->N_Hits > max_hits_1) {
        max_hits_2 = max_hits_1;
        max_hits_1 = c->N_Hits;
      }else if(c->N_Hits > max_hits_2) 
        max_hits_2 = c->N_Hits;
      
    }

    ratio        = 1;
    charge_ratio = 1;
    for(size_t c_index=0; c_index<clusters.size(); ++c_index) {
      auto const& c = clusters[c_index];
      
      double length = c->length ;
      //auto charge = c->sum_charge ;                
      //Order hits from most to least     
      //SetMaxMiddleMin(hits_0,hits_1,hits_2,max_hits,middle_hits,min_hits);
      
      //Make start_t always smaller
      if(c->start_point.t > c->end_point.t){
        start_t = c->end_point.t   ;
        end_t   = c->start_point.t ;
      }
      else{
        start_t = c->start_point.t ;
        end_t        = c->end_point.t   ;
      }         
      
      if(prev_start_t ==0)
        prev_start_t = start_t ;
      if(prev_end_t ==0)
        prev_end_t = end_t ;
      
      time_difference  = end_t - start_t ; 
      
      ratio *= time_difference/max_time_difference ;
      
      charge_ratio = max_hits_2 / max_hits_1;// charge/max_charge ;

      if(c_index==(clusters.size()-1))
        ratio *= charge_ratio ;
      
      //If current cluster's start time is not within some range of the previous cluster's start time,
      //modify ratio to disallow matching
      
      if( (start_t > (prev_start_t - _start_time_cut) && start_t < (prev_start_t + _start_time_cut)) 
          || (end_t > (prev_end_t - _start_time_cut) && end_t < (prev_end_t + _start_time_cut) )
          || (length >25 && start_t >(prev_start_t - 2*_start_time_cut) && start_t < (prev_start_t + 2*_start_time_cut) ) )
        ratio *= 1;
      else        
        ratio*= 0.001;
      
      prev_start_t = start_t ;
      prev_end_t   = end_t ;        
      
      if( _debug && c_index==(clusters.size()-1) && ratio > _time_ratio_cut ){
        std::cout<<"\nPLANE: "<<c->plane_id.Plane ;
        std::cout<<"\nStart point: "<<start_t<<std::endl;
        std::cout<<"End Point: "<<end_t  <<std::endl;
        //        std::cout<<"Previous start time: "<<prev_start_t<<std::endl;
        std::cout<<"Time diff: "<<time_difference<<std::endl;
        std::cout<<"Max time diff: "<<max_time_difference<<std::endl;
        std::cout<<"Ratio for each cluster: "<<ratio<<std::endl;
        //                                std::cout<<"Charge: "<<charge<<std::endl;
        std::cout<<"Charge Ratio: "<<charge_ratio<<std::endl;
        //std::cout<<"Hits are: "<<min_hits<<", "<<middle_hits<<", "<<max_hits<<std::endl;
        //                                std::cout<<"Adjusted Charge Ratio: "<<adjusted_charge_ratio<<std::endl;
        std::cout<<"Length and Width: "<<c->length<<", "<<c->width<<std::endl;
      } 
      
    }
    
    if(_verbose && ratio> _time_ratio_cut)
      std::cout<<"**************************FOUND A MATCH . ratio is: "<<ratio<<"\n\n\n"<<std::endl;
    
    return (ratio > _time_ratio_cut ? ratio : -1 ); 
    
   }
  
  //------------------------------
  /*
  void CFAlgoTimeOverlap::SetMaxMiddleMin(const double first, 
                                          const double second, 
                                          const double third, 
                                          double &max, 
                                          double &middle, 
                                          double &min)
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
      min = second    ;   
    }   
    
    if(min ==first && second > third){
      middle = third ;
      max = second;
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
  */





  //------------------------------
  void CFAlgoTimeOverlap::Report()
  //------------------------------
  {
  }
    
}
#endif
