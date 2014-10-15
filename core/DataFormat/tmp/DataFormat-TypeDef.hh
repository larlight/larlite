//
// type-def file for ROOT
//

#ifndef DATAFORMAT_TYPEDEF_HH
#define DATAFORMAT_TYPEDEF_HH

// Header files included here
#include "data_base.hh"
#include "mctrajectory.hh"
#include "mcpart.hh"
#include "mcnu.hh"
#include "mctruth.hh"
#include "mcshower.hh"
#include "simch.hh"
#include "wire.hh"
#include "fifo.hh"
#include "pmtfifo.hh"
#include "tpcfifo.hh"
#include "pulse.hh"
#include "trigger.hh"
#include "hit.hh"
#include "user_info.hh"
#include "track.hh"
#include "cluster.hh"
#include "spacepoint.hh"
#include "shower.hh"
#include "calorimetry.hh"
#include "vertex.hh"
#include "cosmictag.hh"
#include "opflash.hh"
#include "ophit.hh"
#include "storage_manager.hh"
// Class forward declaration here

namespace larlight{

  class data_base;
  class event_base;

  class mcstep;
  class mctrajectory;

  class mcpart;
  class event_mcpart;

  class mcnu;

  class mctruth;
  class event_mctruth;

  class mcshower;
  class event_mcshower;

  class ide;
  class simch;
  class event_simch;
  
  class wire;
  class event_wire;
  
  class hit;
  class event_hit;

  class cluster;
  class event_cluster;

  class fifo;
  class event_fifo;

  class tpcfifo;
  class event_tpcfifo;

  class pmtfifo;
  class event_pmtfifo;

  class pulse;  
  class event_pulse;

  class trigger;
  
  class spacepoint;
  class event_sps;
  
  class user_info;
  class event_user;
  
  class track;
  class event_track;

  class shower;
  class event_shower;

  class calorimetry;
  class event_calorimetry;

  class vertex;
  class event_vertex;

  class cosmictag;

  class ophit;
  class opflash;

  class storage_manager;
}
#endif



