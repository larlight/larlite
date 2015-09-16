#ifndef FINDSHOWERINGPOINT_CXX
#define FINDSHOWERINGPOINT_CXX

#include "FindShoweringPoint.h"
#include "ClusterRecoUtil/Base/CRUException.h"
#include "LArUtil/GeometryHelper.h"
#include <map>
#include "TPrincipal.h"

namespace cluster {

void FindShoweringPoint::do_params_fill(cluster_params & cluster) {

  ///\ToDo: Make sure we don't try too hard to find showering Start Points on clusters
  ///that aren't showers, or are too small.

  // Geometry Utilities
  auto geomHelper = ::larutil::GeometryHelper::GetME();

  // get hit list
  auto const hits = cluster.hit_vector;

  // Make sure the start dir cand vec is ready:
  cluster.start_dir_cand.resize(cluster.start_point_cand.size());
  cluster.shwr_point_cand.resize(cluster.start_point_cand.size());

  // Need a place to store the hitmaps:
  std::vector<std::map<float, size_t> > hitmapByStartCand;
  hitmapByStartCand.resize(cluster.start_point_cand.size());

  // Also making an array of TPrincipal's to compute the lines for the start_dir
  std::vector<TPrincipal*> tPrincipalArray;
  tPrincipalArray.resize(hitmapByStartCand.size());

  float dist_cutoff = 5 * 5;

  // First build the collections of hits nearby to the start points:
  for (size_t i_hit = 0; i_hit < hits.size(); i_hit++) {

    // loop over the whole list of start_point_candidates:
    for (size_t i_start_cand = 0; i_start_cand < cluster.start_dir_cand.size(); i_start_cand++) {
      if (! tPrincipalArray.at(i_start_cand))
        tPrincipalArray.at(i_start_cand) = new TPrincipal(2, "D");
      double distSq = ( (hits[i_hit].w - cluster.start_point_cand.at(i_start_cand).w)
                        * (hits[i_hit].w - cluster.start_point_cand.at(i_start_cand).w)
                        + (hits[i_hit].t - cluster.start_point_cand.at(i_start_cand).t)
                        * (hits[i_hit].t - cluster.start_point_cand.at(i_start_cand).t) );


      if (distSq < dist_cutoff) {
        // Could require it be forward here, but since this doesn't assume it's the right start point don't bother
        hitmapByStartCand.at(i_start_cand).insert(std::pair<double, size_t>(distSq, i_hit));
        double data[2];
        data[0] = hits[i_hit].w;
        data[1] = hits[i_hit].t;
        tPrincipalArray.at(i_start_cand)->AddRow(data);
      }

    }

  }



  // Now, loop through the potential start points and do two things:
  // Find their showering point
  // Find the start_dir
  for (size_t j = 0; j < cluster.start_point_cand.size(); j++) {
    // Dont do this if there are less than 5 hits:
    if (hitmapByStartCand.at(j).size() < 5) {
      cluster.start_dir[0] = 0.0;
      cluster.start_dir[1] = 0.0;
      continue;
    }
    else {
      // Make the principal component:
      tPrincipalArray.at(j) -> MakePrincipals();
      cluster.start_dir_cand.at(j).w = (* tPrincipalArray.at(j)->GetEigenVectors())[0][0];
      cluster.start_dir_cand.at(j).t = (* tPrincipalArray.at(j)->GetEigenVectors())[1][0];
      cluster.shwr_point_cand.at(j) = cluster.start_point_cand.at(j);
    }

    // loop through hits in distance-order to start-point
    // keep track of the cosine of the angle between
    // the line connecting points (n -> n+1) and (n+1 -> n+2)
    // if this cosine falls below a certain value -> stop and
    // mark the last point explored as the showering start point
    size_t nPoints = 0;
    // Find the showering point for this group.
    // To check forwardness, need to get a second point that is down the axis
    // Use the mean of the selected hits to draw a line from start point to mean.
    Point2D mean(cluster.plane_id.Plane,
                 tPrincipalArray.at(j)->GetMeanValues()[0][0],
                 tPrincipalArray.at(j)->GetMeanValues()[0][1]);
    // std::cout << "mean is " << mean.w << ", " << mean.t << std::endl;

    // Remove all the points from consideration if the point is behind the start point:
    std::vector<int> goodValues;
    goodValues.reserve(hitmapByStartCand.at(j).size());
    for (auto it = hitmapByStartCand.at(j).begin();
         it != std::prev(std::prev(hitmapByStartCand.at(j).end()));
         it++)
    {
      // Determine if this point is behind the start point:
      float forward = (hits[it->second].w - cluster.start_point_cand.at(j).w) *
                      (mean.w - cluster.start_point_cand.at(j).w);
      forward += (hits[it->second].t - cluster.start_point_cand.at(j).t) *
                 (mean.t - cluster.start_point_cand.at(j).t);
      if (forward > 0) {
        goodValues.push_back(it->second);
      }
    }
    if (goodValues.size() < 2) continue;
    // std::cout << "goodValues is ";
    // for (auto & val : goodValues)
    //   std::cout << val << " ";
    // std::cout << std::endl;
    for (size_t i = 0; i < goodValues.size() - 2; i++){
      int curr = goodValues.at(i);
      int next = goodValues.at(i + 1);
      int nextnext = goodValues.at(i+2);
      if (_verbose ) {
        std::cout << "points: " << curr << ", " << next << ", " << nextnext;
        std::cout << " (" << hits[curr].w << ", " << hits[curr].t << "),"
                  << " (" << hits[next].w << ", " << hits[next].t << "),"
                  << " (" << hits[nextnext].w << ", " << hits[nextnext].t << ")"
                  << std::endl;
      }

      // std::cout << "Checking forwardness of hit at "
      //           << hits[curr].w << ", " << hits[curr].t << std::endl;




      // std::cout << "forward is " << forward << std::endl;

      // here, check for forwardness against the direction of this start candidate
      // if ( forward > 0 )
      // {
        // std::cout << "Forward" << std::endl;
        // std::cout << curr << ", " << next << ", " << nextnext << std::endl;
        double cos = geomHelper->GetCosAngleBetweenLines(hits[curr], hits[next],
                     hits[next], hits[nextnext]);
        if (cos > 0.7)
          cluster.shwr_point_cand.at(j)  = hits[nextnext];
        else
          break;
        nPoints += 1;
      // }


    }

    // Make sure to clear out the TPrincipal objects:
    if (tPrincipalArray.at(j))
      delete tPrincipalArray.at(j);

  }

  // Set the default values just in case.
  // Unless there is exactly 1 candidate, this might be junk.
  cluster.start_dir.resize(2);
  if (cluster.shwr_point_cand.size() != 0){
    cluster.showering_point = cluster.shwr_point_cand.front();
    cluster.start_dir[0] = cluster.start_dir_cand.front().w;
    cluster.start_dir[1] = cluster.start_dir_cand.front().t;
  }



  // // get ordered hit list (from closest to start point to furthest)
  // // save only those that are < 10 cm away
  // // Map[dist] -> position of hit in hit vector
  // std::map<double, size_t> hitmap;
  // for (size_t i = 0; i < hits.size(); i++) {
  //   // distance to start point:
  //   double distSq = ( (hits[i].w - cluster.start_point.w) * (hits[i].w - cluster.start_point.w) +
  //                     (hits[i].t - cluster.start_point.t) * (hits[i].t - cluster.start_point.t) );

  //   if ( distSq < 25 ) {
  //     // check that the point is towards the shower direction (and not backwards)
  //     // this assumes we have got the right direction
  //     // the full cosine is not necessary. All we need is the sign.
  //     // so we just use the numerator from "GetCosAngleBetweenLines" function in GeometryHelper
  //     // for 3 points
  //     if ( ( (hits[i].w - start.w) * (end.w - start.w)
  //            + (hits[i].t - start.t) * (end.t - start.t) ) > 0 )
  //       hitmap.insert(std::pair<double, size_t>(distSq, i));
  //   }
  // }


  // //\\\TODO: Make this slope determination done by a fit and not just connecting points.

  // //typedef std::map<double,size_t>::iterator *it;
  // // for (auto it = hitmap.begin(); it != hitmap.end(); it++){
  // //   std::cout << "hit " << it->second << " charge: : " << it->first << std::endl;
  // //   //std::cout << "this hit's charge: : " << next->first << std::endl;
  // // }
  // if (_verbose) std::cout << "\n\nLooping over " << hitmap.size() << " hits ..." << std::endl;
  // for (auto it = hitmap.begin(); it != std::prev(std::prev(hitmap.end())); it++) {
  //   auto next = std::next(it);
  //   if (_verbose) {
  //     std::cout << "points: " << it->second << ", " << next->second << ", " << std::next(next)->second;
  //     std::cout << " (" << hits[it->second].w << ", " << hits[it->second].t << ")" << std::endl;
  //   }
  //   double cos = geomHelper->GetCosAngleBetweenLines(hits[(it)->second], hits[next->second],
  //                hits[next->second], hits[std::next(next)->second]);
  //   if (cos > 0.7)
  //     showering_start = hits[std::next(next)->second];
  //   else
  //     break;
  //   nPoints += 1;
  // }

  // cluster.showering_point = showering_start;

  // // Now that there is a showering point, use the start point and the showering point
  // //  and the hits in between to determine the slope of the start of the shower
  // //
  // // If there are a few hits, use a linear regression to make a best fit line
  // // Otherwise, use the difference between the two points
  // //
  // if (hitmap.size() < 5)
  // {
  //   cluster.start_dir.resize(2);
  //   float mag = sqrt(pow((showering_start.w - cluster.start_point.w), 2) + pow((showering_start.t - cluster.start_point.t), 2));
  //   cluster.start_dir[0] = (showering_start.w - cluster.start_point.w) / mag;
  //   cluster.start_dir[1] = (showering_start.t - cluster.start_point.t) / mag;
  // }
  // else {

  //   TPrincipal fPrincipal(2, "D");
  //   for (auto& hit : hitmap) {
  //     // std::cout << "This hit has charge " <<  hit.charge << "\n";

  //     // Cast the wire/time information into a double array for prin. comp. analysis
  //     double data[2];
  //     data[0] = hits[hit.second].w;
  //     data[1] = hits[hit.second].t;
  //     // Add this data point to the PCA
  //     fPrincipal.AddRow(data);
  //   }

  //   // Run the PCA analysis
  //   fPrincipal.MakePrincipals();

  //   // Save the principal direction
  //   cluster.start_dir.resize(2);
  //   // std::cout << (* fPrincipal.GetEigenVectors())[0][0] << ", " << (* fPrincipal.GetEigenVectors())[0][1] << "\n"
  //   // << (* fPrincipal.GetEigenVectors())[1][0] << ", " << (* fPrincipal.GetEigenVectors())[1][1] << "\n";

  //   cluster.start_dir[0] = (* fPrincipal.GetEigenVectors())[0][0];
  //   cluster.start_dir[1] = (* fPrincipal.GetEigenVectors())[1][0];

  //   // float x_mean(0), y_mean(0), xy_mean(0), xx_mean(0);
  //   // for (auto it = hitmap.begin(); it != std::prev(std::prev(hitmap.end())); it++) {
  //   //   x_mean  += hits[it->second].w;
  //   //   y_mean  += hits[it->second].t;
  //   //   xy_mean += hits[it->second].w * hits[it->second].t;
  //   //   xx_mean += hits[it->second].w * hits[it->second].w;
  //   // }
  //   // int count = hitmap.size();
  //   // float slope = (count*xy_mean - x_mean * y_mean) / (count*xx_mean - x_mean * x_mean);
  //   // float theta = atan(slope);

  //   // cluster.start_dir[0] = cos(theta);
  //   // cluster.start_dir[1] = sin(theta);

  // }



  return;
}

} // cluster

#endif
