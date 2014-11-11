//
// type-def file for ROOT
//

#ifndef DATAFORMAT_TYPEDEF_H
#define DATAFORMAT_TYPEDEF_H

// Header files included here
#include <TVector3.h>
#include <TMatrixD.h>
#include <TLorentzVector.h>

#include "larlite_dataformat_utils.h"
#include "data_base.h"
//#include "larlite_association.h"

#include "potsummary.h"

#include "rawdigit.h"

#include "wire.h"
#include "hit.h"
#include "cluster.h"
#include "cosmictag.h"
#include "opflash.h"
#include "ophit.h"
#include "seed.h"
#include "endpoint2d.h"
#include "spacepoint.h"
#include "track.h"
#include "shower.h"
#include "vertex.h"
#include "calorimetry.h"
#include "partid.h"
#include "pfpart.h"

#include "user_info.h"

#include "gtruth.h"
#include "mcshower.h"
#include "mctrack.h"
#include "mctrajectory.h"
#include "mcpart.h"
#include "mctree.h"
#include "mcflux.h"
#include "mcnu.h"
#include "mctruth.h"
#include "simch.h"

#include "treenode.h"

/*
#include "trigger.h"
*/
#include "storage_manager.h"
// Class forward declaration here

namespace larlite{

  class data_base;
  class event_base;
  class product_id;

  class potsummary;

  class rawdigit;
  class event_rawdigit;

  class wire;
  class event_wire;

  class hit;
  class event_hit;

  class cosmictag;
  class event_cosmictag;

  class ophit;
  class event_ophit;

  class opflash;
  class event_opflash;

  class cluster;
  class event_cluster;

  class endpoint2d;
  class event_endpoint2d;

  class spacepoint;
  class event_spacepoint;  

  class seed;
  class event_seed;
  
  class vertex;
  class event_vertex;

  class track;
  class event_track;

  class shower;
  class event_shower;

  class calorimetry;
  class event_calorimetry;
  
  class user_info;
  class event_user;

  class gtruth;
  class event_gtruth;

  class mcshower;
  class event_mcshower;

  class mctrack;
  class event_mctrack;

  class mcstep;
  class mctrajectory;

  class mcpart;
  class event_mcpart;

  class treenode;
  class mctree;
  class event_mcpart;

  class mcflux;
  class event_mcflux;

  class mcnu;
  class mctruth;
  class event_mctruth;

  class ide;
  class simch;
  class event_simch;

  class pfpart;
  class event_pfpart;

  class partid;
  class event_partid;

  /*
  class trigger;
  */
  class storage_manager;

}

class treenode;
#endif



