#ifndef RECOTOOL_CLUSTERVIEWERALGO_CXX
#define RECOTOOL_CLUSTERVIEWERALGO_CXX

#include "ClusterViewerAlgo.h"

namespace cluster {
  //##################################################################
  ClusterViewerAlgo::ClusterViewerAlgo(std::string name) : _name(name)
  //##################################################################
  {
    _nplanes = larutil::Geometry::GetME()->Nplanes();
    _cAllCluster = nullptr;
    _cOneCluster = nullptr;
    _cTwoClusters= nullptr;
    _hits_log_z= true;
    ShowShowers(false);

  }

  //#############################
  void ClusterViewerAlgo::Reset()
  //#############################
  {
    // delete
    for(auto &h : _hAllHits)        { delete h; h=0; }

    for(auto &hs : _hClusterHits)
      for(auto &h : hs)
	{ delete h; h=0; }

    for(auto &hs : _hShowerHits)
      for(auto &h : hs)
	{ delete h; h=0; }

    for(auto &gs : _gClusterStart)
      for(auto &g : gs)
	{ delete g; g=0; }

    for(auto &gs : _gClusterEnd)
      for(auto &g : gs)
	{ delete g; g=0; }

    for(auto &gs : _gClusterPolygon)
      for(auto &g : gs)
	{ delete g; g=0; }

    // clear
    _hAllHits.clear();
    _hShowerHits.clear();
    _hClusterHits.clear();
    _gClusterStart.clear();
    _gClusterEnd.clear();
    _gClusterPolygon.clear();
    _xrange.clear();
    _yrange.clear();
    _range_set.clear();

    // resize
    _hAllHits.resize(_nplanes,nullptr);
    _hShowerHits.resize(_nplanes,std::vector<TH2D*>());
    _hClusterHits.resize(_nplanes,std::vector<TH2D*>());
    _gClusterStart.resize(_nplanes,std::vector<TGraph*>());
    _gClusterEnd.resize(_nplanes,std::vector<TGraph*>());
    _gClusterPolygon.resize(_nplanes,std::vector<TGraph*>());

    _xrange.resize(_nplanes,std::pair<double,double>(0,0));
    _yrange.resize(_nplanes,std::pair<double,double>(0,0));
    _range_set.resize(_nplanes,false);
  }


  //######################################################################
  void ClusterViewerAlgo::SetRange(UChar_t plane, 
				   const std::pair<double,double> &xrange,
				   const std::pair<double,double> &yrange)
  //######################################################################
  {
    if(plane >= _nplanes)
      throw ViewerException(Form("Invalid plane ID: %d",plane));

    if(ReadyTakeData())
      throw ViewerException("SetRange() can be called only after Reset()!");

    _xrange.at(plane) = xrange;
    _yrange.at(plane) = yrange;
    _range_set.at(plane) = true;
  }

  //#####################################################
  bool ClusterViewerAlgo::ReadyTakeData(bool raise) const
  //#####################################################
  {
    bool status = true;
    for(auto b : _range_set) status = status && b;

    if(raise && !status)
      throw ViewerException("X and Y min/max range not yet set for all planes!");

    return status;
  }

  //####################################################################################
  void ClusterViewerAlgo::AddHits(const UChar_t plane, 
				  const std::vector<std::pair<double,double> > &hits_xy,
				  const std::vector<double> &hits_charge)
  //####################################################################################
  {
    if(!ReadyTakeData(true)) return;
    if(plane >= _nplanes)
      throw ViewerException(Form("Invalid plane ID: %d",plane));
    if(hits_xy.size() != hits_charge.size())
      throw ViewerException(Form("hits_xy (length %zu) and hits_charge (length %zu) must have same length!",
				 hits_xy.size(),
				 hits_charge.size()
				 )
			    );

    if(_hAllHits.at(plane)) { delete _hAllHits.at(plane); _hAllHits.at(plane) = nullptr; }

    _hAllHits.at(plane) = GetPlaneViewHisto(plane,
					    Form("hAllHitsPlane%02d",plane),
					    Form("Hits for Plane %d; Wire; Time",plane));
    for(size_t i=0; i<hits_xy.size(); ++i)
      //DavidC--keep track of this line
      _hAllHits.at(plane)->Fill(hits_xy.at(i).first,hits_xy.at(i).second,hits_charge.at(i));
  }

