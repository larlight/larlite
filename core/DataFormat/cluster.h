/**
 * \file cluster.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for cluster data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_CLUSTER_H
#define LARLITE_CLUSTER_H

#include "data_base.h"
#include "Base/GeoTypes.h"
#include "Base/GeoConstants.h"
namespace larlite{

  typedef int ID_t; ///< Cluster ID

  typedef enum {
    clStart,       ///< Represents the most likely start of the cluster
    clEnd,         ///< Represents the end, or the alternative start, of the cluster
    NEnds,         ///< End count
    clFirstEnd = 0 ///< Just an alias for loops
  } ClusterEnds_t; ///< Used to decide which end to use

  typedef enum {
    cmFit,          ///< Sums from the fitted hit values
    cmADC,          ///< Sums directly from ADC counts
    NChargeModes,   ///< End count
    cmFirstMode = 0 ///< Just an alias for loops
  } ChargeMode_t; ///< Used to decide which style of charge sum to use  typedef enum {

  const ID_t InvalidID = -1;

  /**
     \class cluster
     LArSoft Cluster class equivalent data container
  */
  class cluster : public data_base {
  public:
    
    /// Default constructor
    cluster() : data_base(data::kCluster) { clear_data(); }
    
    /// Default destructor
    virtual ~cluster(){}

    void clear_data();
    
    /// @{
    /// @name Accessors
    
    /// Number of hits in the cluster
    unsigned int NHits() const { return fNHits; }
    
    /** **********************************************************************
     * @brief Returns the wire coordinate of the start of the cluster
     * @return wire coordinate of the start of the cluster (may lie between wires)
     * @see EndWire(), WireCoord(), StartTick()
     *
     * The wire coordinate is in wire units (the homogenized coordinate),
     * but can have a fractional part describing the relative distance from
     * the previous wire.
     */
    float StartWire() const { return fEndWires[clStart]; }
    
    /**
     * @brief Returns the tick coordinate of the start of the cluster
     * @return tick coordinate of the start of the cluster (may br fractional)
     * @see EndTick(), TickCoord(), StartWire()
     *
     * The tick coordinate is in tick units (the homogenized coordinate),
     * but can have a fractional part describing the relative time from
     * the previous tick.
     */
    float StartTick() const { return fEndTicks[clStart]; }
    
    /**
     * @brief Returns the uncertainty on wire coordinate of the start of the cluster
     * @return uncertainty on wire coordinate of the start of the cluster
     * @see StartWire()
     *
     * The wire uncertainty is in wire units (as for StartWire()).
     */
    float SigmaStartWire() const { return fSigmaEndWires[clStart]; }
    
    /**
     * @brief Returns the uncertainty on tick coordinate of the start of the cluster
     * @return uncertainty on tick coordinate of the start of the cluster
     * @see StartTick()
     *
     * The tick uncertainty is in tick units (as for StartTick()).
     */
    float SigmaStartTick() const { return fSigmaEndTicks[clStart]; }
    
    
    /** **********************************************************************
     * @brief Returns the wire coordinate of the end of the cluster
     * @return wire coordinate of the end of the cluster (may lie between wires)
     * @see StartWire(), WireCoord(), EndTick()
     *
     * The "end" of the cluster is, in the more ambiguous cluster shapes,
     * defined as an alternative cluster start.
     * The wire coordinate is in wire units (the homogenized coordinate),
     * but can have a fractional part describing the relative distance from
     * the previous wire.
     */
    float EndWire() const { return fEndWires[clEnd]; }
    
    /**
     * @brief Returns the tick coordinate of the end of the cluster
     * @return tick coordinate of the end of the cluster (may be fractional)
     * @see StartTick(), TickCoord(), EndWire()
     *
     * The "end" of the cluster is, in the more ambiguous cluster shapes,
     * defined as an alternative cluster start.
     * The tick coordinate is in tick units (the homogenized coordinate),
     * but can have a fractional part describing the relative time from
     * the previous tick.
     */
    float EndTick() const { return fEndTicks[clEnd]; }
    
    /**
     * @brief Returns the uncertainty on wire coordinate of the end of the cluster
     * @return uncertainty on wire coordinate of the end of the cluster
     * @see EndWire()
     *
     * The wire uncertainty is in wire units (as for EndWire()).
     */
    float SigmaEndWire() const { return fSigmaEndWires[clEnd]; }
    
    /**
     * @brief Returns the uncertainty on tick coordinate of the end of the cluster
     * @return uncertainty on tick coordinate of the end of the cluster
     * @see EndTick()
     *
     * The tick uncertainty is in tick units (as for EndTick()).
     */
    float SigmaEndTick() const { return fSigmaEndTicks[clEnd]; }
    
    
    //@{
    /** **********************************************************************
     * @brief Returns the wire coordinate of one of the end sides of the cluster
     * @param side clStart for start, clEnd for end of the cluster
     * @return wire coordinate of the requested end of the cluster (may lie between wires)
     * @see StartWire(), EndWire(), TickCoord()
     *
     * The wire coordinate is in wire units (the homogenized coordinate),
     * but can have a fractional part describing the relative distance from
     * the previous wire.
     * 
     * For algorithms not distinguishing start and end, all the ends can be
     * tested by the loop:
     *     
     *     for (unsigned int side = recob::Cluster::clFirstEnd;
     *       side < recob::Cluster::NEnds; ++side)
     *     {
     *       float wire = cluster.WireCoord(side);
     *       float tick = cluster.TickCoord(side);
     *       // ...
     *     } // for
     *     
     */
    float WireCoord(ClusterEnds_t side) const { return fEndWires[side]; }
    float WireCoord(unsigned int side) const { return fEndWires[side]; }
    //@}
    
    //@{
    /**
     * @brief Returns the tick coordinate of one of the end sides of the cluster
     * @param side clStart for start, clEnd for end of the cluster
     * @return tick coordinate of the requested end of the cluster (may be fractional)
     * @see StartTick(), EndTick(), WireCoord()
     *
     * The tick coordinate is in tick units (the homogenized coordinate),
     * but can have a fractional part describing the relative time from
     * the previous tick.
     * 
     * For algorithms not distinguishing start and end, all the ends can be
     * tested by the loop:
     *     
     *     for (unsigned int side = recob::Cluster::clFirstEnd;
     *       side < recob::Cluster::NEnds; ++side)
     *     {
     *       float wire = cluster.WireCoord(side);
     *       float tick = cluster.TickCoord(side);
     *       // ...
     *     } // for
     *     
     */
    float TickCoord(ClusterEnds_t side) const { return fEndTicks[side]; }
    float TickCoord(unsigned int side) const { return fEndTicks[side]; }
    //@}
      
      
    //@{
    /**
     * @brief Returns the uncertainty on wire coordinate of one of the end sides of the cluster
     * @param side clStart for start, clEnd for end of the cluster
     * @return uncertainty on wire coordinate of the requested end of the cluster
     * @see SigmaStartWire(), SigmaEndWire(), SigmaTickCoord(), TimeCoord()
     *
     * Usage of this method is similar to WireCoord().
     */
    float SigmaWireCoord(ClusterEnds_t side) const { return fSigmaEndWires[side]; }
    float SigmaWireCoord(unsigned int side) const { return fSigmaEndWires[side]; }
    //@}
      
    //@{
    /**
     * @brief Returns the uncertainty on tick coordinate of one of the end sides of the cluster
     * @param side clStart for start, clEnd for end of the cluster
     * @return uncertainty on tick coordinate of the requested end of the cluster
     * @see SigmaStartTick(), SigmaEndTick(), SigmaWireCoord(), TimeCoord()
     *
     * Usage of this method is similar to TimeCoord().
     */
    float SigmaTickCoord(ClusterEnds_t side) const { return fSigmaEndTicks[side]; }
    float SigmaTickCoord(unsigned int side) const { return fSigmaEndTicks[side]; }
    //@}
      
      
    /** **********************************************************************
     * @brief Returns the charge on the first wire of the cluster
     * @return charge on the first wire in ADC counts, negative if not available
     * @see EndCharge(), EdgeCharge()
     * 
     * The returned value is in unit of ADC count, although it may be
     * fractional.
     * This value can be result of extrapolation or average from a range of hits.
     */
    float StartCharge() const { return fEndCharges[clStart]; }
      
    /**
     * @brief Returns the starting angle of the cluster
     * @return angle in radians
     * @see EndAngle(), Angle()
     * 
     * The angle of the group of hits at the start position of the cluster is
     * returned, defined @f$ \alpha \in [ -\pi, \pi ]@f$ and so that
     * @f$ \tan(\alpha) = dT/dW @f$ (or, more precisely,
     * `angle = atan2(dT, dW)`).
     * The elements are expressed in physical distances and therefore this
     * represents a physical angle on the plane orthogonal to the wires in
     * the view and containing the drift direction ("x"); the angle is 0 or
     * @f$ \pi @f$ when lying on the wire plane, @f$ \pm\pi/2 @f$ when
     * pointing into/from the wire plane.
     * The angle is pointing toward the inside of the cluster (that is,
     * @f$ dW @f$ is positive going from the first wire on).
     * This value can be result of extrapolation or average from a range of
     * hits.
     */
    float StartAngle() const { return fAngles[clStart]; }
      
    /**
     * @brief Returns the opening angle at the start of the cluster
     * @return opening angle in radians
     * @see EndOpeningAngle(), OpeningAngle()
     * 
     * The returned value is from physical coordinates and in the range
     * @f$[ 0, \pi ]@f$.
     */
    float StartOpeningAngle() const { return fOpeningAngles[clStart]; }
      
      
    /**
     * @brief Returns the charge on the last wire of the cluster
     * @return charge on the last wire in ADC counts, negative if not available
     * @see StartCharge(), EdgeCharge()
     * 
     * The returned value is in unit of ADC count, although it may be
     * fractional.
     * This value can be result of extrapolation or average from a range of
     * hits.
     */
    float EndCharge() const { return fEndCharges[clEnd]; }
      
    /**
     * @brief Returns the ending angle of the cluster
     * @return angle in radians
     * @see StartAngle(), Angle()
     * 
     * The angle of the group of hits at the end position of the cluster is
     * returned, defined @f$ \alpha \in [ -\pi, \pi ]@f$ and so that
     * @f$ \tan(\alpha) = dT/dW @f$ (or, more precisely,
     * `angle = atan2(dT, dW)`).
     * The elements are expressed in physical distances and therefore this
     * represents a physical angle on the plane orthogonal to the wires in
     * the view and containing the drift direction ("x"); the angle is 0 or
     * @f$ \pi @f$ when lying on the wire plane, @f$ \pm\pi/2 @f$ when
     * pointing into/from the wire plane.
     * The angle is pointing toward the outside of the cluster (that is,
     * @f$ dW @f$ is positive going toward the last wire).
     * This value can be result of extrapolation or average from a range of
     * hits.
     */
    float EndAngle() const { return fAngles[clEnd]; }
      
    /**
     * @brief Returns the opening angle at the end of the cluster
     * @return opening angle in radians
     * @see StartOpeningAngle(), OpeningAngle()
     * 
     * The returned value is from homogenized coordinates and in the range
     * @f$[ 0, \pi ]@f$.
     */
    float EndOpeningAngle() const { return fOpeningAngles[clEnd]; }
      
      
    //@{
    /**
     * @brief Returns the charge on the first or last wire of the cluster
     * @param side clStart for start, clEnd for end of the cluster
     * @return charge on the requested wire in ADC counts, negative if not available
     * @see StartCharge(), EndCharge()
     * 
     * The returned value is in unit of ADC count, although it may be
     * fractional.
     * This value can be result of extrapolation or average from a range of
     * hits.
     */
    float EdgeCharge(ClusterEnds_t side) const { return fEndCharges[side]; }
    float EdgeCharge(unsigned int side) const { return fEndCharges[side]; }
    //@}
      
    //@{
    /**
     * @brief Returns the angle at either end of the cluster
     * @param side clStart for start, clEnd for end of the cluster
     * @return angle in radians
     * @see StartAngle(), EndAngle()
     * 
     * The angle of the group of hits at the specified position of the cluster
     * is returned, defined @f$ \alpha \in [ -\pi, \pi ]@f$ and so that
     * @f$ \tan(\alpha) = dT/dW @f$ (or, more precisely,
     * `angle = atan2(dT, dW)`).
     * The elements are expressed in physical distances and therefore this
     * represents a physical angle on the plane orthogonal to the wires in
     * the view and containing the drift direction ("x"); the angle is 0 or
     * @f$ \pi @f$ when lying on the wire plane, @f$ \pm\pi/2 @f$ when
     * pointing into/from the wire plane.
     * The angle is pointing so that increasing wire number yields positive
     * @f$ dW @f$.
     * This value can be result of extrapolation or average from a range of
     * hits.
     */
    float Angle(ClusterEnds_t side) const { return fAngles[side]; }
    float Angle(unsigned int side) const { return fAngles[side]; }
    //@}
      
    //@{
    /**
     * @brief Returns the opening angle at either end of the cluster
     * @return opening angle in radians
     * @see StartOpeningAngle(), EndOpeningAngle()
     * 
     * The returned value is from homogenized coordinates and in the range
     * @f$[ 0, \pi ]@f$.
     * This value can be result of extrapolation or average from a range of
     * hits.
     */
    float OpeningAngle(ClusterEnds_t side) const
    { return fOpeningAngles[side]; }
    float OpeningAngle(unsigned int side) const
    { return fOpeningAngles[side]; }
    //@}
      
      
    /** **********************************************************************
     * @brief Returns the total charge of the cluster from hit shape
     * @return total charge of the cluster from hit shape, in ADC counts
     * @see IntegralStdDev(), IntegralAverage(), SummedADC(), Charge()
     *
     * The total charge is computed as the sum of the charge of all the hits.
     * The charge of a single hit includes the hit shape (fit) and is obtained
     * by recob::Hit::Integral().
     */
    float Integral() const { return fChargeSum[cmFit]; }
      
    /**
     * @brief Returns the standard deviation of the charge of the cluster hits
     * @return standard deviation of the charge of the cluster hits, in ADC counts
     * @see Integral(), IntegralAverage(), SummedADCstdDev(), ChargeStdDev()
     *
     * The charge of a single hit includes the hit shape (fit) and is obtained
     * by recob::Hit::Integral().
     * It should return 0 if less than two hits are available.
     */
    float IntegralStdDev() const { return fChargeStdDev[cmFit]; }
      
    /**
     * @brief Returns the average charge of the cluster hits
     * @return average of the charge of the cluster hits, in ADC counts
     * @see Integral(), IntegralStdDev(), SummedADCaverage(), ChargeAverage()
     *
     * The charge of a single hit includes the hit shape (fit) and is obtained
     * by recob::Hit::Integral().
     * It should return 0 if no hit is available.
     */
    float IntegralAverage() const { return fChargeAverage[cmFit]; }
      
      
    /** **********************************************************************
     * @brief Returns the total charge of the cluster from signal ADC counts
     * @return total charge of the cluster from signal ADC, in ADC counts
     * @see SummedADCstdDev(), SummedADCaverage(), Integral(), Charge()
     *
     * The total charge is computed as the sum of the charge of all the hits.
     * The charge of a single hit includes the signal ADC counts and is
     * obtained by recob::Hit::SummedADC().
     */
    float SummedADC() const { return fChargeSum[cmADC]; }
      
    /**
     * @brief Returns the standard deviation of the signal ADC counts of the cluster hits
     * @return standard deviation of the signal of the cluster hits, in ADC counts
     * @see SummedADC(), SummedADCaverage(), IntegralStdDev(), ChargeStdDev()
     *
     * The charge of a single hit includes the signal ADC counts and is
     * obtained by recob::Hit::SummedADC().
     * It should return 0 if less than two hits are available.
     */
    float SummedADCstdDev() const { return fChargeStdDev[cmADC]; }
      
    /**
     * @brief Returns the average signal ADC counts of the cluster hits
     * @return average of the signal of the cluster hits, in ADC counts
     * @see SummedADC(), SummedADCstdDev(), IntegralAverage(), ChargeAverage()
     *
     * The charge of a single hit includes the signal ADC counts and is
     * obtained by recob::Hit::SummedADC().
     * It should return 0 if no hit is available.
     */
    float SummedADCaverage() const { return fChargeAverage[cmADC]; }
      
      
    //@{
    /** **********************************************************************
     * @brief Returns the total charge of the cluster
     * @param mode cmFit to use fitted hit shape, cmADC for signal ADCs
     * @return total charge of the cluster, in ADC counts
     * @see ChargeStdDev(), ChargeAverage(), SummedADC(), Integral()
     *
     * The total charge is computed as the sum of the charge of all the hits.
     * The charge of a single hit comes from the fitted hit shape
     * (recob::Hit::Integral()) for cmFit, and signal ADC counts
     * (recob::Hit::SummedADC()) for cmADC.
     * 
     * @note Cluster class older than version 14 had a Charge() method too;
     * the new one is not strictly equivalent, although in practice replacing
     * the old `Charge()` with `Charge(cmFit)` should do the trick.
     * The recommended update is to use `Integral()` instead.
     */
    float Charge(ChargeMode_t mode) const { return fChargeSum[mode]; }
    float Charge(unsigned int mode) const { return fChargeSum[mode]; }
    //@}
      
    //@{
    /**
     * @brief Returns the standard deviation of charge of the cluster hits
     * @return standard deviation of charge of the cluster hits, in ADC counts
     * @see Charge(), ChargeAverage(), SummedADCstdDev(), IntegralStdDev()
     *
     * The charge of a single hit comes from the fitted hit shape
     * (recob::Hit::Integral()) for cmFit, and signal ADC counts
     * (recob::Hit::SummedADC()) for cmADC.
     * It should return 0 if less than two hits are available.
     */
    float ChargeStdDev(ChargeMode_t mode) const
    { return fChargeStdDev[mode]; }
    float ChargeStdDev(unsigned int mode) const
    { return fChargeStdDev[mode]; }
    //@}
      
    //@{
    /**
     * @brief Returns the average charge of the cluster hits
     * @return average of the charge of the cluster hits, in ADC counts
     * @see Charge(), ChargeStdDev(), SummedADCaverage(), IntegralAverage()
     *
     * The charge of a single hit comes from the fitted hit shape
     * (recob::Hit::Integral()) for cmFit, and signal ADC counts
     * (recob::Hit::SummedADC()) for cmADC.
     * It should return 0 if no hit is available.
     */
    float ChargeAverage(ChargeMode_t mode) const
    { return fChargeAverage[mode]; }
    float ChargeAverage(unsigned int mode) const
    { return fChargeAverage[mode]; }
    //@}
      
    /**
     * @brief Density of wires in the cluster with more than one hit
     * @return density of wires in the cluster with more than one hit
     *
     * Returns a quantity defined as NMultiHitWires / Length,
     * where NMultiHitWires is the number of wires which have more than just
     * one hit amd Length is an estimation of the length of the cluster, in
     * centimetres.
     */
    float MultipleHitDensity() const { return fMultipleHitDensity; }
      
      
    /// A measure of the cluster width, in homogenized units.
    float Width() const { return fWidth; }
     

    /**
     * @brief return whether this cluster is the result of merging
     * @details Clusters are often merged in larlite, so it can be useful 
     *          to keep track of whether or not the have been merged
     * @return boolean, true if this cluster is the output of merging.
     */
    bool IsMergedCluster()  const {return fIsMergedCluster;}
      

    /**
     * @brief For merged clusters, return the original producer
     * @details This is not necessarily the producer of the clusters this
     *          cluster has been made out of, if merging has multiple stages.
     *          Instead, this is the producer that is completely unmerged.  This
     *          is the producer that you want to get associations for.
     * @return string of the original producer name
     */
    std::string OriginalClusterProducer() const {return fOriginalClusterProducer;}

    /**
     * @brief return the indexes of the original cluster in the larlite counting scheme
     * @details Get the indexes of the original clusters for this particular cluster.  This 
     *          is useful if you want to figure out associations later.
     * @return vector of indexes
     */
    const std::vector<unsigned short> & OriginalClusterIndexes() const {return fOriginalClusterIndexes;}

    /**
     * @brief Identifier of this cluster
     * @return the identifier of this cluster
     * @see isValid()
     *
     * The identifier should be unique per event and per algorithm.
     * An invalid cluster can be defined by having an ID Cluster::InvalidID.
     */
    ID_t  ID() const { return fID; }
      
    /// Returns the view for this cluster
    larlite::geo::View_t View() const { return fView; }
      
    /// Returns the plane ID this cluster lies on
    larlite::geo::PlaneID Plane() const { return fPlaneID; }
      
    /// @}
    /// Returns whether geometry plane is valid
    bool hasPlane() const { return fPlaneID.isValid; }
      
    /// Returns if the cluster is valid (that is, if its ID is not invalid)
    bool isValid() const { return ID() != InvalidID; }
      
    friend bool operator <  (cluster const& a, cluster const& b);

    void set_start_wire(float w,float err);
    void set_start_tick(float t,float err);
    void set_start_charge(float q);
    void set_start_angle(float a);
    void set_start_opening(float o);
    void set_end_wire(float w,float err);
    void set_end_tick(float t,float err);
    void set_end_charge(float q);
    void set_end_angle(float a);
    void set_end_opening(float o);
    void set_integral(float m, float err, float average);
    void set_summedADC(float m, float err, float average);
    void set_n_hits(unsigned int n);
    void set_multiple_hit_density(float v);
    void set_width(float v);
    void set_id(ID_t id);
    void set_view(geo::View_t v);
    void set_planeID(const geo::PlaneID& id);

    void set_is_merged(bool b);
    void set_original_producer(std::string p);
    void set_original_indexes(std::vector<unsigned short> s);

  private:

    unsigned int fNHits; ///< Number of hits in the cluster
    
    /// @name Data referring to start and end of the cluster
    /// @{
    /// Wire coordinate of the start and end of the cluster (may lie between wires);
    /// index is intended to be of type ClusterEnds_t.
    float fEndWires[NEnds];
    
    /// Uncertainty on wire coordinate of the start and end of the cluster;
    /// index is intended to be of type ClusterEnds_t.
    float fSigmaEndWires[NEnds];
    
    /// Tick coordinate of the start and end of the cluster (may be set between ticks);
    /// index is intended to be of type ClusterEnds_t.
    float fEndTicks[NEnds];
    
    /// Uncertainty on tick coordinate of the start and end of the cluster;
    /// index is intended to be of type ClusterEnds_t.
    float fSigmaEndTicks[NEnds];
    
    /// Charge on the start and end wire of the cluster.
    /// This value can be result of extrapolation or average from a range of hits.
    /// index is intended to be of type ClusterEnds_t.
    float fEndCharges[NEnds];
    
    /// Angle of the start and end of the cluster, defined in [-pi,pi]
    /// and so that tan(angle) = dT/dW (or, more precisely, `angle = atan2(dT, dW)`).
    /// The elements are expressed in physical distances and therefore this
    /// represents a physical angle on the plane orthogonal to the wires in
    /// the view and containing the drift direction ("x"); the angle is 0 or
    /// @f$ \pi @f$ when lying on the wire plane, @f$ \pm\pi/2 @f$ when
    /// pointing into/from the wire plane.
    /// Index is intended to be of type ClusterEnds_t.
    float fAngles[NEnds];
    
    /// Opening angle of the cluster shape at the start and end of the cluster.
    /// The coordinates are expressed in physical distances and therefore this
    /// represents a physical opening angle on the plane orthogonal to the
    /// wires in the view and containing the drift direction ("x").
    /// Index is intended to be of type ClusterEnds_t.
    float fOpeningAngles[NEnds];
    /// @}
    
    
    /// @name Data derived from hit charge
    /// @{
    /// Sum of the charge of all hits in the cluster.
    /// Index is intended to be of type ChargeMode_t
    float fChargeSum[NChargeModes];
      
    /// Standard deviation of the charge of hits.
    /// Index is intended to be of type ChargeMode_t
    float fChargeStdDev[NChargeModes];
    
    ///< Average of the charge of all hits in the cluster (fChargeSum/NHits()).
    /// Index is intended to be of type ChargeMode_t
    float fChargeAverage[NChargeModes];
    /// @}
    
    /// Density of wires in the cluster with more than one hit.
    float fMultipleHitDensity;
    
    /// A measure of the cluster width, in homogenized units.
    float fWidth;
      
    /// Identifier of this cluster.
    /// It should be unique per event and per algorithm.
    /// An invalid cluster can be defined by having an ID Cluster::InvalidID.
    ID_t fID;
    
    geo::View_t fView; ///< View for this cluster
    
    geo::PlaneID fPlaneID; ///< Location of the start of the cluster


    // The following section is larlite specific information that gets produced in merging
    bool fIsMergedCluster; ///< True only if this cluster is the result of merging other clusters
    std::vector<unsigned short> fOriginalClusterIndexes;  ///< The original indexes of the clusters from merging
    std::string fOriginalClusterProducer;  ///< The producer of the original cluster before merging

  };
  
  /**
     \class event_cluster
     A collection storage class of multiple clusters.
  */
  class event_cluster : public std::vector<larlite::cluster>,
			public event_base {
    
  public:
    
    /// Default constructor
    event_cluster(std::string name="noname") : event_base(data::kCluster,name) {clear_data();}
    
    /// Default copy constructor
    event_cluster(const event_cluster& original)
      : std::vector<larlite::cluster>(original), event_base(original)
    {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
    /// Default destructor
    ~event_cluster(){}

  private:
    
  };
}
#endif
  
/** @} */ // end of doxygen group 
  
