/**
 * \file ClusterViewerAlgo.h
 *
 * \ingroup CMTool
 *
 * \brief Class def header for a class ClusterViewerAlgo
 *
 * @author kazuhiro/kaleko
 */

/** \addtogroup CMTool

    @{*/

#ifndef RECOTOOL_CLUSTERVIEWERALGO_H
#define RECOTOOL_CLUSTERVIEWERALGO_H

#include "ViewerException.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryUtilities.h"
#include <TH2D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TPad.h>

namespace cluster {
/**
   \class ClusterViewerAlgo
   User custom analysis class made by kazuhiro/kaleko
*/


class ClusterViewerAlgo {
   
public:

    /// Default constructor
    ClusterViewerAlgo(std::string name = "DeafultViewer");

    /// Default destructor
    virtual ~ClusterViewerAlgo() {};

    /// Resets settings
    void Reset();

    /**
       Function to check if it is ready to take data using AddXXX functions
       If raise is set to true, this function throws an exception.
    */
    bool ReadyTakeData(bool raise = false) const;

    /// Setter for x and y range ... must be called before any of AddXXX functions and/or after Reset()
    void SetRange(UChar_t plane,
                  const std::pair<double, double> &xrange,
                  const std::pair<double, double> &yrange);

    /// Appender for hits
    void AddHits(const UChar_t plane,
                 const std::vector<std::pair<double, double> > &hits_xy,
                 const std::vector<double> &hits_charge);

    /// Appender for hits
    void AddShowers(const UChar_t plane,
                    const std::vector<std::pair<double, double> > &shower_hits);

    /// Appender for clusters
    void AddCluster(const UChar_t plane,
                    const std::vector<std::pair<double, double> > &cluster_hits,
                    bool zoomed_in = false);

    /// Appender for clusters
    void AddCluster(const UChar_t plane,
                    const std::vector<std::pair<double, double> > &cluster_hits,
                    const std::pair<double, double> &cluster_start,
                    const std::pair<double, double> &cluster_end);

    /// Appender for clusters
    void AddCluster(const UChar_t plane,
                    const std::vector<std::pair<double, double> > &cluster_hits,
                    const std::vector<std::pair<double, double> > &cluster_polygon);

    /// Appender for clusters
    void AddCluster(const UChar_t plane,
                    const std::vector<std::pair<double, double> > &cluster_hits,
                    const std::pair<double, double> &cluster_start,
                    const std::pair<double, double> &cluster_end,
                    const std::vector<std::pair<double, double> > &cluster_polygon);

    /// A function to draw all clusters on a dedicated canvas
    void DrawAllClusters();

    /// A function to draw each cluster on a separate canvas than the one with all clusters
    void DrawOneCluster(UChar_t plane, size_t index);

    /// Function to draw one cluster (as a trgraph) with nearby hits (as XXX) overlaid
    void DrawOneClusterGraphAndHits(UChar_t plane, size_t index);

    /// A function to draw two clusters on a separat canvas than the one with all clusters.
    void DrawTwoClusters(UChar_t plane, size_t index1, size_t index2);

    /// A function to count # clusters in the given plane
    size_t ClusterCount(UChar_t plane);

    /// A function to decide if to show shower-coded hits or charge-coded hits
    void ShowShowers(bool on) { _showerColor = on; }

    /// A function to decide if the hits COLZ canvas should be shown in log-z scale
    void SetHitsLogZ(bool flag) { _hits_log_z = flag; }


protected:

    /// A utility function to create TH2D histogram
    TH2D* GetPlaneViewHisto(const UChar_t plane,
                            const std::string &name,
                            const std::string &title) const;

    /// Overloaded definition if user wants to supply custom x-y min-max values
    TH2D* GetPlaneViewHisto(const UChar_t plane,
                            double xmin, double ymin, double xmax, double ymax,
                            const std::string &name,
                            const std::string &title) const;

    /// A utility function to create TGraph
    TGraph* GetPlaneViewGraph(const UChar_t plane,
                              const std::string &name,
                              const std::string &title) const;

    /// Overloaded definition if user wants to supply custom x-y min-max values
    TGraph* GetPlaneViewGraph(const UChar_t plane,
                              double xmin, double ymin, double xmax, double ymax,
                              const std::string &name,
                              const std::string &title) const;
protected:

    //--- Run control variables ---//
    /// Name ... has to be unique per algo instance
    std::string _name;
    /// Stores # of planes
    UChar_t _nplanes;
    /// Edge of wire vs. time view per plane
    std::vector<std::pair<double, double> > _xrange, _yrange;
    /// Boolean to confirm if range is set by a user
    std::vector<bool> _range_set;
    /// Boolean: 0-show simch. 1-show hits associated to mcshowers
    bool _showerColor;

    //---- Canvas ----//
    TCanvas* _cAllCluster;
    TCanvas* _cOneCluster;
    TCanvas* _cTwoClusters;

    //---- If hits canvas will use logZ scale ----//
    bool _hits_log_z;

    //---- Things to be drawn (DATA) ----//

    /// 2D hit map (wire [cm] vs. time [cm]) ... index = plane
    std::vector<TH2D*> _hAllHits;
    /// 2D hit map (wire [cm] vs. time [cm]) ... 1st index = plane, 2nd index = cluster
    std::vector<std::vector<TH2D*> >   _hClusterHits;
    /// 2D hit map (wire [cm] vs. time [cm]) ... 1st index = plane, 2nd index = MCshower
    std::vector<std::vector<TH2D*> >   _hShowerHits;
    /// Cluster start point (wire [cm], time [cm]) ... 1st index = plane, 2nd index = cluster
    std::vector<std::vector<TGraph*> > _gClusterStart;
    /// Cluster end point (wire [cm], time [cm]) ... 1st index = plane, 2nd index = cluster
    std::vector<std::vector<TGraph*> > _gClusterEnd;
    /// Cluster polygon points in (wire [cm], time [cm]) ... 1st index = plane, 2nd index = cluster
    std::vector<std::vector<TGraph*> > _gClusterPolygon;
    /// Cluster hit points in (wire [cm], time [cm]) ... 1st index = plane, 2nd index = cluster
    std::vector<std::vector<TGraph*> > _gClusterHits;
    /// All hits in the event, as a massive tgraph. Index in vector is plane.
    std::vector<TGraph*> _gAllHits;

    double time2cm;
    double wire2cm;
};

}
#endif

/** @} */ // end of doxygen group
