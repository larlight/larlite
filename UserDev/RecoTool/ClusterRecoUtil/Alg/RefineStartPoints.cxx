#ifndef REFINESTARTPOINTS_CXX
#define REFINESTARTPOINTS_CXX

#include "RefineStartPoints.h"
#include "LArUtil/GeometryUtilities.h"
#include "LArUtil/GeometryHelper.h"

namespace cluster {


  /**
   * Calculates the following variables:
   * length
   * width
   * hit_density_1D
   * hit_density_2D
   * direction
   * @param override [description]
   */
  void RefineStartPoints::do_params_fill(cluster_params & cluster) {
 
    // need to define physical direction with opening angles and pass that to Ryan's line finder.
    
    
    // Direction decision has been moved entirely to Refine Direction()
    // opening and closing angles are already set
    // they are associated with the start and endpoints.
    // If refine direction opted to switch the shower direction, 
    // it also switched opening and closing angles.
    
    /////////////////////////////////
    // cluster.direction=  ...
    /////////////////////////////////
    
    // Direction is decided by RefineDirection()
    Hit2D startHit,endHit;
    startHit.w = cluster.start_point.w;
    startHit.t = cluster.start_point.t;
    endHit.w   = cluster.end_point.w;
    endHit.t   = cluster.end_point.t;

    


    ///////////////////////////////
    //Ryan's Shower Strip finder work here. 
    //First we need to define the strip width that we want
    double d=0.6;//this is the width of the strip.... this needs to be tuned to something.
    //===============================================================================================================       
    // Will need to feed in the set of hits that we want. 
    
    // double linearDistance=8;
    // double orthogDistance=12;
    double lineslopetest(cluster.slope_2d);
    Hit2D averageHit;
    
    auto fGSer = larutil::GeometryUtilities::GetME();
    auto geoHelper = larutil::GeometryHelper::GetME();

    //also are we sure this is actually doing what it is supposed to???
    //are we sure this works?
    //is anybody even listening?  Were they eaten by a grue?    
    std::vector <unsigned int> subhit = geoHelper->SelectLocalPointList(cluster.hit_vector,
                                                                        startHit,
                                                                        linearDistance,
                                                                        orthogDistance,
                                                                        lineslopetest,
                                                                        averageHit);

    
    if(!(subhit.size()) || subhit.size()<3) {
      if(_verbose) 
        std::cout<<"Subhit list is empty or too small. Using rough start/end points..."<<std::endl;      
      return;
    }
    
    double avgwire= averageHit.w;
    double avgtime= averageHit.t;
    //vertex in tilda-space pair(x-til,y-til)
    std::vector<std::pair<double,double>> vertil;
    // vertil.clear();// This isn't needed?
    vertil.reserve(subhit.size() * subhit.size());
    //vector of the sum of the distance of a vector to every vertex in tilda-space
    std::vector<double> vs;
    // vs.clear();// this isn't needed?
    // $$This needs to be corrected//this is the good hits that are between strip
    std::vector<unsigned int>  ghits;
    ghits.reserve(subhit.size());
    int n=0;
    double fardistcurrent=0;
    Hit2D startpoint;
    double gwiretime=0; 
    double gwire=0; 
    double gtime=0;
    double gwirewire=0;
    //KAZU!!! I NEED this too
    //this will need to come from somewhere... 
    //This is some hit that is from the way far side of the entire shower cluster...
    Point2D farhit= cluster.end_point;
    
    //=============building the local vector===================
    //this is clumsy... but just want to get something to work for now. 
    //THIS is REAL Horrible and CLUMSY... We can make things into helper functions soon. 
    std::vector<Hit2D> returnhits;
    std::vector<double> radiusofhit;
    std::vector<int> closehits;
    //unsigned int minhits=0; 
    //double maxreset=0;
    //    double avgwire=0;
    //    double avgtime=0;
    //    if(minhits<fHitVector.size()){
    //      for(auto & hit : fHitVector){
    //  std::pair<double,double> rv(fRoughEndPoint.w,fRoughEndPoint.t);
    //  closehits.clear();
    //  radiusofhit.clear();
    //  returnhits.clear();
    // for( unsigned int a=0; a<hit.size(); a++){
    //  double d= sqrt( pow((rv.first-hit.w),2) + pow((rv.second-hit.t),2)  );
    //      maxreset+=d;
    //        radiusofhit.push_back(d);}
    //      for(unsigned int b=0; b<minhits; b++){
    //  int minss= std::min_element(radiusofhit.begin(),radiusofhit.end())-radiusofhit.begin();
    //  closehits.push_back(minss);
    //  radiusofhit[minss] = maxreset;}
    //now make the vector o just the close hits using the closehit index
    //      for( unsigned int k=0; k < closehits.size(); k++){
    //  //first find the average wire and time for each set of hits... and make that the new origin before the transpose.
    //  avgwire+=fHitVector[closehits[k]].w;
    //  avgtime+=fHitVector[closehits[k]].t;
    //  returnhits.push_back(fHitVector[closehits[k]]);}
    //   }//if fHitVector is big enough 
    
    //    avgwire=avgwire/closehits.size();
    //   avgtime=avgtime/closehits.size();
    //    subhit=returnhits;
    
    //==============================================================================
    //Now we need to do the transformation into "tilda-space"
    for(unsigned int a=0; a<subhit.size();a++){
      for(unsigned int b=a+1;b<subhit.size();b++){
        if(cluster.hit_vector.at(subhit.at(a)).w != cluster.hit_vector.at(subhit.at(b)).w){
          double xtil = ((cluster.hit_vector.at(subhit.at(a)).t - avgtime) - (cluster.hit_vector.at(subhit.at(b)).t - avgtime));
          xtil /= ((cluster.hit_vector.at(subhit.at(a)).w - avgwire)-(cluster.hit_vector.at(subhit.at(b)).w - avgwire));
          double ytil = (cluster.hit_vector.at(subhit.at(a)).w - avgwire)*xtil -(cluster.hit_vector.at(subhit.at(a)).t - avgtime);
          //push back the tilda vertex point on the pair
          std::pair<double,double> tv(xtil,ytil);
          vertil.push_back(tv);
        }//if Wires are not the same
      }//for over b
    }//for over a
    // look at the distance from a tilda-vertex to all other tilda-verticies
    for(unsigned int z=0;z<vertil.size();z++){
      double vd=0;//the distance for vertex... just needs to be something 0
      for(unsigned int c=0; c<vertil.size();c++)

        vd+= sqrt(pow((vertil.at(z).first - vertil.at(c).first),2)
                + pow((vertil.at(z).second-vertil.at(c).second),2));
      vs.push_back(vd);
      vd=0.0;
    }//for z loop
    
    if(vs.size()==0)   //al hits on same wire?!
    {
      if(_verbose) std::cout<<"vertil list is empty. all subhits are on the same wire?"<<std::endl;      
      return;
    }
    //need to find the min of the distance of vertex in tilda-space
    //this will get me the area where things are most linear
    int minvs= std::min_element(vs.begin(),vs.end())-vs.begin();
    // now use the min position to find the vertex in tilda-space
    //now need to look a which hits are between the tilda lines from the points
    //in the tilda space everything in wire time is based on the new origin which is at the average wire/time
    double tilwire=vertil.at(minvs).first;//slope
    double tiltimet=-vertil.at(minvs).second+d*sqrt(1+pow(tilwire,2));//negative cept is accounted for top strip
    double tiltimeb=-vertil.at(minvs).second-d*sqrt(1+pow(tilwire,2));//negative cept is accounted for bottom strip
    // look over the subhit list and ask for which are inside of the strip
    for(unsigned int a=0; a<subhit.size(); a++){
      double dtstrip= (-tilwire * (cluster.hit_vector.at(subhit.at(a)).w - avgwire) 
                     +(cluster.hit_vector.at(subhit.at(a)).t - avgtime)-tiltimet)/sqrt(tilwire*tilwire+1);
      double dbstrip= (-tilwire * (cluster.hit_vector.at(subhit.at(a)).w - avgwire) 
                     +(cluster.hit_vector.at(subhit.at(a)).t - avgtime)-tiltimeb)/sqrt(tilwire*tilwire+1);
      
      if((dtstrip<0.0 && dbstrip>0.0)||(dbstrip<0.0 && dtstrip>0.0)){
        ghits.push_back(subhit.at(a));
      }//if between strip
    }//for a loop

    //=======================Do stuff with the good hits============================
    
    //of these small set of hits just fit a simple line. 
    //then we will need to see which of these hits is farthest away 
    
    for(unsigned int g=0; g<ghits.size();g++){
      // should call the helper funtion to do the fit
      //but for now since there is no helper function I will just write it here......again
      n+=1;
      gwiretime+= cluster.hit_vector.at(ghits.at(g)).w * cluster.hit_vector.at(ghits.at(g)).t;
      gwire+= cluster.hit_vector.at(ghits.at(g)).w;
      gtime+= cluster.hit_vector.at(ghits.at(g)).t;
      gwirewire+= cluster.hit_vector.at(ghits.at(g)).w * cluster.hit_vector.at(ghits.at(g)).w;
      // now work on calculating the distance in wire time space from the far point
      //farhit needs to be a hit that is given to me
      double fardist= sqrt(pow(cluster.hit_vector.at(ghits.at(g)).w - farhit.w,2)+pow(cluster.hit_vector.at(ghits.at(g)).t - farhit.t,2));
      //come back to this... there is a better way to do this probably in the loop
      //there should also be a check that the hit that is farthest away has subsequent hits after it on a few wires
      if(fardist>fardistcurrent){
        fardistcurrent=fardist;
        //if fardist... this is the point to use for the start point
        startpoint.t = cluster.hit_vector.at(ghits.at(g)).t;
        startpoint.w = cluster.hit_vector.at(ghits.at(g)).w;
        startpoint.plane = cluster.hit_vector.at(ghits.at(g)).plane;
        startpoint.charge = cluster.hit_vector.at(ghits.at(g)).charge;
      }
    }//for ghits loop
    //This can be the new start point
    // std::cout<<"Here Kazu"<<std::endl;
    // std::cout<<"Ryan This is the new SP ("<<startpoint.w<<" , "<<startpoint.t<<")"<<std::endl;
    // double gslope=(n* gwiretime- gwire*gtime)/(n*gwirewire -gwire*gwire);
    // double gcept= gtime/n -gslope*(gwire/n);
    
    //should this be here? Id argue this might be done once outside.
    cluster.length=fGSer->Get2DDistance((Hit2D *)&startpoint,&endHit);
    if(cluster.length)
      cluster.hit_density_1D=cluster.N_Hits/cluster.length;
    else
      cluster.hit_density_1D=0;
       
    if(cluster.length && cluster.width)
      cluster.hit_density_2D=cluster.N_Hits/cluster.length/cluster.width;
    else
      cluster.hit_density_2D=0;
    
    
    cluster.start_point=startpoint;



    return;
  }

} // cluster

#endif