  //####################################################################################
  void ClusterViewerAlgo::AddShowers(const UChar_t plane, 
				     const std::vector<std::pair<double,double> > &shower_hits)
  //####################################################################################
  {
    if(!ReadyTakeData(true)) return;
    if(plane >= _nplanes)
      throw ViewerException(Form("Invalid plane ID: %d",plane));

    size_t index = _hShowerHits.at(plane).size();
    _hShowerHits.at(plane).push_back(GetPlaneViewHisto(plane,
						       Form("hShowerHitsPlane%02dCluster%03zu",
							    plane,
							    index),
						       Form("Shower Hits on Plane %d Cluster %zu; Wire; Time",
							    plane,
							    index)
						       )
				      ); 
    
    for(size_t i=0; i<shower_hits.size(); ++i){
      _hShowerHits.at(plane).at(index)->Fill(shower_hits.at(i).first,shower_hits.at(i).second);
    }
    _hShowerHits.at(plane).at(index)->SetMarkerColor(index+1);
    _hShowerHits.at(plane).at(index)->SetMarkerStyle(kFullStar);

  }


  //############################################################################################
  void ClusterViewerAlgo::AddCluster(const UChar_t plane,
				     const std::vector<std::pair<double,double> > &cluster_hits)
  //############################################################################################
  {
    if(!ReadyTakeData(true)) return;
    if(plane >= _nplanes)
      throw ViewerException(Form("Invalid plane ID: %d",plane));

    size_t index = _hClusterHits.at(plane).size();

    // Create
    _hClusterHits.at(plane).push_back(GetPlaneViewHisto(plane,
							Form("hClusterHitsPlane%02dCluster%03zu",
							     plane,
							     index),
							Form("Cluster Hits on Plane %d Cluster %zu; Wire; Time",
							     plane,
							     index)
							)
				      ); 

    _gClusterStart.at(plane).push_back(nullptr);

    _gClusterEnd.at(plane).push_back(nullptr);
    
    _gClusterPolygon.at(plane).push_back(nullptr);

    // Fill
    for(auto const &h : cluster_hits)
      
      _hClusterHits.at(plane).at(index)->Fill(h.first,h.second);
    
    // Set Color (skipping white and close-to-white)
    if(index+1 != 10 && index+1 != 19 ){
      _hClusterHits.at(plane).at(index)->SetMarkerColor(index+1);
    }
    else if (index+1 == 10)
      _hClusterHits.at(plane).at(index)->SetMarkerColor(92);
    else if (index+1 == 19)
      _hClusterHits.at(plane).at(index)->SetMarkerColor(99);

    _hClusterHits.at(plane).at(index)->SetMarkerStyle(kFullStar);

  }

  //############################################################################################
  void ClusterViewerAlgo::AddCluster(const UChar_t plane,
				     const std::vector<std::pair<double,double> > &cluster_hits,
				     const std::pair<double,double> &cluster_start,
				     const std::pair<double,double> &cluster_end)
  //############################################################################################
  {
    if(!ReadyTakeData(true)) return;
    if(plane >= _nplanes)
      throw ViewerException(Form("Invalid plane ID: %d",plane));

    size_t index = _hClusterHits.at(plane).size();

    // Create
    _hClusterHits.at(plane).push_back(GetPlaneViewHisto(plane,
							Form("hClusterHitsPlane%02dCluster%03zu",
							     plane,
							     index),
							Form("Cluster Hits on Plane %d Cluster %zu; Wire; Time",
							     plane,
							     index)
							)
				      ); 

    _gClusterStart.at(plane).push_back(GetPlaneViewGraph(plane,
							 Form("gClusterStartPlane%02dCluster%03zu",plane,index),
							 ""
							 )
				       );
    
    _gClusterEnd.at(plane).push_back(GetPlaneViewGraph(plane,
						       Form("gClusterEndPlane%02dCluster%03zu",plane,index),
						       ""
						       )
				     );
    
    _gClusterPolygon.at(plane).push_back(nullptr);

    // Fill
    for(auto const &h : cluster_hits)
      
      _hClusterHits.at(plane).at(index)->Fill(h.first,h.second);

    _gClusterStart.at(plane).at(index)->SetPoint(0,cluster_start.first,cluster_start.second);
    _gClusterEnd.at(plane).at(index)->SetPoint(0,cluster_end.first,cluster_end.second);
    
    // Set Color
    _hClusterHits.at(plane).at(index)->SetMarkerColor(index+1);
    _hClusterHits.at(plane).at(index)->SetMarkerStyle(kFullStar);
    _gClusterStart.at(plane).at(index)->SetMarkerSize(2);
    _gClusterStart.at(plane).at(index)->SetMarkerStyle(30);
    _gClusterEnd.at(plane).at(index)->SetMarkerSize(2);
    _gClusterEnd.at(plane).at(index)->SetMarkerStyle(29);
    
  }
  
