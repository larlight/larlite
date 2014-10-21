/**
 * \file mcshower.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for mcshower data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef LARLITE_MCSHOWER_H
#define LARLITE_MCSHOWER_H

#include "data_base.h"

namespace larlite {
  /**
     \class mcshower
     LArSoft utility class (not data container) MCShowerProfile 
     implemented as a data product in Larlite.
  */
  class mcshower : public data_base {
    
  public:

    /// Default constructor
    mcshower() : data_base(data::kMCShower) {clear_data();}

    /// Default destructor
    virtual ~mcshower(){}

    /// Clear method
    virtual void clear_data();

    //--- Getters ---//

    /// Shower mother's PDGID
    int  MotherPDGID()   const { return fMotherPDGID;   }

    /// Shower mother's G4 track ID 
    unsigned int MotherTrackID() const { return fMotherTrackID; }

    /// Mother's creation process
    const std::string& MotherCreationProcess() const { return fMotherProcess; }

    /// Shower mother's start point position
    const std::vector<double>& MotherPosition() const { return fMotherVtx;      }

    /// Shower mother's start point momentum
    const std::vector<double>& MotherMomentum() const { return fMotherMomentum; }

    /// Array of daughters' track ID
    const std::vector<unsigned int>&  DaughterTrackID() const
    { return fDaughterTrackID; }

    /// (x,y,z,dE) vector as a weighted average of daughters' energy deposition points
    const std::vector<double>& DaughterPosition() const
    { return fDaughterVtx; }

    /// (x,y,z,dE) vector as a weighted average of daughters' energy deposition points
    const std::vector<double>& DaughterMomentum() const
    { return fDaughterMomentum; }

    /// Charge deposited by daughters per plane
    double Charge(const size_t plane) const;

    //--- Setters ---//

    void SetMotherID(int pdgid, unsigned int trackid)
    { fMotherPDGID = pdgid; fMotherTrackID = trackid; }

    void SetMotherProcess(const std::string& process)
    { fMotherProcess = process; }

    void SetMotherPoint(const std::vector<double> &vtx);

    void SetMotherMomentum(const std::vector<double> &mom);

    void SetDaughterPosition(const std::vector<double> &vtx);

    void SetDaughterMomentum(const std::vector<double> &mom);
    
    void SetDaughterTrackList(const std::vector<unsigned int> &list)
    { fDaughterTrackID = list; }

    void SetPlaneCharge(const std::vector<double>& q)
    { fPlaneCharge = q; }

  private:

    //---- Mother info ----//
    int  fMotherPDGID;                   ///< mother PDG code
    unsigned int fMotherTrackID;                 ///< mother G4 Track ID
    std::string fMotherProcess;            ///< mother creation process
    std::vector<double> fMotherVtx;      ///< mother position 4-vector @ generation
    std::vector<double> fMotherMomentum; ///< mother momentum 4-vector @ generation

    //---- Daughter info ----//
    std::vector<unsigned int>  fDaughterTrackID;  ///< Daughters' track ID
    std::vector<double> fDaughterVtx;      ///< Daughters' deposit point closest to the mother vtx
    std::vector<double> fDaughterMomentum; ///< Daughters' deposit sum momentum 4-vector

    //---- Charge per plane ----//
    std::vector<double> fPlaneCharge; ///< Charge deposit per plane

    ////////////////////////
    ClassDef(mcshower,5)
    ////////////////////////
      
  };

  /**
     \class event_mcshower
     A collection storage class of multiple mcshower
  */
  class event_mcshower : public std::vector<larlite::mcshower>,
			 public event_base {
    
  public:
    
    /// Default constructor
    event_mcshower(std::string name="noname") : event_base(data::kMCShower,name) {clear_data();}
    
    /// Default copy constructor
    event_mcshower(const event_mcshower& original) : std::vector<larlite::mcshower>(original),
						     event_base(original)
    {}
    
    /// Default destructor
    ~event_mcshower(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    //////////////////////////
    ClassDef(event_mcshower,1)
    //////////////////////////
  };
  
}
#endif

/** @} */ // end of doxygen group 
