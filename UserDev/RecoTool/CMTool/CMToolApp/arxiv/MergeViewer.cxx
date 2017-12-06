#ifndef RECOTOOL_MERGEVIEWER_CXX
#define RECOTOOL_MERGEVIEWER_CXX

#include "MergeViewer.h"
#include "LArUtil/GeometryHelper.h"

namespace larlite {
  
  //################################################################
  MergeViewer::MergeViewer() : ::larlite::ClusterMerger(),
                           _algo("MergeViewer")
  //################################################################
  {
    // Class name
    _name = "MergeViewer";
    
    // Default values for stuff
    SetDrawPolygon(false);
    SetPrintClusterInfo(false);
    SetMinHitsToDraw(0);
    SetDrawTracks(true);
    SetDrawOnlyTracks(false);
    SetDrawStartEnd(false);
  }
  
  //################################################################
  bool MergeViewer::initialize()
  //################################################################
  {
    _algo.Reset();
    return ClusterMerger::initialize();
  }
  
  //################################################################
  bool MergeViewer::analyze(storage_manager* storage)
  //################################################################
  {
    _algo.Reset();
    
    const ::larutil::Geometry* geo = ::larutil::Geometry::GetME();
    const ::larutil::GeometryHelper* geo_help = ::larutil::GeometryHelper::GetME();
    
    UChar_t nplanes = geo->Nplanes();
    double  wire2cm = geo_help->WireToCm();
    double  time2cm = geo_help->TimeToCm();
    
    //
    // Obtain event-wise data object pointers
    //
    auto ev_clus = storage->get_data<event_cluster>(_input_producer);
    if(!ev_clus)
      throw ::cluster::ViewerException(Form("Did not find cluster data product by %s!",
                                     _input_producer.c_str()
                                     )
                                 );

    if(!ev_clus->size()) {
      print(msg::kWARNING,__FUNCTION__,
            Form("Skipping event %d since no cluster found...",ev_clus->event_id()));
      return false;
    }

    event_hit* ev_hit = nullptr;
    auto const& ass_hit_v = storage->find_one_ass(ev_clus->id(),ev_hit,ev_clus->name());
    if (!ev_hit)
      throw ::cluster::ViewerException("Did not find associated hits!");
    if (!ass_hit_v.size())
      throw ::cluster::ViewerException("Did not find associated hits!");
    /*
    auto associated_hit_producers = ev_clus->association_keys(data::kHit);
    
    if(!(associated_hit_producers.size()))

      throw ::cluster::ViewerException("Did not find associated hits!");

    auto ev_hit  = storage->get_data<event_hit>(associated_hit_producers[0]);

    if(!ev_hit)
      throw ::cluster::ViewerException(Form("Did not find hit data product by %s!",
                                     associated_hit_producers[0].c_str()
                                     )
                                 );
    */

    // Find hit range & fill all-hits vector
    std::vector<std::pair<double,double> > xrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::pair<double,double> > yrange(nplanes,std::pair<double,double>(1e9,0));
    std::vector<std::vector<std::pair<double,double> > > hits_xy(nplanes,std::vector<std::pair<double,double> >());
    std::vector<std::vector<double> > hits_charge(nplanes,std::vector<double>());
    for(auto const &h : *ev_hit) {
      
      UChar_t plane = geo->ChannelToPlane(h.Channel());
      double x = h.WireID().Wire * wire2cm;
      double y = h.PeakTime() * time2cm;
      
      if(xrange.at(plane).first > x ) xrange.at(plane).first = x;
      if(xrange.at(plane).second < x ) xrange.at(plane).second = x;
      
      if(yrange.at(plane).first > y ) yrange.at(plane).first = y;
      if(yrange.at(plane).second < y ) yrange.at(plane).second = y;
      
      hits_xy.at(plane).push_back(std::pair<double,double>(x,y));
      hits_charge.at(plane).push_back(h.Integral());
    }
    
    // Inform the algorithm about the range
    for(size_t i=0; i<nplanes; ++i)
      _algo.SetRange(i,xrange.at(i),yrange.at(i));
    
    // Provide plane-hits to the algorithm
    for(size_t i=0; i<nplanes; ++i)
      _algo.AddHits(i,hits_xy.at(i),hits_charge.at(i));
    
    
    // Now run ClusterMerger on this event
    ClusterMerger::analyze(storage);
    
    // Find hits-per-cluster
    for(auto const &cl : ClusterMerger::GetManager(this->GetNumManager()-1).GetClusters()) {
      
      UChar_t plane = cl.plane_id.Plane;
      
      std::vector<std::pair<double,double> > cluster_hits;
      std::pair<double,double> cluster_start  ( cl.start_point.w, cl.start_point.t ); 
      std::pair<double,double> cluster_end    ( cl.end_point.w,   cl.end_point.t   ); 
      
      for(auto const& h : cl.hit_vector)
        
        cluster_hits.push_back(std::pair<double,double>( h.w, h.t));
      
      std::vector<std::pair<double,double> > cluster_polygon(cl.PolyObject.Size() + 1,
                                                std::pair<double,double>(0,0)
                                                );
      
      for(size_t i=0; i<cl.PolyObject.Size(); ++i) {
        cluster_polygon.at(i).first = cl.PolyObject.Point(i).first;
        cluster_polygon.at(i).second = cl.PolyObject.Point(i).second;
      }
      
      if(cluster_polygon.size()){
        cluster_polygon.at(cluster_polygon.size()-1).first = cluster_polygon.at(0).first;
        cluster_polygon.at(cluster_polygon.size()-1).second = cluster_polygon.at(0).second;
      }
    

      //only draw clusters with more than _min_hits_to_draw
      if(cluster_hits.size() > _min_hits_to_draw){

        bool is_track = cl.eigenvalue_principal > 0.99000;
        
        //if it's not a track, draw it only if !_draw_only_tracks
        if(!is_track){
          if(!_draw_only_tracks){
            if( _draw_polygon and _showStartEnd )
              _algo.AddCluster(plane,
                           cluster_hits,
                           cluster_start,
                           cluster_end,
                           cluster_polygon);
            else if ( _draw_polygon and !_showStartEnd )
              _algo.AddCluster(plane,
                           cluster_hits,
                           cluster_polygon);
            else if ( !_draw_polygon and _showStartEnd )
              _algo.AddCluster(plane,
                           cluster_hits,
                           cluster_start,
                           cluster_end);
            else if ( !_draw_polygon and !_showStartEnd )
              _algo.AddCluster(plane,
                           cluster_hits);
          }//end if draw only tracks
        }//end if it is not a track
        
        else{//if it is a track, draw it if _draw_tracks is true
          if(_draw_tracks){
            if( _draw_polygon and _showStartEnd )
              _algo.AddCluster(plane,
                           cluster_hits,
                           cluster_start,
                           cluster_end,
                           cluster_polygon);
            else if ( _draw_polygon and !_showStartEnd )
              _algo.AddCluster(plane,
                           cluster_hits,
                           cluster_polygon);
            else if ( !_draw_polygon and _showStartEnd )
              _algo.AddCluster(plane,
                           cluster_hits,
                           cluster_start,
                           cluster_end);
            else if ( !_draw_polygon and !_showStartEnd )
              _algo.AddCluster(plane,
                           cluster_hits);
          }//end if(_draw_tracks)
        }//end if it is a track
      }//end only drawing clusters with more than min hits
    }//end loop over clusters
    
    return true;
  };