  //###############################################################################################
  void ClusterViewerAlgo::AddCluster(const UChar_t plane,
				     const std::vector<std::pair<double,double> > &cluster_hits,
				     const std::pair<double,double> &cluster_start,
				     const std::pair<double,double> &cluster_end,
				     const std::vector<std::pair<double,double> > &cluster_polygon)
  //###############################################################################################
  {
    if(!ReadyTakeData(true)) return;
    if(plane >= _nplanes)
      throw ViewerException(Form("Invalid plane ID: %d",plane));

    size_t index = _hClusterHits.at(plane).size();

    // Create
    _hClusterHits.at(plane).push_back(GetPlaneViewHisto(plane,
							Form("hClusterHitsPlane%02dCluster%03zu",
							     plane,
							     index),
							Form("Cluster Hits on Plane %d Cluster %zu; Wire; Time",
							     plane,
							     index)
							)
				      ); 

    _gClusterStart.at(plane).push_back(GetPlaneViewGraph(plane,
							 Form("gClusterStartPlane%02dCluster%03zu",plane,index),
							 ""
							 )
				       );
    
    _gClusterEnd.at(plane).push_back(GetPlaneViewGraph(plane,
						       Form("gClusterEndPlane%02dCluster%03zu",plane,index),
						       ""
						       )
				     );
    
    _gClusterPolygon.at(plane).push_back(GetPlaneViewGraph(plane,
							   Form("gClusterPolygonPlane%02dCluster%03zu",plane,index),
							   ""
							   )
					 );

    // Fill
    for(auto const &h : cluster_hits)
      
      _hClusterHits.at(plane).at(index)->Fill(h.first,h.second);
    _gClusterStart.at(plane).at(index)->SetPoint(0,cluster_start.first,cluster_start.second);
    _gClusterEnd.at(plane).at(index)->SetPoint(0,cluster_end.first,cluster_end.second);
    
    for(size_t i=0; i<cluster_polygon.size(); ++i)
      
      _gClusterPolygon.at(plane).at(index)->SetPoint(i,
						     cluster_polygon.at(i).first,
						     cluster_polygon.at(i).second);
    // Set Color
    _hClusterHits.at(plane).at(index)->SetMarkerColor(index+1);
    _hClusterHits.at(plane).at(index)->SetMarkerStyle(kFullStar);
    _gClusterStart.at(plane).at(index)->SetMarkerSize(2);
    _gClusterStart.at(plane).at(index)->SetMarkerStyle(30);
    _gClusterEnd.at(plane).at(index)->SetMarkerSize(2);
    _gClusterEnd.at(plane).at(index)->SetMarkerStyle(29);
    _gClusterPolygon.at(plane).at(index)->SetMarkerColor(index+1);
    _gClusterPolygon.at(plane).at(index)->SetLineColor(index+1);
    _gClusterPolygon.at(plane).at(index)->SetMarkerStyle(20);
    _gClusterPolygon.at(plane).at(index)->SetMarkerSize(1);
  }

