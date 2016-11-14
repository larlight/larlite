#ifndef RECOTOOL_GEOCONIC_CXX
#define RECOTOOL_GEOCONIC_CXX

#include "geoconic.h"

namespace larlite {
  
  //-----------------------------------------------------------------------------------------------------------
  // Is the 3d point in the tpc?
  bool geoconic::TPCContained(const TVector3& pos  ){
    auto geo = larutil::Geometry::GetME();
    auto x = geo->DetHalfWidth();
    auto y = geo->DetHalfHeight();
    auto z = geo->DetLength();
    double fid = 14; // This needs to be here because we are not doing the position box properly
    double xlo = 0+fid;
    double xhi = 2*x-fid;
    double ylo = -y+fid;
    double yhi = y-fid;
    double zlo = 0+fid;
    double zhi = z-fid;
    if(pos.X()>xhi || pos.X()<xlo || pos.Y()>yhi || pos.Y()<ylo || pos.Z()<zlo || pos.Z()>zhi){
      return  false;}
    return true;
  }//geoconic::TPCContained
  
  //-----------------------------------------------------------------------------------------------------------
  // Are the 3d Cone Edges Contained in the tpc?
  bool geoconic::ConeInTPC(const TVector3& Pos, const TVector3& dir, double Length, double OpeningAngle, int smoothness){
    // Calculate the radius at a given length and opening angle
    double Radius = Length*tan(OpeningAngle/2 *PI/180);
    auto geo = larutil::Geometry::GetME();
    // Get the proper bounds for tpc
    auto x = geo->DetHalfWidth();
    auto y = geo->DetHalfHeight();
    auto z = geo->DetLength();
    //double fid = 1; // This needs to be here because we are not doing the position box properly
    double fid = 14; // This needs to be here because we are not doing the position box properly
    double xlo = 0+fid;
    double xhi = 2*x-fid;
    double ylo = -y+fid;
    double yhi = y-fid;
    double zlo = 0+fid;
    double zhi = z-fid;
    // Make the points
    TVector3 Axis;
    double axisnorm = sqrt(dir.Px()*dir.Px()+dir.Py()*dir.Py()+dir.Pz()*dir.Pz());
    Axis.SetX(dir.Px()/axisnorm);
    Axis.SetY(dir.Py()/axisnorm);
    Axis.SetZ(dir.Pz()/axisnorm);
    // Get orthogonal vector
    TVector3 avec;
    avec = Axis.Orthogonal();
    // cross these vects to get the other mut-orthoganal one
    TVector3 bvec;
    bvec = Axis.Cross(avec);
    // If vertex point is not in tpc return false
    if(Pos.X()>xhi || Pos.X()<xlo || Pos.Y()>yhi || Pos.Y()<ylo || Pos.Z()<zlo || Pos.Z()>zhi){
      return false;}
    for(double theta =0 ; theta<2*PI; theta+=PI/smoothness){
      double paramX = Pos.X() + Length*Axis.X()/Axis.Mag() + Radius*cos(theta)*avec.X() + Radius*sin(theta)*bvec.X();
      double paramY = Pos.Y() + Length*Axis.Y()/Axis.Mag() + Radius*cos(theta)*avec.Y() + Radius*sin(theta)*bvec.Y();
      double paramZ = Pos.Z() + Length*Axis.Z()/Axis.Mag() + Radius*cos(theta)*avec.Z() + Radius*sin(theta)*bvec.Z();
      // if any of the cone edges are not in tpc return false
      if(paramX>xhi || paramX<xlo || paramY>yhi || paramY<ylo || paramZ<zlo || paramZ>zhi) return false;
    }// theta loop
    return true;
  }// ConeInTPC
  
  //-----------------------------------------------------------------------------------------------------------
  // This returns Polygon edges for the cone If you already have the cone points
  std::vector<larutil::PxPoint> geoconic::ConicalEdge(std::vector<larutil::PxPoint> incone){
    std::vector<larutil::PxPoint> edges;
    int n = incone.size();
    int p = 0;
    int l ;
    for (int i =1 ; i < n; i++){
      if(incone[p].w > incone[i].w){p = i;}
      else if(incone[p].w == incone[i].w && incone[p].t > incone[i].t){p = i;}
    }// for loop over all the points
    l=p;
    int q = (p+1)%n;
    int count=0;
    std::vector<larutil::PxPoint> result(n+1);
    do
    {
      result[count]=incone[p];
      count++;
      for( int i=0; i<n; i++){
        if(orientation(incone[p],incone[i],incone[q])==2){q=i;}
						}// for over i
      p=q;
      q = (p+1)%n;
    }while(p!=l);
    for(unsigned int b = 0 ; b<result.size(); b++){
      if( result[b].w==0 && result[b].t==0) break;
      edges.push_back(result[b]);
      std::cout<<" The output values "<<result[b].w << " , " <<result[b].t<<std::endl;}
    
    edges = result;
    return edges;
  }
  
