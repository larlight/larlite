#ifndef GETAVERAGES_CXX
#define GETAVERAGES_CXX

#include "GetAverages.h"
#include "TPrincipal.h"
#include "CRUException.h"
#include <map>

namespace cluster {

  void GetAverages::do_params_fill(cluster_params & cluster, bool verbose){

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

  }

} // cluster

#endif