  //###############################################################################################
  void ClusterViewerAlgo::AddCluster(const UChar_t plane,
				     const std::vector<std::pair<double,double> > &cluster_hits,
				     const std::vector<std::pair<double,double> > &cluster_polygon)
  //###############################################################################################
  {
    if(!ReadyTakeData(true)) return;
    if(plane >= _nplanes)
      throw ViewerException(Form("Invalid plane ID: %d",plane));

    size_t index = _hClusterHits.at(plane).size();

    // Create
    _hClusterHits.at(plane).push_back(GetPlaneViewHisto(plane,
							Form("hClusterHitsPlane%02dCluster%03zu",
							     plane,
							     index),
							Form("Cluster Hits on Plane %d Cluster %zu; Wire; Time",
							     plane,
							     index)
							)
				      ); 

    _gClusterStart.at(plane).push_back(nullptr);

    _gClusterEnd.at(plane).push_back(nullptr);
    
    _gClusterPolygon.at(plane).push_back(GetPlaneViewGraph(plane,
							   Form("gClusterPolygonPlane%02dCluster%03zu",plane,index),
							   ""
							   )
					 );

    // Fill
    for(auto const &h : cluster_hits)
      
      _hClusterHits.at(plane).at(index)->Fill(h.first,h.second);
    
    for(size_t i=0; i<cluster_polygon.size(); ++i)
      
      _gClusterPolygon.at(plane).at(index)->SetPoint(i,
						     cluster_polygon.at(i).first,
						     cluster_polygon.at(i).second);
    // Set Color
    _hClusterHits.at(plane).at(index)->SetMarkerColor(index+1);
    _hClusterHits.at(plane).at(index)->SetMarkerStyle(kFullStar);
    _gClusterPolygon.at(plane).at(index)->SetMarkerColor(index+1);
    _gClusterPolygon.at(plane).at(index)->SetLineColor(index+1);
    _gClusterPolygon.at(plane).at(index)->SetMarkerStyle(20);
    _gClusterPolygon.at(plane).at(index)->SetMarkerSize(1);
  }

