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
  _cTwoClusters = nullptr;
  _hits_log_z = true;
  ShowShowers(false);
  time2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
  wire2cm = larutil::GeometryUtilities::GetME()->WireToCm();
}

//#############################
void ClusterViewerAlgo::Reset()
//#############################
{
  // delete
  for (auto &h : _hAllHits)        { delete h; h = 0; }

  for (auto &hs : _hClusterHits)
    for (auto &h : hs)
    { delete h; h = 0; }

  for (auto &hs : _hShowerHits)
    for (auto &h : hs)
    { delete h; h = 0; }

  for (auto &gs : _gClusterStart)
    for (auto &g : gs)
    { delete g; g = 0; }

  for (auto &gs : _gClusterEnd)
    for (auto &g : gs)
    { delete g; g = 0; }

  for (auto &gs : _gClusterPolygon)
    for (auto &g : gs)
    { delete g; g = 0; }

  for (auto &gs : _gClusterHits)
    for (auto &g : gs)
    { delete g; g = 0; }

  for (auto &g : _gAllHits)
  { delete g; g = 0; }

  // clear
  _hAllHits.clear();
  _hShowerHits.clear();
  _hClusterHits.clear();
  _gClusterStart.clear();
  _gClusterEnd.clear();
  _gClusterPolygon.clear();
  _gClusterHits.clear();
  _gAllHits.clear();
  _xrange.clear();
  _yrange.clear();
  _range_set.clear();

  // resize
  _hAllHits.resize(_nplanes, nullptr);
  _hShowerHits.resize(_nplanes, std::vector<TH2D*>());
  _hClusterHits.resize(_nplanes, std::vector<TH2D*>());
  _gClusterStart.resize(_nplanes, std::vector<TGraph*>());
  _gClusterEnd.resize(_nplanes, std::vector<TGraph*>());
  _gClusterPolygon.resize(_nplanes, std::vector<TGraph*>());
  _gClusterHits.resize(_nplanes, std::vector<TGraph*>());
  _gAllHits.resize(_nplanes, nullptr);

  _xrange.resize(_nplanes, std::pair<double, double>(0, 0));
  _yrange.resize(_nplanes, std::pair<double, double>(0, 0));
  _range_set.resize(_nplanes, false);
}


//######################################################################
void ClusterViewerAlgo::SetRange(UChar_t plane,
                                 const std::pair<double, double> &xrange,
                                 const std::pair<double, double> &yrange)
//######################################################################
{
  //    double x = h.WireID().Wire * wire2cm;
  //    double y = h.PeakTime() * time2cm;
  if (plane >= _nplanes)
    throw ViewerException(Form("Invalid plane ID: %d", plane));

  if (ReadyTakeData())
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
  for (auto b : _range_set) status = status && b;

  if (raise && !status)
    throw ViewerException("X and Y min/max range not yet set for all planes!");

  return status;
}

//####################################################################################
void ClusterViewerAlgo::AddHits(const UChar_t plane,
                                const std::vector<std::pair<double, double> > &hits_xy,
                                const std::vector<double> &hits_charge)
//####################################################################################
{
  if (!ReadyTakeData(true)) return;
  if (plane >= _nplanes)
    throw ViewerException(Form("Invalid plane ID: %d", plane));
  if (hits_xy.size() != hits_charge.size())
    throw ViewerException(Form("hits_xy (length %zu) and hits_charge (length %zu) must have same length!",
                               hits_xy.size(),
                               hits_charge.size()
                              )
                         );

  if (_hAllHits.at(plane)) { delete _hAllHits.at(plane); _hAllHits.at(plane) = nullptr; }
  if (_gAllHits.at(plane)) { delete _gAllHits.at(plane); _gAllHits.at(plane) = nullptr; }

  _hAllHits.at(plane) = GetPlaneViewHisto(plane,
                                          Form("hAllHitsPlane%02d", plane),
                                          Form("Hits for Plane %d; Wire; Time", plane));
  _gAllHits.at(plane) = GetPlaneViewGraph(plane,
                                          Form("gAllHitsPlane%02d", plane),
                                          Form("Hits for Plane %d; Wire; Time", plane));
  for (size_t i = 0; i < hits_xy.size(); ++i) {
    //DavidC--keep track of this line
    _hAllHits.at(plane)->Fill(hits_xy.at(i).first, hits_xy.at(i).second, hits_charge.at(i));
    _gAllHits.at(plane)->SetPoint(_gAllHits.at(plane)->GetN(), hits_xy.at(i).first, hits_xy.at(i).second);
  }
}

