#ifndef LARLITE_core_DataFormat_data_type_H
#define LARLITE_core_DataFormat_data_type_H

#include "Base/DataFormatConstants.h"
#include "Base/messenger.h"
#include <exception>
#include <vector>

namespace recob {
  class Hit;
  class Cluster;
}

namespace sim {
  class MCShower;
}

namespace larlite {

  class trigger;
  class potsummary;
  class event_hit;
  class hit;
  class event_track;
  class event_trackmom;
  class event_mctruth;
  class event_gtruth;
  class event_mcpart;
  class event_mctree;
  class event_user;
  class event_spacepoint;
  class event_rawdigit;
  class event_wire;
  class event_hit;
  class event_cluster;
  class event_shower;
  class event_mcshower;
  class event_mctrack;
  class event_simch;
  class event_calorimetry;
  class event_vertex;
  class event_endpoint2d;
  class event_seed;
  class event_cosmictag;
  class event_opflash;
  class event_ophit;
  class event_mcflux;
  class event_pfpart;
  class event_partid;
  class event_gtruth;
  class event_minos;
  class event_ass;
  class event_pcaxis;
  class event_flashmatch;
  class event_fifo;
  class event_opdetwaveform;
  class event_simphotons;
  class event_mucsdata;
  class event_mucsreco;
  class event_pizeroroi;
  class event_auxsimch;
  class event_chstatus;
  template <typename T> class wrapper;

  /// Run data product class => enum type converter
  template <class T>
  data::RunDataType_t rundata_type() { 
    Message::send(msg::kERROR,
		  __PRETTY_FUNCTION__,
		  "No corresponding data::RunDataType_t enum value found!");
    throw std::exception();
    return data::kRUNDATA_Undefined;
  }

  /// SubRun data product class => enum type converter
  template <class T>
  data::SubRunDataType_t subrundata_type() { 
    Message::send(msg::kERROR,
		  __PRETTY_FUNCTION__,
		  "No corresponding data::SubRunDataType_t enum value found!");
    throw std::exception();
    return data::kSUBRUNDATA_Undefined;
  }

  /// Event data product class => enum type converter
  template <class T>
  data::DataType_t data_type() {
    Message::send(msg::kERROR,
		  __PRETTY_FUNCTION__,
		  "No corresponding data::DataType_t enum value found!");
    throw std::exception();
    return data::kUndefined;
  }

  template<> data::DataType_t data_type<trigger> ();
  template<> data::DataType_t data_type<event_gtruth> ();
  template<> data::DataType_t data_type<event_mctruth> ();
  template<> data::DataType_t data_type<event_mcpart> ();
  template<> data::DataType_t data_type<event_mcflux> ();
  template<> data::DataType_t data_type<event_simch> ();
  template<> data::DataType_t data_type<event_mcshower> ();
  template<> data::DataType_t data_type<event_rawdigit> ();
  template<> data::DataType_t data_type<event_wire> ();
  template<> data::DataType_t data_type<event_hit> ();
  template<> data::DataType_t data_type<event_ophit> ();
  template<> data::DataType_t data_type<event_opflash> ();
  template<> data::DataType_t data_type<event_cluster> ();
  template<> data::DataType_t data_type<event_seed> ();
  template<> data::DataType_t data_type<event_spacepoint> ();
  template<> data::DataType_t data_type<event_track> ();
  template<> data::DataType_t data_type<event_trackmom> ();
  template<> data::DataType_t data_type<event_shower> ();
  template<> data::DataType_t data_type<event_vertex> ();
  template<> data::DataType_t data_type<event_endpoint2d> ();
  template<> data::DataType_t data_type<event_calorimetry> ();
  template<> data::DataType_t data_type<event_partid> ();
  template<> data::DataType_t data_type<event_pfpart> ();
  template<> data::DataType_t data_type<event_user> ();
  template<> data::DataType_t data_type<event_mctrack> ();
  template<> data::DataType_t data_type<event_mctree> ();
  template<> data::DataType_t data_type<event_minos> ();
  template<> data::DataType_t data_type<event_cosmictag>();
  template<> data::DataType_t data_type<event_fifo>();
  template<> data::DataType_t data_type<event_ass>();
  template<> data::DataType_t data_type<event_pcaxis>();
  template<> data::DataType_t data_type<event_flashmatch>();
  template<> data::DataType_t data_type<event_opdetwaveform> ();
  template<> data::DataType_t data_type<event_simphotons> ();
  template<> data::DataType_t data_type<event_mucsdata> ();
  template<> data::DataType_t data_type<event_mucsreco> ();
  template<> data::DataType_t data_type<event_pizeroroi> ();
  template<> data::DataType_t data_type<event_auxsimch> ();
  template<> data::DataType_t data_type<event_chstatus> ();
  template<> data::DataType_t data_type<wrapper<std::vector<recob::Hit> > > ();
  template<> data::DataType_t data_type<wrapper<std::vector<recob::Cluster> > > ();
  template<> data::DataType_t data_type<wrapper<std::vector<sim::MCShower> > > ();
  template<> data::SubRunDataType_t subrundata_type<potsummary>();
}
#endif
