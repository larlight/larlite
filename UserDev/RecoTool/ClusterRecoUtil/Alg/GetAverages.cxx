#ifndef GETAVERAGES_CXX
#define GETAVERAGES_CXX

#include "GetAverages.h"
#include "TPrincipal.h"
#include "ClusterRecoUtil/Base/CRUException.h"
#include <map>

namespace cluster {

  void GetAverages::do_params_fill(cluster_params & cluster){

    // Use principal component analysis for a lot of this
    // So, use root's built in class
    TPrincipal fPrincipal(2,"D");

    // Set the total number of hits
    cluster.N_Hits = cluster.hit_vector.size();

    // Used to figure out duplicate wires
    std::map<double, int> wireMap;

    // Find out the mean, total charge, etc.
    cluster.sum_charge = 0.;
    int uniquewires = 0;
    int multi_hit_wires = 0;
    for(auto& hit : cluster.hit_vector){
      // std::cout << "This hit has charge " <<  hit.charge << "\n";

      // Cast the wire/time information into a double array for prin. comp. analysis
      double data[2];
      data[0] = hit.w;
      data[1] = hit.t;
      // Add this data point to the PCA
      fPrincipal.AddRow(data);
      // Add this hit to the computation of charge-weighted mean and total charge
      cluster.charge_wgt_x += hit.w * hit.charge;
      cluster.charge_wgt_y += hit.t * hit.charge;
      cluster.sum_charge += hit.charge;

      // Figure out whether this wire already has a hit
      if (wireMap[hit.w] == 0) {
        uniquewires ++;
      }
      if (wireMap[hit.w] == 1) {
        multi_hit_wires ++;
      }
      wireMap[hit.w] ++;


    }

    // Set the number of wires and multi hit wires
    cluster.N_Wires = uniquewires;
    cluster.multi_hit_wires = multi_hit_wires;

    // Finish the calculation of charge weighted means
    cluster.charge_wgt_x /= cluster.sum_charge;
    cluster.charge_wgt_y /= cluster.sum_charge;

    // Check that there are mean values before setting them
    if(fPrincipal.GetMeanValues()->GetNrows()<2) {
      throw cluster::CRUException();
      return;
    }

    // Set the mean values from the PCA, which calculates them anyways
    cluster.mean_x = (* fPrincipal.GetMeanValues())[0];
    cluster.mean_y = (* fPrincipal.GetMeanValues())[1];
    cluster.mean_charge = cluster.sum_charge / cluster.N_Hits;

    // Run the PCA analysis
    fPrincipal.MakePrincipals();

    // Save the output eigen vectors
    cluster.eigenvalue_principal = (* fPrincipal.GetEigenValues() )[0];
    cluster.eigenvalue_secondary = (* fPrincipal.GetEigenValues() )[1];

    // Save the principal direction
    cluster.principal_dir.resize(2);
    // std::cout << (* fPrincipal.GetEigenVectors())[0][0] << ", " << (* fPrincipal.GetEigenVectors())[0][1] << "\n"
              // << (* fPrincipal.GetEigenVectors())[1][0] << ", " << (* fPrincipal.GetEigenVectors())[1][1] << "\n";

    cluster.principal_dir[0] = (* fPrincipal.GetEigenVectors())[0][0];
    cluster.principal_dir[1] = (* fPrincipal.GetEigenVectors())[1][0];
    cluster.slope_2d = cluster.principal_dir[1] / cluster.principal_dir[0];

    // By request, save the angle too:
    cluster.angle_2d = atan(cluster.slope_2d);


    double rmsx = 0.0;
    double rmsy = 0.0;
    double rmsq = 0.0;
    //using the charge weighted coordinates find the axis from slope
    double ncw=0;
    double sumtime=0;//from sum averages
    double sumwire=0;//from sum averages
    double sumwiretime=0;//sum over (wire*time)
    double sumwirewire=0;//sum over (wire*wire)
    //next loop over all hits again

    for (auto& hit : cluster.hit_vector){
      // First, abuse this loop to calculate rms in x and y
      rmsx += pow(cluster.mean_x - hit.w, 2)/cluster.N_Hits;
      rmsy += pow(cluster.mean_y - hit.t, 2)/cluster.N_Hits;
      rmsq += pow(cluster.mean_charge - hit.charge, 2)/cluster.N_Hits;
      //if charge is above avg_charge
      // std::cout << "This hit has charge " <<  hit . charge << "\n";
       
      if(hit.charge > cluster.mean_charge){
        ncw+=1;
        sumwire+=hit.w;
        sumtime+=hit.t;
        sumwiretime+=hit.w * hit.t;
        sumwirewire+=pow(hit.w,2);  
      }//for high charge
    }//For hh loop

    cluster.rms_x = sqrt(rmsx);
    cluster.rms_y = sqrt(rmsy);
    cluster.RMS_charge = sqrt(rmsq);
    
    cluster.N_Hits_HC = ncw;

    //Looking for the slope and intercept of the line above avg_charge hits

    float slope_high_charge = (ncw*sumwiretime - sumwire*sumtime) / (ncw*sumwirewire - sumwire*sumwire);

    cluster.slope_2d_high_q = slope_high_charge;

    return;

  }

} // cluster

#endif