//####################################################################################
void ClusterViewerAlgo::AddShowers(const UChar_t plane,
                                   const std::vector<std::pair<double, double> > &shower_hits)
//####################################################################################
{
  if (!ReadyTakeData(true)) return;
  if (plane >= _nplanes)
    throw ViewerException(Form("Invalid plane ID: %d", plane));

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

  for (size_t i = 0; i < shower_hits.size(); ++i) {
    _hShowerHits.at(plane).at(index)->Fill(shower_hits.at(i).first, shower_hits.at(i).second);
  }
  _hShowerHits.at(plane).at(index)->SetMarkerColor(index + 1);
  _hShowerHits.at(plane).at(index)->SetMarkerStyle(kFullStar);

}


//############################################################################################
void ClusterViewerAlgo::AddCluster(const UChar_t plane,
                                   const std::vector<std::pair<double, double> > &cluster_hits,
                                   bool zoomed_in)
//############################################################################################
{
  if (!ReadyTakeData(true)) return;
  if (plane >= _nplanes)
    throw ViewerException(Form("Invalid plane ID: %d", plane));

  size_t index = _hClusterHits.at(plane).size();

  // Create
  //For the not zoomed_in mode, draw entire-hit-collection x and y boundaries
  if (!zoomed_in) {

    _hClusterHits.at(plane).push_back(GetPlaneViewHisto(plane,
                                      Form("hClusterHitsPlane%02dCluster%03zu", plane, index),
                                      Form("Cluster Hits on Plane %d Cluster %zu; Wire; Time", plane, index)));
    _gClusterHits.at(plane).push_back(GetPlaneViewGraph(plane,
                                      Form("gClusterHitsPlane%02dCluster%03zu", plane, index),
                                      Form("Cluster Hits on Plane %d Cluster %zu; Wire; Time", plane, index)));
  }
  //For the zoomed_in mode (usually used in tandem with drawonecluster function),
  //compute custom x-y boundaries just based on this one cluster
  else {

    double tmpxmin = 9e9, tmpymin = 9e9, tmpxmax = -9e9, tmpymax = -9e9;
    //Loop over hits in this cluster, compute new x-y min and maxes
    for ( auto const& myxy : cluster_hits) {
      if ( myxy.first < tmpxmin ) tmpxmin = myxy.first;
      if ( myxy.first > tmpxmax ) tmpxmax = myxy.first;
      if ( myxy.second < tmpymin ) tmpymin = myxy.second;
      if ( myxy.second > tmpymax ) tmpymax = myxy.second;
    }

    _hClusterHits.at(plane).push_back(GetPlaneViewHisto(plane, tmpxmin, tmpymin, tmpxmax, tmpymax,
                                      Form("hClusterHitsPlane%02dCluster%03zu", plane, index),
                                      Form("Cluster Hits on Plane %d Cluster %zu; Wire; Time", plane, index)));
    _gClusterHits.at(plane).push_back(GetPlaneViewGraph(plane, tmpxmin, tmpymin, tmpxmax, tmpymax,
                                      Form("gClusterHitsPlane%02dCluster%03zu", plane, index),
                                      Form("Cluster Hits on Plane %d Cluster %zu; Wire; Time", plane, index)));
  }
  _gClusterStart.at(plane).push_back(nullptr);

  _gClusterEnd.at(plane).push_back(nullptr);

  _gClusterPolygon.at(plane).push_back(nullptr);

  // Fill
  for (auto const &h : cluster_hits) {
    _hClusterHits.at(plane).at(index)->Fill(h.first, h.second);
    _gClusterHits.at(plane).at(index)->SetPoint(_gClusterHits.at(plane).at(index)->GetN(), h.first, h.second);
  }
  // Set Color (skipping white and close-to-white)
  if (index + 1 != 10 && index + 1 != 19 ) {
    _hClusterHits.at(plane).at(index)->SetMarkerColor(index + 1);
  }
  else if (index + 1 == 10)
    _hClusterHits.at(plane).at(index)->SetMarkerColor(92);
  else if (index + 1 == 19)
    _hClusterHits.at(plane).at(index)->SetMarkerColor(99);

  _hClusterHits.at(plane).at(index)->SetMarkerStyle(kFullStar);
  _gClusterHits.at(plane).at(index)->SetMarkerStyle(kFullDotLarge);

}

