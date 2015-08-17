#ifndef REFINEDIRECTION_CXX
#define REFINEDIRECTION_CXX

#include "RefineDirection.h"
#include "CRUException.h"
#include "LArUtil/GeometryHelper.h"

const float PI = 3.14159265;

namespace cluster {

  RefineDirection::RefineDirection(){
    
    _percentage = 0.9;
    _Nbins      = 200;
    _Qmin       = 25;
    _name       = "RefineDirection";

  }

  void RefineDirection::do_params_fill(cluster_params & cluster){

    // Geometry Utilities
    auto geomHelper = ::larutil::GeometryHelper::GetME();

    larutil::PxPoint this_startPoint, this_endPoint;

    this_startPoint = cluster.start_point;
    this_endPoint   = cluster.end_point;

    if(_verbose) {
      std::cout << "Angle: Start point: (" << this_startPoint.w 
                << ", " << this_startPoint.t << ")\n";
      std::cout << "Angle: End point  : (" << this_endPoint.w   
                << ", " << this_endPoint.t << ")\n";
    }

    double endStartDiff_x = (this_endPoint.w - this_startPoint.w);
    double endStartDiff_y = (this_endPoint.t - this_startPoint.t);
    double rms_forward   = 0;
    double rms_backward  = 0;
    double mean_forward  = 0;
    double mean_backward = 0;
    //double weight_total  = 0;
    double hit_counter_forward  = 0;
    double hit_counter_backward = 0;
    
    if (_verbose && endStartDiff_y == 0 && endStartDiff_x == 0) {
      std::cerr << "Error:  end point and start point are the same!\n";

      return;
    }

    double percentage_HC = _percentage*cluster.N_Hits_HC/cluster.N_Hits;
    const double wgt = 1.0/cluster.N_Hits;

    // Containers for the angle histograms
    std::vector<float> opening_angle_bin(_Nbins,0.0 ) ;
    std::vector<float> closing_angle_bin(_Nbins,0.0 ) ;
    std::vector<float> opening_angle_highcharge_bin(_Nbins,0.0 ) ;
    std::vector<float> closing_angle_highcharge_bin(_Nbins,0.0 ) ;
    std::vector<float> opening_angle_chargeWgt_bin(_Nbins,0.0 ) ;
    std::vector<float> closing_angle_chargeWgt_bin(_Nbins,0.0 ) ;

    int index = -1;

    // Calculate a Rough Charge Profile
    // This is basically a set of two numbers, containing info
    // on how much charge is in each half of the shower
    // Each hit's charget is added to either bin depending on whether
    // the perpendicular projection of the hit onto the 2D slope
    // of the cluster falls in the 1st or 2nd half.
    // the charge of the cluster is weighted by the perpendicular
    // distance to the 2Dslope line.
    float Q_begin = 0; ///< Charge (weighted) in the 1st half of the cluster
    float Q_end   = 0; ///< Charge (weighted) in the 2nd half of the cluster

    // calculate a cluster length as distance from start and end point
    float clusterLength = geomHelper->Get2DDistance(cluster.start_point,cluster.end_point);

    for(auto& hit : cluster.hit_vector) {
      index ++;

      // keep information on a hit's projected point on the cluster's slope
      Point2D hitProjectedOnLine;
      geomHelper->GetPointOnLine(cluster.slope_2d,cluster.start_point,hit,hitProjectedOnLine);
      // calculate orthogonal distance between the hit and the slope line
      float orthDist = geomHelper->Get2DDistance(hitProjectedOnLine,hit);
      // distance from the start of the cluster
      float lineDist = geomHelper->Get2DDistance(cluster.start_point,hitProjectedOnLine);

      // calculate a weight for this hit
      // w = Q                (if orthDist < 1 i.e. close to trunk)
      // w = Q/(2*orthDist)   (if orthDist > 1 i.e. far from trunk)
      float weight = (orthDist < 1.) ? (hit.charge) : (hit.charge) / (orthDist/2.);
      
      // figure out if in 1st or 2nd half of cluster
      if ( (lineDist*2) > clusterLength )
        Q_end   += weight;
      else
        Q_begin += weight;
      
      //skip this hit if below minimum cutoff param
      if(hit.charge < _Qmin) continue;
      
      // Compute forward mean
      double hitStartDiff_x = (hit.w - this_startPoint.w) ;
      double hitStartDiff_y = (hit.t - this_startPoint.t) ;
      
      if (hitStartDiff_x == 0 && hitStartDiff_y == 0) continue;

      double cosangle_start = (endStartDiff_x*hitStartDiff_x + 
                               endStartDiff_y*hitStartDiff_y);
      cosangle_start /= ( pow(pow(endStartDiff_x,2)+pow(endStartDiff_y,2),0.5)
                        * pow(pow(hitStartDiff_x,2)+pow(hitStartDiff_y,2),0.5));

      if(cosangle_start>0) {


        // Only take into account for hits that are in "front"
        //no weighted average, works better as flat average w/ min charge cut
        mean_forward += cosangle_start;
        rms_forward  += pow(cosangle_start,2);
        hit_counter_forward++;
      }

      // Compute backward mean
      double hitEndDiff_x = (hit.w - this_endPoint.w);
      double hitEndDiff_y = (hit.t - this_endPoint.t);
      if (hitEndDiff_x == 0 && hitEndDiff_y == 0) continue;

      double cosangle_end  = (endStartDiff_x*hitEndDiff_x +
                              endStartDiff_y*hitEndDiff_y) * (-1.);
      cosangle_end /= ( pow(pow(endStartDiff_x,2)+pow(endStartDiff_y,2),0.5) 
                      * pow(pow(hitEndDiff_x,2)+pow(hitEndDiff_y,2),0.5));
      
      if(cosangle_end>0) {

        //no weighted average, works better as flat average w/ min charge cut
        mean_backward += cosangle_end;
        rms_backward  += pow(cosangle_end,2);
        hit_counter_backward++;
      }

      int N_bins_OPEN = (_Nbins-1) * acos(cosangle_start)/PI;
      int N_bins_CLOSE = (_Nbins-1) * acos(cosangle_end)/PI;
      if (N_bins_OPEN  < 0) N_bins_OPEN  = 0;
      if (N_bins_CLOSE < 0) N_bins_CLOSE = 0;

      if (_verbose){
        std::cout << "endStartDiff_x :" << endStartDiff_x << "\n"
                  << "endStartDiff_y :" << endStartDiff_y << "\n"
                  << "cosangle_start :" << cosangle_start << "\n"
                  << "cosangle_end   :" << cosangle_end << "\n"
                  << "N_bins_OPEN    :" << N_bins_OPEN << "\n"
                  << "N_bins_CLOSE   :" << N_bins_CLOSE << "\n"
                  << std::endl; 
      }

      opening_angle_chargeWgt_bin[N_bins_OPEN ] 
                    += hit.charge/cluster.sum_charge;
      closing_angle_chargeWgt_bin[N_bins_CLOSE] 
                    += hit.charge/cluster.sum_charge;
      opening_angle_bin[N_bins_OPEN] += wgt;
      closing_angle_bin[N_bins_CLOSE] += wgt;

      //Also fill bins for high charge hits
      if(hit.charge > cluster.mean_charge){
        opening_angle_highcharge_bin[N_bins_OPEN] += wgt;
        closing_angle_highcharge_bin[N_bins_CLOSE] += wgt;
      }

    }

    //initialize iterators for the angles
    int iBin(0), jBin(0), kBin(0), lBin(0), mBin(0), nBin(0);  
    double percentage_OPEN(0.0),
           percentage_CLOSE(0.0),
           percentage_OPEN_HC(0.0),
           percentage_CLOSE_HC(0.0),  //The last 2 are for High Charge (HC)
           percentage_OPEN_CHARGEWGT(0.0),
           percentage_CLOSE_CHARGEWGT(0.0); 

    for(iBin = 0; percentage_OPEN<= _percentage && iBin < _Nbins; iBin++)
    {
      percentage_OPEN += opening_angle_bin[iBin];
    }

    for(jBin = 0; percentage_CLOSE<= _percentage && jBin < _Nbins; jBin++)
    {
      percentage_CLOSE += closing_angle_bin[jBin];
    }

    for(kBin = 0; percentage_OPEN_HC<= percentage_HC && kBin < _Nbins; kBin++)
    {
      percentage_OPEN_HC += opening_angle_highcharge_bin[kBin];
    }

    for(lBin = 0; percentage_CLOSE_HC<= percentage_HC && lBin < _Nbins; lBin++)
    {
      percentage_CLOSE_HC += closing_angle_highcharge_bin[lBin];
    }

    for(mBin = 0; percentage_OPEN_CHARGEWGT<= _percentage && mBin < _Nbins; mBin++)
    {
      percentage_OPEN_CHARGEWGT += opening_angle_chargeWgt_bin[mBin];
    }

    for(nBin = 0; percentage_CLOSE_CHARGEWGT<= _percentage && nBin < _Nbins; nBin++)
    {
      percentage_CLOSE_CHARGEWGT += closing_angle_chargeWgt_bin[nBin];
    }

    double opening_angle = iBin * PI /_Nbins ;
    double closing_angle = jBin * PI /_Nbins ;
    double opening_angle_highcharge = kBin * PI /_Nbins ;
    double closing_angle_highcharge = lBin * PI /_Nbins ;
    double opening_angle_charge_wgt = mBin * PI /_Nbins ;
    double closing_angle_charge_wgt = nBin * PI /_Nbins ;

    if (_verbose){
      std::cout<<"opening angle: "<<opening_angle << "\n"
               <<"closing angle: "<<closing_angle << "\n"
               <<"opening high charge angle: "<<opening_angle_highcharge << "\n"
               <<"closing high charge angle: "<<closing_angle_highcharge << "\n"
               <<"opening high charge wgt  : "<<opening_angle_charge_wgt << "\n"
               <<"closing high charge wgt  : "<<closing_angle_charge_wgt << "\n"
               <<"fCoarseChargeProfile     : "<<Q_begin << "\n"
               <<"Q_end                    : " << Q_end << std::endl;
    }

    double value_1 = closing_angle/opening_angle -1;
    double value_2 = Q_begin/Q_end;//(fCoarseChargeProfile[0]/fCoarseChargeProfile[1]);
    
    if (value_2 < 100.0 && value_2 > 0.01)
      value_2 = log(value_2);
    else
      value_2 = 0.0;
    double value_3 = closing_angle_charge_wgt/opening_angle_charge_wgt -1;

    if (_verbose){
      std::cout << "value_1 : " << value_1 << "\n"
                << "value_2 : " << value_2 << "\n"
                << "value_3 : " << value_3 << "\n"
                << std::endl;
    }

    // Using a sigmoid function to determine flipping.
    // I am going to weigh all of the values above (1, 2, 3) equally.
    // But, since value 2 in particular, I'm going to cut things off at 5.
    
    if (value_1 > 3  ) value_1 = 3.0;
    if (value_1 < -3 ) value_1 = -3.0;
    if (value_2 > 3  ) value_2 = 3.0;
    if (value_2 < -3 ) value_2 = -3.0;
    if (value_3 > 3  ) value_3 = 3.0;
    if (value_3 < -3 ) value_3 = -3.0;

    double Exp = exp(value_1 + value_2 + value_3);
    double sigmoid = (Exp - 1)/(Exp + 1);

    if (_verbose)
      std::cout << "sigmoid: " << sigmoid << std::endl;
    
    bool flip = false;
    if (sigmoid < 0) flip = true;
    if (flip){
      if(_verbose) std::cout << "Flipping!" << std::endl;
      std::swap(opening_angle,closing_angle);
      std::swap(opening_angle_highcharge,closing_angle_highcharge);
      std::swap(opening_angle_charge_wgt,closing_angle_charge_wgt);
      std::swap(cluster.start_point,cluster.end_point);
    }
    else if(_verbose){
      std::cout << "Not Flipping!\n";
    }
    
    cluster.opening_angle = opening_angle;
    cluster.opening_angle_charge_wgt = opening_angle_charge_wgt;
    cluster.closing_angle = closing_angle;
    cluster.closing_angle_charge_wgt = closing_angle_charge_wgt;
    
    return;
  }
  
} // cluster

#endif