  //########################################################################
  TH2D* ClusterViewerAlgo::GetPlaneViewHisto(const UChar_t plane,
					     const std::string &name, 
					     const std::string &title) const
  //########################################################################
  {
    if(plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d",plane));
    TH2D* h = new TH2D(Form("%sFor%s",name.c_str(),_name.c_str()),
		       title.c_str(),
		       100, _xrange.at(plane).first * 0.9, _xrange.at(plane).second * 1.1,
		       100, _yrange.at(plane).first * 0.9, _yrange.at(plane).second * 1.1);
    return h;
  }
  
  
  //##########################################################################
  TGraph* ClusterViewerAlgo::GetPlaneViewGraph(const UChar_t plane,
					       const std::string &name, 
					       const std::string &title) const
  //##########################################################################
  {
    if(plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d",plane));
    TGraph* g = new TGraph;
    g->Set(0);
    g->GetXaxis()->SetRangeUser(_xrange.at(plane).first * 0.9, _xrange.at(plane).second * 1.1);
    g->SetMaximum(_yrange.at(plane).second * 1.1);
    g->SetMinimum(_yrange.at(plane).first * 0.9);
    g->SetTitle(title.c_str());
    g->SetName(Form("%sFor%s",name.c_str(),_name.c_str()));
    return g;
  }

  //#######################################
  void ClusterViewerAlgo::DrawAllClusters() 
  //#######################################
  {
    if(!_cAllCluster) {
      _cAllCluster = new TCanvas(Form("cAllClusterFor%s",_name.c_str()),
				 Form("%s: Cluster Hits Wire vs. Time",_name.c_str()),
				 900,600);
      _cAllCluster->Divide(2,_nplanes);
    }

    _cAllCluster->cd();
    for(UChar_t plane=0; plane<_nplanes; ++plane) {
      
      _cAllCluster->cd((plane*2 + 1));

      if(plane < _hClusterHits.size()) {

	for(size_t cindex=0; cindex<_hClusterHits.at(plane).size(); ++cindex) {
	  
	  if(!cindex) {
	    _hClusterHits.at(plane).at(cindex)->Draw();
	    if( _gClusterStart.at(plane).at(cindex)   ) _gClusterStart.at(plane).at(cindex)->Draw("P");
	    if( _gClusterEnd.at(plane).at(cindex)     ) _gClusterEnd.at(plane).at(cindex)->Draw("P");
	    if(_gClusterPolygon.at(plane).at(cindex))
	      _gClusterPolygon.at(plane).at(cindex)->Draw("PL");
	}
	  else{
	    _hClusterHits.at(plane).at(cindex)->Draw("sames");
	    if( _gClusterStart.at(plane).at(cindex)   ) _gClusterStart.at(plane).at(cindex)->Draw("P");
	    if( _gClusterEnd.at(plane).at(cindex)     ) _gClusterEnd.at(plane).at(cindex)->Draw("P");
	    if(_gClusterPolygon.at(plane).at(cindex))
	      _gClusterPolygon.at(plane).at(cindex)->Draw("PL");
	  }
	}
      }else
	
	throw ViewerException(Form("Plane %zu is invalid!",(size_t)plane));

      _cAllCluster->cd((plane+1)*2);
      
      if(_hits_log_z)
	gPad->SetLogz();
      
      if ( !_showerColor && _hAllHits.at(plane))
	_hAllHits.at(plane)->Draw("COLZ");

      if ( _showerColor and  _hShowerHits.at(plane).size() ){
	for(size_t cindex=0; cindex<_hShowerHits.at(plane).size(); ++cindex) {
	  
	  if(!cindex) {
	    _hShowerHits.at(plane).at(cindex)->Draw();
	  }
	  else{
	    _hShowerHits.at(plane).at(cindex)->Draw("sames");
	  }
	}
      }


    }
    _cAllCluster->Update();
  }

  //#################################################################
  void ClusterViewerAlgo::DrawOneCluster(UChar_t plane, size_t index)
  //#################################################################
  {
    if(!_cOneCluster)
      _cOneCluster = new TCanvas(Form("cOneClusterFor%s",_name.c_str()),
				 Form("%s : Individual Cluster Start/End Points",_name.c_str()),
				 500,300);

    if(plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d",plane));

    if(index >= _hClusterHits.at(plane).size()) throw ViewerException(Form("Invalid cluster index: %zu (for plane %d)",
									   index,
									   plane)
								      );
    _cOneCluster->cd();
    
    _hClusterHits.at(plane).at(index)->Draw();

    if( _gClusterStart.at(plane).at(index)   ) _gClusterStart.at(plane).at(index)->Draw("P");
    if( _gClusterEnd.at(plane).at(index)     ) _gClusterEnd.at(plane).at(index)->Draw("P");
    if( _gClusterPolygon.at(plane).at(index) ) _gClusterPolygon.at(plane).at(index)->Draw("PL");
    _cOneCluster->Update();
  }


  //#################################################################
  void ClusterViewerAlgo::DrawTwoClusters(UChar_t plane, size_t index1, size_t index2)
  //#################################################################
  {
    if(!_cTwoClusters)
      _cTwoClusters = new TCanvas(Form("cTwoClustersFor%s",_name.c_str()),
				  Form("%s : Two Cluster Start/End Points",_name.c_str()),
				  500,300);
    
    if(plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d",plane));
    
    if(index1 >= _hClusterHits.at(plane).size())
      throw ViewerException(Form("Invalid cluster index: %zu (for plane %d)",
				 index1,
				 plane)
			    );
    if(index2 >= _hClusterHits.at(plane).size()) 
      throw ViewerException(Form("Invalid cluster index: %zu (for plane %d)",
				 index2,
				 plane)
			    );
    _cTwoClusters->cd();
    
    _hClusterHits.at(plane).at(index1)->Draw();
    _hClusterHits.at(plane).at(index2)->Draw("same");

    if( _gClusterStart.at(plane).at(index1)   ) _gClusterStart.at(plane).at(index1)->Draw("P");
    if( _gClusterEnd.at(plane).at(index1)     ) _gClusterEnd.at(plane).at(index1)->Draw("P");
    if( _gClusterPolygon.at(plane).at(index1) ) _gClusterPolygon.at(plane).at(index1)->Draw("PL");

    if( _gClusterStart.at(plane).at(index2)   ) _gClusterStart.at(plane).at(index2)->Draw("P");
    if( _gClusterEnd.at(plane).at(index2)     ) _gClusterEnd.at(plane).at(index2)->Draw("P");
    if( _gClusterPolygon.at(plane).at(index2) ) _gClusterPolygon.at(plane).at(index2)->Draw("PL");

    _cTwoClusters->Update();
  }




  //###################################################
  size_t ClusterViewerAlgo::ClusterCount(UChar_t plane)
  //###################################################
  {
    if(plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d",plane));

    return _hClusterHits.at(plane).size();

  }
  
}
#endif