//############################################################################################
void ClusterViewerAlgo::AddCluster(const UChar_t plane,
                                   const std::vector<std::pair<double, double> > &cluster_hits,
                                   const std::pair<double, double> &cluster_start,
                                   const std::pair<double, double> &cluster_end)
//############################################################################################
{
  if (!ReadyTakeData(true)) return;
  if (plane >= _nplanes)
    throw ViewerException(Form("Invalid plane ID: %d", plane));

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
                                     Form("gClusterStartPlane%02dCluster%03zu", plane, index),
                                     ""
                                                      )
                                    );

  _gClusterEnd.at(plane).push_back(GetPlaneViewGraph(plane,
                                   Form("gClusterEndPlane%02dCluster%03zu", plane, index),
                                   ""
                                                    )
                                  );

  _gClusterPolygon.at(plane).push_back(nullptr);

  // Fill
  for (auto const &h : cluster_hits)

    _hClusterHits.at(plane).at(index)->Fill(h.first, h.second);

  _gClusterStart.at(plane).at(index)->SetPoint(0, cluster_start.first, cluster_start.second);
  _gClusterEnd.at(plane).at(index)->SetPoint(0, cluster_end.first, cluster_end.second);

  // Set Color
  _hClusterHits.at(plane).at(index)->SetMarkerColor(index + 1);
  _hClusterHits.at(plane).at(index)->SetMarkerStyle(kFullStar);
  _gClusterStart.at(plane).at(index)->SetMarkerSize(2);
  _gClusterStart.at(plane).at(index)->SetMarkerStyle(30);
  _gClusterEnd.at(plane).at(index)->SetMarkerSize(2);
  _gClusterEnd.at(plane).at(index)->SetMarkerStyle(29);

}

//###############################################################################################
void ClusterViewerAlgo::AddCluster(const UChar_t plane,
                                   const std::vector<std::pair<double, double> > &cluster_hits,
                                   const std::pair<double, double> &cluster_start,
                                   const std::pair<double, double> &cluster_end,
                                   const std::vector<std::pair<double, double> > &cluster_polygon)
//###############################################################################################
{
  if (!ReadyTakeData(true)) return;
  if (plane >= _nplanes)
    throw ViewerException(Form("Invalid plane ID: %d", plane));

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
                                     Form("gClusterStartPlane%02dCluster%03zu", plane, index),
                                     ""
                                                      )
                                    );

  _gClusterEnd.at(plane).push_back(GetPlaneViewGraph(plane,
                                   Form("gClusterEndPlane%02dCluster%03zu", plane, index),
                                   ""
                                                    )
                                  );

  _gClusterPolygon.at(plane).push_back(GetPlaneViewGraph(plane,
                                       Form("gClusterPolygonPlane%02dCluster%03zu", plane, index),
                                       ""
                                                        )
                                      );

  // Fill
  for (auto const &h : cluster_hits)

    _hClusterHits.at(plane).at(index)->Fill(h.first, h.second);
  _gClusterStart.at(plane).at(index)->SetPoint(0, cluster_start.first, cluster_start.second);
  _gClusterEnd.at(plane).at(index)->SetPoint(0, cluster_end.first, cluster_end.second);

  for (size_t i = 0; i < cluster_polygon.size(); ++i)

    _gClusterPolygon.at(plane).at(index)->SetPoint(i,
        cluster_polygon.at(i).first,
        cluster_polygon.at(i).second);
  // Set Color
  _hClusterHits.at(plane).at(index)->SetMarkerColor(index + 1);
  _hClusterHits.at(plane).at(index)->SetMarkerStyle(kFullStar);
  _gClusterStart.at(plane).at(index)->SetMarkerSize(2);
  _gClusterStart.at(plane).at(index)->SetMarkerStyle(30);
  _gClusterEnd.at(plane).at(index)->SetMarkerSize(2);
  _gClusterEnd.at(plane).at(index)->SetMarkerStyle(29);
  _gClusterPolygon.at(plane).at(index)->SetMarkerColor(index + 1);
  _gClusterPolygon.at(plane).at(index)->SetLineColor(index + 1);
  _gClusterPolygon.at(plane).at(index)->SetMarkerStyle(20);
  _gClusterPolygon.at(plane).at(index)->SetMarkerSize(1);
}