  //-----------------------------------------------------------------------------------------------------------
  // This returns Polygon edges for the cone
  // List of Conical Features that make the polygon
  // need to give the input
  // 1 Position of vertex
  // 2 Direction of vertex
  // 3 Length of cone
  // 4 Opening Angle
		// 5 Plane
		// 6 smoothness
  std::vector<larutil::PxPoint> geoconic::ConicalFeatures(const TVector3& Pos, const TVector3& dir, double Length, double OpeningAngle, int plane, int smoothness){
    std::vector<larutil::PxPoint> ret;
    // Calculate the radius at a given length and opening angle
    double Radius = Length*tan(OpeningAngle/2 *PI/180);
    auto geom = larutil::GeometryHelper::GetME();
    // Fill up the vertex 2d point FullList
    std::vector<larutil::PxPoint> pConeFullHits;
    // Fill up the Final 2d point Multiple points List
    std::vector<larutil::PxPoint> CFHA;
    // Fill up the Final 2d point Collinear points List
    std::vector<larutil::PxPoint> CFH;
    
    TVector3 pos = Pos;
    TVector3 Axis;
    double axisnorm = sqrt(dir.Px()*dir.Px()+dir.Py()*dir.Py()+dir.Pz()*dir.Pz());
    Axis.SetX(dir.Px()/axisnorm);
    Axis.SetY(dir.Py()/axisnorm);
    Axis.SetZ(dir.Pz()/axisnorm);
    // Get orthogonal vector
    TVector3 avec;
    avec = Axis.Orthogonal();
    // cross these vects to get the other mut-orthoganal one
    TVector3 bvec;
    bvec = Axis.Cross(avec);
    
    // Fill the the vertex point
    auto vert = geom->Point_3Dto2D(pos,plane);
    pConeFullHits.push_back(vert);
    
    // Get some points that are on the edge of the cone
    for(double theta =0 ; theta<2*PI; theta+=PI/smoothness){
      double paramx = Pos.X() + Length*Axis.X()/Axis.Mag() + Radius*cos(theta)*avec.X() + Radius*sin(theta)*bvec.X();
      double paramy = Pos.Y() + Length*Axis.Y()/Axis.Mag() + Radius*cos(theta)*avec.Y() + Radius*sin(theta)*bvec.Y();
      double paramz = Pos.Z() + Length*Axis.Z()/Axis.Mag() + Radius*cos(theta)*avec.Z() + Radius*sin(theta)*bvec.Z();
      TVector3 FillPoint;
      FillPoint.SetX(paramx);
      FillPoint.SetY(paramy);
      FillPoint.SetZ(paramz);
      auto pt =geom->Point_3Dto2D(FillPoint,plane);
      // might need a bail here
      pConeFullHits.push_back(pt);
    }
    
    // Check if there are double points
    std::vector<unsigned int > exclude;
    for(unsigned int a=0; a< pConeFullHits.size();a++){
      for(unsigned int b=a+1; b< pConeFullHits.size();b++){
        if(pConeFullHits[a].w==pConeFullHits[b].w && pConeFullHits[a].t==pConeFullHits[b].t){
          // need to get rid of point b
          exclude.push_back(b);
        }
      }
    }
    // Fill the points that are not double
    for(unsigned int a=0; a<pConeFullHits.size() ;a++){
      bool goodpt = true;
      for(unsigned int b=0; b< exclude.size(); b++){
        if(a==exclude[b]) goodpt = false;
      }
      if(goodpt){CFHA.push_back(pConeFullHits[a]);}
    }
    
    // Check if there are collinear points... If so deal with them
    std::vector<std::vector<unsigned int>> cofix;
    for(unsigned int a=0; a<CFHA.size(); a++){
      for(unsigned int b=a+1; b<CFHA.size(); b++){
        for(unsigned int c=b+1; c<CFHA.size(); c++){
          double collinear = (CFHA[b].t-CFHA[a].t)*(CFHA[c].w-CFHA[b].w) - (CFHA[c].t-CFHA[b].t)*(CFHA[b].w - CFHA[a].w);
          if(collinear==0){
            std::vector<unsigned int> copt(3);
            copt[0] = a;
            copt[1] = b;
            copt[2] = c;
            cofix.push_back(copt);
            a = c;
            goto moveon;
          }// if collinear
        }
      }
    moveon:;
    }
    
    // Fill all the hits that are not collinear
    for(unsigned int a=0; a<CFHA.size(); a++){
      bool goodpt = true;
      for(unsigned int co=0; co<cofix.size();co++){
        if(a==cofix[co][0] ||a==cofix[co][1] ||a==cofix[co][2]){ goodpt = false; break;} // don't fill this point
      }
      if(goodpt){CFH.push_back(CFHA[a]);}
    }
    // Need to average the collinear points and put them in too
    for(unsigned int co=0; co<cofix.size();co++){
      // just pick the middle one for now
      CFH.push_back(CFHA[cofix[co][1]]);
    }
    
    /////////
    //&&&&&&&
    /////////
    // Now do the walk.
    int n = CFH.size();
    int p = 0;
    int l ;
    for (int i =1 ; i < n; i++){
      if(CFH[p].w > CFH[i].w){p = i;}
      else if(CFH[p].w == CFH[i].w && CFH[p].t > CFH[i].t){p = i;}
    }//
    l=p;
    int q = (p+1)%n;
    int count=0;
    std::vector<larutil::PxPoint> result(n+1);
    do
    {
      result[count]=CFH[p];
      count++;
      for( int i=0; i<n; i++){if(orientation(CFH[p],CFH[i],CFH[q])==2) q=i;}
      p=q;
      q = (p+1)%n;
      
      /////////////////////////
      /// This  might be able to be removed
      /////////////////////////
      if(count> n+1){
        ret.push_back(CFH[0]);
        std::cout<< " DOES THIS EVER HAPPEN? "<<ret.size()<<std::endl;
        //// looking at stuff before the bail
        for(unsigned int a=0; a<CFH.size(); a++){
          std::cout<<"Circle edge w , t 2D "<< CFH[a].w<<" , "<<CFH[a].t<<std::endl;
        }//
        return ret;}
      /////////////////////////
      /////////////////////////
    }while(p!=l);
    
    for(unsigned int b = 0 ; b<result.size(); b++){
      if( result[b].w==0 && result[b].t==0) break;
      ret.push_back(result[b]);
    }
    return ret;
		}
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  
  bool geoconic::ConicalOverlap(const std::vector<larutil::PxPoint> conea,const std::vector<larutil::PxPoint> coneb){
    // look to see if the cones intersect at all
    // if return true then that means cone intesect
    // if returns false then the cones do not intersect
    for(unsigned int a=0 ; a<conea.size()-1;a++){
      auto p1 = conea[a];
      auto q1 = conea[a+1];
      for(unsigned int b=0 ; b<coneb.size()-1;b++){
        auto p2 = coneb[b];
        auto q2 = coneb[b+1];
        // Find the four orientations needed for general and
        // special cases
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);
        
        // General case
        if (o1 != o2 && o3 != o4)
          return true;
        // Special Cases
        // p1, q1 and p2 are colinear and p2 lies on segment p1q1
        if (o1 == 0 && onSegment(p1, p2, q1)) return true;
        
        // p1, q1 and p2 are colinear and q2 lies on segment p1q1
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;
        
        // p2, q2 and p1 are colinear and p1 lies on segment p2q2
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;
        
        // p2, q2 and q1 are colinear and q1 lies on segment p2q2
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;
      }// loop over coneb
    }//loop over conea
    return false; // Doesn't fall in any of the above cases
  }
		
  
  
  
  //-----------------------------------------------------------------------------------------------------------
  // This returns a vector of hits that are contained in the polygon
  std::vector<larutil::PxHit> geoconic::PolyContain(std::vector<larutil::PxHit> hits, std::vector<larutil::PxPoint> polygon ){
    double InfAdd = 10000000;
    std::vector<larutil::PxHit> rethits;
    // Loop over all hits
    for( auto hit: hits){
      // Need to make an extreme point for each hit
      larutil::PxPoint extreme;
      extreme.w = hit.w+InfAdd;// are these in cm
      extreme.t = hit.t;// are these in cm
      larutil::PxPoint currenthit;
      currenthit.t = hit.t ; // are these in cm
      currenthit.w = hit.w;// are these in cm
      // Count intersections of the above line with sides of polygon
      int count = 0, i = 0;
      int n = polygon.size();
      do
      {
        int next = (i+1)%n;
        if (doIntersect(polygon[i], polygon[next], currenthit, extreme))
        {
          count++;
          //Fill the hit
          //rethits.push_back(hit);
        }
        i = next;
      } while (i != 0);
      if(count%2!=0) rethits.push_back(hit);
    }// for loop over all hits to check
    return rethits;
  }// end
  
  //-----------------------------------------------------------------------------------------------------------
  // This returns a vector of hits that are contained in the polygon
  //std::vector<larlite::hit> geoconic::PolyContainHit(std::vector<larlite::hit> hits, std::vector<larutil::PxPoint> polygon ){
  std::vector<unsigned int> geoconic::PolyContainHit(std::vector<larlite::hit> hits, std::vector<larutil::PxPoint> polygon , unsigned int plane){
    double InfAdd = 10000000;
    auto geom = larutil::GeometryHelper::GetME();
    auto tservice = larutil::TimeService::GetME();
    auto const& tpc_clock = tservice->TPCClock();
    double tick_offset = tservice->TriggerOffsetTPC() * tpc_clock.Frequency();// Kazu's fix
    
    //	std::vector<larlite::hit> rethits;
    std::vector<unsigned int > rethits;
    // Loop over all hits
    for(unsigned int a = 0; a<hits.size(); a++){
      // Need to make an extreme point for each hit
      if(hits[a].View()==plane){
        larutil::PxPoint extreme;
        extreme.w=  (hits[a].WireID().Wire *geom->WireToCm() )+InfAdd;// are these in cm
        extreme.t=  (hits[a].PeakTime() + tick_offset )* geom->TimeToCm();
        larutil::PxPoint currenthit;
        currenthit.t = (hits[a].PeakTime() + tick_offset )* geom->TimeToCm()  ; // are these in cm
        currenthit.w = hits[a].WireID().Wire*geom->WireToCm() ;// are these in cm
        // Count intersections of the above line with sides of polygon
        int count = 0, i = 0;
        int n = polygon.size();
        do
        {
          int next = (i+1)%n;
          if (doIntersect(polygon[i], polygon[next], currenthit, extreme))
          {
            count++;
            //Fill the hit
            //rethits.push_back(hit);
            rethits.push_back(a);
          }
          i = next;
        } while (i != 0);
        
      }// if the proper plane
    }// for loop over all hits to check
    return rethits;
  }// end
  
  
  
  
  
  
  
  ///////////////====Needed for the cone conatain =========================================
  int geoconic::orientation(larutil::PxPoint p,larutil::PxPoint q, larutil::PxPoint r){
    double val = (q.t - p.t) * (r.w - q.w) - (q.w - p.w) * (r.t - q.t);
    if (val == 0) return 0;  // colinear
    if (val > 0) return 1 ; // clock or counterclock wise
    else return 2;
  }
  //----------------------------------------------------------------------------------------
  bool geoconic::onSegment(larutil::PxPoint p, larutil::PxPoint q, larutil::PxPoint r){
    if (q.w <= std::max(p.w, r.w) && q.w >= std::min(p.w, r.w) &&
        q.t <= std::max(p.t, r.t) && q.t >= std::min(p.t, r.t))
      return true;
    return false;
  }
  //----------------------------------------------------------------------------------------
  bool geoconic::doIntersect(larutil::PxPoint p1, larutil::PxPoint q1, larutil::PxPoint p2, larutil::PxPoint q2){
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    // General case
    if (o1 != o2 && o3 != o4)
      return true;
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    
    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
    
    return false; // Doesn't fall in any of the above cases
  }
  ///////////////===== End for the Contain ===========================================
  
  
  bool geoconic::walkable(const TVector3& Pos, const TVector3& dir, double Length, double OpeningAngle, int plane, int smoothness){
    // Calculate the radius at a given length and opening angle
    double Radius = Length*tan(OpeningAngle/2 *PI/180);
    auto geom = larutil::GeometryHelper::GetME();
    // Fill up the vertex 2d point FullList
    std::vector<larutil::PxPoint> pConeFullHits;
    // Fill up the Final 2d point Multiple points List
    std::vector<larutil::PxPoint> CFHA;
    // Fill up the Final 2d point Collinear points List
    std::vector<larutil::PxPoint> CFH;
    
    TVector3 pos = Pos;
    TVector3 Axis;
    double axisnorm = sqrt(dir.Px()*dir.Px()+dir.Py()*dir.Py()+dir.Pz()*dir.Pz());
    Axis.SetX(dir.Px()/axisnorm);
    Axis.SetY(dir.Py()/axisnorm);
    Axis.SetZ(dir.Pz()/axisnorm);
    // Get orthogonal vector
    TVector3 avec;
    avec = Axis.Orthogonal();
    // cross these vects to get the other mut-orthoganal one
    TVector3 bvec;
    bvec = Axis.Cross(avec);
    
    // Fill the the vertex point
    auto vert = geom->Point_3Dto2D(pos,plane);
    pConeFullHits.push_back(vert);
    
    // Get some points that are on the edge of the cone
    for(double theta =0 ; theta<2*PI; theta+=PI/smoothness){
      double paramx = Pos.X() + Length*Axis.X()/Axis.Mag() + Radius*cos(theta)*avec.X() + Radius*sin(theta)*bvec.X();
      double paramy = Pos.Y() + Length*Axis.Y()/Axis.Mag() + Radius*cos(theta)*avec.Y() + Radius*sin(theta)*bvec.Y();
      double paramz = Pos.Z() + Length*Axis.Z()/Axis.Mag() + Radius*cos(theta)*avec.Z() + Radius*sin(theta)*bvec.Z();
      TVector3 FillPoint;
      FillPoint.SetX(paramx);
      FillPoint.SetY(paramy);
      FillPoint.SetZ(paramz);
      bool cont = TPCContained(FillPoint);
      if(!cont) return false;
      auto pt =geom->Point_3Dto2D(FillPoint,plane);
      // might need a bail here
      pConeFullHits.push_back(pt);
    }
    
    if(pConeFullHits.size()<3) return false;
    
    // Check if there are double points
    std::vector<unsigned int > exclude;
    for(unsigned int a=0; a< pConeFullHits.size();a++){
      for(unsigned int b=a+1; b< pConeFullHits.size();b++){
        if(pConeFullHits[a].w==pConeFullHits[b].w && pConeFullHits[a].t==pConeFullHits[b].t){
          // need to get rid of point b
          exclude.push_back(b);
        }
      }
    }
    // Fill the points that are not double
    for(unsigned int a=0; a<pConeFullHits.size() ;a++){
      bool goodpt = true;
      for(unsigned int b=0; b< exclude.size(); b++){if(a==exclude[b]) goodpt = false;}
      if(goodpt){CFHA.push_back(pConeFullHits[a]);}
    }
    
    if(CFHA.size()<3) return false;
    // Check if there are collinear points... If so deal with them
    std::vector<std::vector<unsigned int>> cofix;
    for(unsigned int a=0; a<CFHA.size(); a++){
      for(unsigned int b=a+1; b<CFHA.size(); b++){
        for(unsigned int c=b+1; c<CFHA.size(); c++){
          double collinear = (CFHA[b].t-CFHA[a].t)*(CFHA[c].w-CFHA[b].w) - (CFHA[c].t-CFHA[b].t)*(CFHA[b].w - CFHA[a].w);
          if(collinear==0){
            std::vector<unsigned int> copt(3);
            copt[0] = a;
            copt[1] = b;
            copt[2] = c;
            cofix.push_back(copt);
            a = c;
            goto moveon;
          }// if collinear
        }
      }
    moveon:;
    }
    
    // Fill all the hits that are not collinear
    for(unsigned int a=0; a<CFHA.size(); a++){
      bool goodpt = true;
      for(unsigned int co=0; co<cofix.size();co++){
        if(a==cofix[co][0] ||a==cofix[co][1] ||a==cofix[co][2]){ goodpt = false; break;} // don't fill this point
      }
      if(goodpt){CFH.push_back(CFHA[a]);}
    }
    // Need to average the collinear points and put them in too 
    for(unsigned int co=0; co<cofix.size();co++){
      // just pick the middle one for now
      CFH.push_back(CFHA[cofix[co][1]]);
    }
    if(CFH.size()<3) return false;
    return true;
  }
  
  
  
}

#endif

