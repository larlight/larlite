#ifndef ERTOOL_ERALGOFLASHMATCH_CXX
#define ERTOOL_ERALGOFLASHMATCH_CXX

#include "ERAlgoFlashMatch.h"
#include "OpT0Helper.h"
#include "OpT0Finder/Algorithms/NPtFilter.h"
#include "OpT0Finder/Algorithms/MaxNPEWindow.h"
#include "OpT0Finder/Algorithms/QWeightPoint.h"
namespace ertool {

  ERAlgoFlashMatch::ERAlgoFlashMatch(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoFlashMatch::Reset()
  {}

  void ERAlgoFlashMatch::AcceptPSet(const ::fcllite::PSet& cfg)
  {

    auto p = cfg.get_pset(Name());
    double step_size = p.get< double > ("StepSize");
    double zdiff_max = p.get< double > ("ZDiffMax");

    std::vector<double> xrange    = p.get< std::vector<double> >("XRange"         );

    std::vector<double> opdet_x_v = p.get< std::vector<double> >("OpDetPosition_X");
    std::vector<double> opdet_y_v = p.get< std::vector<double> >("OpDetPosition_Y");
    std::vector<double> opdet_z_v = p.get< std::vector<double> >("OpDetPosition_Z");

    _beam_dt_min = p.get<double>("BeamDTMin");
    _beam_dt_max = p.get<double>("BeamDTMax");

    if(opdet_x_v.size() != opdet_y_v.size() || opdet_x_v.size() != opdet_z_v.size())
      throw ERException("Optical Detector Position dimension does not match among x/y/z!");
    if(xrange.size()!=2)
      throw ERException("XRange must be length 2 vector!");
    if(xrange[0]<=0)
      throw ERException("XRange cannot be 0 or negative number!");
    if(zdiff_max<=0)
      throw ERException("ZDiffMax cannot be 0 or negative number!");

    _mgr.SetAlgo(new ::flashana::NPtFilter);
    _mgr.SetAlgo(new ::flashana::MaxNPEWindow);
    auto ptr = new ::flashana::QWeightPoint( opdet_x_v, opdet_y_v, opdet_z_v, step_size );
    ptr->SetMaxZDiff(step_size);
    _mgr.SetAlgo(ptr);

  }

  void ERAlgoFlashMatch::ProcessBegin()
  {}

  void ERAlgoFlashMatch::ProcessEnd(TFile* fout)
  {}

  bool ERAlgoFlashMatch::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    OpT0Helper helper;
    
    std::multimap<double,std::pair<NodeID_t,FlashID_t> > score_m;

    std::vector<NodeID_t> primary_id_v;

    for(auto const& primary_node_id : graph.GetPrimaryNodes() ) {

      auto const& primary_part = graph.GetParticle(primary_node_id);

      auto const& children_v = graph.GetAllDescendantNodes(primary_part.ID());

      std::vector<NodeID_t> shower_v, track_v;
      shower_v.reserve(children_v.size());
      track_v.reserve(children_v.size());

      for(auto const& id : children_v) {
	auto const& child_part = graph.GetParticle(id);
	switch(child_part.RecoType()) {
	case kShower:
	  shower_v.push_back(child_part.RecoID());
	  break;
	case kTrack:
	  track_v.push_back(child_part.RecoID());
	  break;
	default:
	  break;
	}
      }

      auto cluster = helper.GetQCluster(data,shower_v,track_v);

      _mgr.Emplace(std::move(cluster));
      
    }

    std::vector<FlashID_t> flash_id_v;
    flash_id_v.reserve(data.Flash().size());

    for(auto const& erflash : data.Flash()) {

      ::flashana::Flash_t f;
      f.x = erflash._x;
      f.y = erflash._y;
      f.pe_v.reserve(erflash._npe_v.size());
      for(auto const& v : erflash._npe_v)
	f.pe_v.push_back(v);
      f.time = erflash._t;
      _mgr.Emplace(std::move(f));
    }

    auto const res = _mgr.Match();

    for(auto const& match : res ) {
      auto const& nord_id  = primary_id_v[match.tpc_id];
      auto const& flash_id = flash_id_v[match.flash_id];
      graph.SetFlashID(nord_id,flash_id);

      auto const& flash = data.Flash(flash_id);

      if(flash._t < _beam_dt_min || flash._t > _beam_dt_max) {

	auto& part = graph.GetParticle(nord_id);
	part.SetParticleInfo(part.PdgCode(),
			     part.Mass(),
			     part.Vertex(),
			     part.Momentum(),
			     part.RecoScore(),
			     kCosmic);
      }      
    }

    return true;
  }

}

#endif