//###############################################################################################
void ClusterViewerAlgo::AddCluster(const UChar_t plane,
                                   const std::vector<std::pair<double, double> > &cluster_hits,
                                   const std::vector<std::pair<double, double> > &cluster_polygon)
//###############################################################################################
{
  if (!ReadyTakeData(true)) return;
  if (plane >= _nplanes)
    throw ViewerException(Form("Invalid plane ID: %d", plane));

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
                                       Form("gClusterPolygonPlane%02dCluster%03zu", plane, index),
                                       ""
                                                        )
                                      );

  // Fill
  for (auto const &h : cluster_hits)

    _hClusterHits.at(plane).at(index)->Fill(h.first, h.second);

  for (size_t i = 0; i < cluster_polygon.size(); ++i)

    _gClusterPolygon.at(plane).at(index)->SetPoint(i,
        cluster_polygon.at(i).first,
        cluster_polygon.at(i).second);
  // Set Color
  _hClusterHits.at(plane).at(index)->SetMarkerColor(index + 1);
  _hClusterHits.at(plane).at(index)->SetMarkerStyle(kFullStar);
  _gClusterPolygon.at(plane).at(index)->SetMarkerColor(index + 1);
  _gClusterPolygon.at(plane).at(index)->SetLineColor(index + 1);
  _gClusterPolygon.at(plane).at(index)->SetMarkerStyle(20);
  _gClusterPolygon.at(plane).at(index)->SetMarkerSize(1);
}

//########################################################################
TH2D* ClusterViewerAlgo::GetPlaneViewHisto(const UChar_t plane,
    const std::string &name,
    const std::string &title) const
