////////////////////////////////////////////////////////////////////////
///
/// \file  Simulation/AuxDetSimChannel.cxx
///
/// \author  miceli@fnal.gov
///
////////////////////////////////////////////////////////////////////////


// our header
//#include "larsim/Simulation/AuxDetSimChannel.h"
//#include "AuxDetSimChannel.h"
#include "auxsimch.h"
#include "Base/DataFormatConstants.h"
// C/C++ standard library
#include <limits> // std::numeric_limits<>
#include <stdexcept>

// LArSoft headers
//#include "larcore/SimpleTypesAndConstants/PhysicalConstants.h" // util::kBogusX

namespace larlite{

  // Default constructor
  //-------------------------------------------------
  auxide::auxide()
    : trackID        (data::kINVALID_INT)
    , energyDeposited(data::kINVALID_FLOAT)
    , entryX         (data::kINVALID_FLOAT)
    , entryY         (data::kINVALID_FLOAT)
    , entryZ         (data::kINVALID_FLOAT)
    , entryT         (data::kINVALID_FLOAT)
    , exitX          (data::kINVALID_FLOAT)
    , exitY          (data::kINVALID_FLOAT)
    , exitZ          (data::kINVALID_FLOAT)
    , exitT          (data::kINVALID_FLOAT)
    , exitMomentumX  (data::kINVALID_FLOAT)
    , exitMomentumY  (data::kINVALID_FLOAT)
    , exitMomentumZ  (data::kINVALID_FLOAT)
  /*
    : trackID        (util::kBogusI)
    , energyDeposited(util::kBogusF)
    , entryX         (util::kBogusF)
    , entryY         (util::kBogusF)
    , entryZ         (util::kBogusF)
    , entryT         (util::kBogusF)
    , exitX          (util::kBogusF)
    , exitY          (util::kBogusF)
    , exitZ          (util::kBogusF)
    , exitT          (util::kBogusF)
    , exitMomentumX  (util::kBogusF)
    , exitMomentumY  (util::kBogusF)
    , exitMomentumZ  (util::kBogusF)
  */
    {}

  // Copy with offset constructor
  //-------------------------------------------------
  auxide::auxide(auxide const& ide, int offset)
    : trackID        (ide.trackID+offset)
    , energyDeposited(ide.energyDeposited)
    , entryX         (ide.entryX)
    , entryY         (ide.entryY)
    , entryZ         (ide.entryZ)
    , entryT         (ide.entryT)
    , exitX          (ide.exitX)
    , exitY          (ide.exitY)
    , exitZ          (ide.exitZ)
    , exitT          (ide.exitT)
    , exitMomentumX  (ide.exitMomentumX)
    , exitMomentumY  (ide.exitMomentumY)
    , exitMomentumZ  (ide.exitMomentumZ)
    {}

  //----------------------------------------------------------------------------
  auxsimch::auxsimch()
    : data_base(data::kAuxDetSimChannel)
    , fAuxDetID(std::numeric_limits<unsigned int>::max())
    , fAuxDetSensitiveID(std::numeric_limits<unsigned int>::max())
  {
    clear_data();
  }

  //----------------------------------------------------------------------------
  auxsimch::auxsimch(unsigned int inputAuxDetID,
		     unsigned int inputAuxDetSensitiveID)
    : data_base(data::kAuxDetSimChannel) 
    , fAuxDetID(inputAuxDetID)
    , fAuxDetSensitiveID(inputAuxDetSensitiveID)
  {
    clear_data();
  }
  
  //----------------------------------------------------------------------------
  auxsimch::auxsimch(unsigned int inputAuxDetID, 
		     const std::vector<larlite::auxide>& inputAuxDetIDEs,
		     unsigned int inputAuxDetSensitiveID)
    : data_base(data::kAuxDetSimChannel)
    , fAuxDetID(inputAuxDetID)
    , fAuxDetSensitiveID(inputAuxDetSensitiveID)
    , fAuxDetIDEs(inputAuxDetIDEs)
  {}

  //----------------------------------------------------------------------------
  auxsimch::auxsimch(unsigned int inputAuxDetID, 
		     std::vector<larlite::auxide>&& inputAuxDetIDEs,
		     unsigned int inputAuxDetSensitiveID)
    : data_base(data::kAuxDetSimChannel)
    , fAuxDetID(inputAuxDetID)
    , fAuxDetSensitiveID(inputAuxDetSensitiveID)
    , fAuxDetIDEs(inputAuxDetIDEs)
  {}

  //----------------------------------------------------------------------------
  std::pair<int,int> auxsimch::MergeAuxDetSimChannel(const auxsimch& chan, 
						     int offset)
  {
    if(this->fAuxDetID != chan.AuxDetID() && this->fAuxDetSensitiveID != chan.AuxDetSensitiveID())
      throw std::runtime_error("ERROR AuxDetSimChannel Merge: Trying to merge different channels!");

    std::pair<int,int> range_trackID(std::numeric_limits<int>::max(),
				     std::numeric_limits<int>::min());

    for(auto const& ide : auxides()) {
      this->fAuxDetIDEs.emplace_back(ide,offset);

      if( ide.trackID+offset < range_trackID.first  )
	range_trackID.first = ide.trackID+offset;
      if( ide.trackID+offset > range_trackID.second )
	range_trackID.second = ide.trackID+offset;
    }

    return range_trackID;
  }

  //----------------------------------------------------------------------------
  bool auxsimch::operator<  (const larlite::auxsimch& other) const 
  { 
    if(fAuxDetID < other.AuxDetID() ) return true;

    return fAuxDetSensitiveID < other.AuxDetSensitiveID(); 
  }

  //----------------------------------------------------------------------------
  bool auxsimch::operator== (const larlite::auxsimch& other) const 
  { 
    return (fAuxDetID == other.AuxDetID() && fAuxDetSensitiveID == other.AuxDetSensitiveID()); 
  }

  
}//namespace sim