  bool MergeViewer::finalize() {

    return ClusterMerger::finalize();

  }

  void MergeViewer::PrintClusterInfo(UChar_t plane, size_t index){

    int index_offset = 0;
    //get the clusters (after merging)
    for (auto const &cl : ClusterMerger::GetManager(this->GetNumManager()-1).GetClusters()) {
      if (cl.plane_id.Plane == plane) break;
      else index_offset++;
    }
    
    index = index + index_offset;
    auto cl = ClusterMerger::GetManager().GetClusters().at(index);
    std::cout<<"**********PRINTING CLUSTER INFO**********"<<std::endl;
    std::cout<<"Slope 2D = "<<cl.slope_2d<<std::endl;
    std::cout<<"Opening Angle = "<<cl.opening_angle<<std::endl;
    std::cout<<"Sum charge = "<<cl.sum_charge<<std::endl;
    std::cout<<"Length = "<<cl.length<<std::endl;
    std::cout<<"Width = "<<cl.width<<std::endl;
    std::cout<<"N_Hits = "<<cl.N_Hits<<std::endl;
    std::cout<<"eigenvalue_principal = "<<cl.eigenvalue_principal<<std::endl;
    std::cout<<"modified hit density = "<<cl.modified_hit_density<<std::endl;
    std::cout<<"multi hit wires = "<<cl.multi_hit_wires<<std::endl;
  }
  
}
#endif