//########################################################################
{

  //Set number of x-bins equal to number of wires (in cm)
  //Set number of y-bins equal to the number of TDCs (in cm)
  if (plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d", plane));

  size_t n_bins_x = (_xrange.at(plane).second - _xrange.at(plane).first) / wire2cm;
  size_t n_bins_y = (_xrange.at(plane).second - _xrange.at(plane).first) / time2cm;

  double bin_width_x = wire2cm;
  
  n_bins_y /= 8;

  TH2D* h = new TH2D(Form("%sFor%s", name.c_str(), _name.c_str()),
                     title.c_str(),
                     n_bins_x + 1, _xrange.at(plane).first - (bin_width_x / 2), _xrange.at(plane).second + (bin_width_x / 2),
                     n_bins_y, _yrange.at(plane).first, _yrange.at(plane).second);
  return h;
}

//########################################################################
TH2D* ClusterViewerAlgo::GetPlaneViewHisto(const UChar_t plane,
    double xmin, double ymin, double xmax, double ymax,
    const std::string &name,
    const std::string &title) const
//########################################################################
{
  // double x = h.WireID().Wire * wire2cm;
  // double y = h.PeakTime() * time2cm;
  if (plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d", plane));
  //Set number of x-bins equal to number of wires (in cm)
  //Set number of y-bins equal to the number of TDCs (in cm)
  size_t n_bins_x = (xmax - xmin) / wire2cm;
  size_t n_bins_y = (ymax - ymin) / time2cm;

  TH2D* h = new TH2D(Form("%sFor%s", name.c_str(), _name.c_str()),
                     title.c_str(),
                     n_bins_x, xmin, xmax,
                     n_bins_y, ymin, ymax);

  return h;
}


//##########################################################################
TGraph* ClusterViewerAlgo::GetPlaneViewGraph(const UChar_t plane,
    double xmin, double ymin, double xmax, double ymax,
    const std::string &name,
    const std::string &title) const
//##########################################################################
{
  if (plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d", plane));
  TGraph* g = new TGraph;
  g->Set(0);
  g->GetXaxis()->SetRangeUser(xmin * 0.9, xmax * 1.1);
  g->SetMaximum(ymax * 1.1);
  g->SetMinimum(ymin * 0.9);
  g->SetTitle(title.c_str());
  g->SetName(Form("%sFor%s", name.c_str(), _name.c_str()));
  return g;
}

//##########################################################################
TGraph* ClusterViewerAlgo::GetPlaneViewGraph(const UChar_t plane,
    const std::string &name,
    const std::string &title) const
//##########################################################################
{
  if (plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d", plane));
  TGraph* g = new TGraph;
  g->Set(0);
  g->GetXaxis()->SetRangeUser(_xrange.at(plane).first * 0.9, _xrange.at(plane).second * 1.1);
  g->SetMaximum(_yrange.at(plane).second * 1.1);
  g->SetMinimum(_yrange.at(plane).first * 0.9);
  g->SetTitle(title.c_str());
  g->SetName(Form("%sFor%s", name.c_str(), _name.c_str()));
  return g;
}

//#######################################
void ClusterViewerAlgo::DrawAllClusters()
//#######################################
{
  if (!_cAllCluster) {
    _cAllCluster = new TCanvas(Form("cAllClusterFor%s", _name.c_str()),
                               Form("%s: Cluster Hits Wire vs. Time", _name.c_str()),
                               900, 600);
    _cAllCluster->Divide(2, _nplanes);
  }

  _cAllCluster->cd();
  for (UChar_t plane = 0; plane < _nplanes; ++plane) {

    _cAllCluster->cd((plane * 2 + 1));

    if (plane < _hClusterHits.size()) {

      for (size_t cindex = 0; cindex < _hClusterHits.at(plane).size(); ++cindex) {

        if (!cindex) {
          _hClusterHits.at(plane).at(cindex)->Draw();
          if ( _gClusterStart.at(plane).at(cindex)   ) _gClusterStart.at(plane).at(cindex)->Draw("P");
          if ( _gClusterEnd.at(plane).at(cindex)     ) _gClusterEnd.at(plane).at(cindex)->Draw("P");
          if (_gClusterPolygon.at(plane).at(cindex))
            _gClusterPolygon.at(plane).at(cindex)->Draw("PL");
        }
        else {
          _hClusterHits.at(plane).at(cindex)->Draw("sames");
          if ( _gClusterStart.at(plane).at(cindex)   ) _gClusterStart.at(plane).at(cindex)->Draw("P");
          if ( _gClusterEnd.at(plane).at(cindex)     ) _gClusterEnd.at(plane).at(cindex)->Draw("P");
          if (_gClusterPolygon.at(plane).at(cindex))
            _gClusterPolygon.at(plane).at(cindex)->Draw("PL");
        }
      }
    } else

      throw ViewerException(Form("Plane %zu is invalid!", (size_t)plane));

    _cAllCluster->cd((plane + 1) * 2);

    if (_hits_log_z)
      gPad->SetLogz();

    if ( !_showerColor && _hAllHits.at(plane))
      _hAllHits.at(plane)->Draw("COLZ");

    if ( _showerColor and  _hShowerHits.at(plane).size() ) {
      for (size_t cindex = 0; cindex < _hShowerHits.at(plane).size(); ++cindex) {

        if (!cindex) {
          _hShowerHits.at(plane).at(cindex)->Draw();
        }
        else {
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
  if (!_cOneCluster)
    _cOneCluster = new TCanvas(Form("cOneClusterFor%s", _name.c_str()),
                               Form("%s : Individual Cluster Start/End Points", _name.c_str()),
                               1000, 600);

  if (plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d", plane));

  if (index >= _hClusterHits.at(plane).size())
    throw ViewerException(Form("Invalid cluster index: %zu (for plane %d)", index, plane));

  _cOneCluster->cd();

  //what is range of hClusterHits?
  _hClusterHits.at(plane).at(index)->Draw();

  if ( _gClusterStart.at(plane).at(index)   ) _gClusterStart.at(plane).at(index)->Draw("P");
  if ( _gClusterEnd.at(plane).at(index)     ) _gClusterEnd.at(plane).at(index)->Draw("P");
  if ( _gClusterPolygon.at(plane).at(index) ) _gClusterPolygon.at(plane).at(index)->Draw("PL");

  _cOneCluster->Update();
}

void ClusterViewerAlgo::DrawOneClusterGraphAndHits(UChar_t plane, size_t index)
{
  if (!_cOneCluster)
    _cOneCluster = new TCanvas(Form("cOneClusterFor%s", _name.c_str()),
                               Form("%s : Individual Cluster Start/End Points", _name.c_str()),
                               1000, 600);

  if (plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d", plane));

  if (index >= _hClusterHits.at(plane).size())
    throw ViewerException(Form("Invalid cluster index: %zu (for plane %d)", index, plane));

  _cOneCluster->cd();

  //Set the graph range to extend 50cm outwards in each direction
  // _gAllHits.at(plane)->GetXaxis()->SetRangeUser(
  //   _gClusterHits.at(plane).at(index)->GetXaxis()->GetXmin() - 50,
  //   _gClusterHits.at(plane).at(index)->GetXaxis()->GetXmin() + 50);
  // _gAllHits.at(plane)->SetMaximum(_gClusterHits.at(plane).at(index)->GetMaximum() + 50.);
  // _gAllHits.at(plane)->SetMinimum(_gClusterHits.at(plane).at(index)->GetMinimum() - 50.);

  // _gAllHits.at(plane)->SetMarkerColor(kBlack);
  // _gAllHits.at(plane)->SetMarkerStyle(kOpenCircle);
  // _gAllHits.at(plane)->SetMarkerSize(.5);
  // _gAllHits.at(plane)->Draw("AP");

  //First draw cluster graph so the range of the plot is good
  //_gClusterHits.at(plane).at(index)->Draw("AP");

  double local_xmin = _gClusterHits.at(plane).at(index)->GetXaxis()->GetXmin() * 0.8;
  double local_xmax = _gClusterHits.at(plane).at(index)->GetXaxis()->GetXmax() * 1.2;
  double local_ymin = _gClusterHits.at(plane).at(index)->GetYaxis()->GetXmin() * 0.8;
  double local_ymax = _gClusterHits.at(plane).at(index)->GetYaxis()->GetXmax() * 1.2;

  _hAllHits.at(plane)->Draw("COLZ");
  _hAllHits.at(plane)->GetYaxis()->SetRangeUser(local_ymin, local_ymax);
  _hAllHits.at(plane)->GetXaxis()->SetRangeUser(local_xmin, local_xmax);

  _gClusterHits.at(plane).at(index)->SetMarkerStyle(kOpenCircle);
  _gClusterHits.at(plane).at(index)->SetMarkerSize(1);
  _gClusterHits.at(plane).at(index)->SetMarkerColor(kBlack);
  //_gClusterHits.at(plane).at(index)->SetMarkerColor(kBlack, 0.5);
  _gClusterHits.at(plane).at(index)->Draw("P");

  _cOneCluster->Modified();
  _cOneCluster->Update();

}

//#################################################################
void ClusterViewerAlgo::DrawTwoClusters(UChar_t plane, size_t index1, size_t index2)
//#################################################################
{
  if (!_cTwoClusters)
    _cTwoClusters = new TCanvas(Form("cTwoClustersFor%s", _name.c_str()),
                                Form("%s : Two Cluster Start/End Points", _name.c_str()),
                                500, 300);

  if (plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d", plane));

  if (index1 >= _hClusterHits.at(plane).size())
    throw ViewerException(Form("Invalid cluster index: %zu (for plane %d)",
                               index1,
                               plane)
                         );
  if (index2 >= _hClusterHits.at(plane).size())
    throw ViewerException(Form("Invalid cluster index: %zu (for plane %d)",
                               index2,
                               plane)
                         );
  _cTwoClusters->cd();

  _hClusterHits.at(plane).at(index1)->Draw();
  _hClusterHits.at(plane).at(index2)->Draw("same");

  if ( _gClusterStart.at(plane).at(index1)   ) _gClusterStart.at(plane).at(index1)->Draw("P");
  if ( _gClusterEnd.at(plane).at(index1)     ) _gClusterEnd.at(plane).at(index1)->Draw("P");
  if ( _gClusterPolygon.at(plane).at(index1) ) _gClusterPolygon.at(plane).at(index1)->Draw("PL");

  if ( _gClusterStart.at(plane).at(index2)   ) _gClusterStart.at(plane).at(index2)->Draw("P");
  if ( _gClusterEnd.at(plane).at(index2)     ) _gClusterEnd.at(plane).at(index2)->Draw("P");
  if ( _gClusterPolygon.at(plane).at(index2) ) _gClusterPolygon.at(plane).at(index2)->Draw("PL");

  _cTwoClusters->Update();
}




//###################################################
size_t ClusterViewerAlgo::ClusterCount(UChar_t plane)
//###################################################
{
  if (plane >= _nplanes) throw ViewerException(Form("Invalid plane ID: %d", plane));

  return _hClusterHits.at(plane).size();

}

}
#endif

