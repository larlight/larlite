#ifndef ERTOOL_PARTICLE_CXX
#define ERTOOL_PARTICLE_CXX

#include "Particle.h"
#include <iostream>
#include "ERException.h"

namespace ertool {

  Particle::Particle(const NodeID_t node_id,
		     const RecoType_t reco_type,
		     const RecoID_t reco_id)
    : _pdg_code(kINVALID_INT)
    , _mass(kINVALID_DOUBLE)
    , _vtx(kINVALID_VERTEX)
    , _mom(kINVALID_MOMENTUM)
    , _node_id(node_id)
    , _parent_id(node_id)
    , _ancestor_id(node_id)
    , _generation(kDefaultGeneration)
    , _child_v()
    , _reco_type(reco_type)
    , _reco_id(reco_id)
  {
    _primary = 0 ;
  }

  const NodeID_t&     Particle::ID         () const { return _node_id;     }
  const NodeID_t&     Particle::Parent     () const { return _parent_id;   }
  const NodeID_t&     Particle::Ancestor   () const { return _ancestor_id; }
  const Generation_t& Particle::Generation () const { return _generation;  }
  float Particle::RelationshipScore(const NodeID_t id)
  {
    if(id >= _score_v.size()) return -1;
    return _score_v[id];
  }
  
  const RecoType_t& Particle::RecoType() const { return _reco_type; }
  const RecoID_t&  Particle:: RecoID()   const { return _reco_id;   }

  const int&    Particle::PdgCode() const
  { return _pdg_code; }
  const double& Particle::Mass() const
  { return _mass; }

  double Particle::Energy() const
  { return  pow((_mom * _mom + _mass * _mass), 0.5); }

  double Particle::KineticEnergy() const
  { return pow((_mom * _mom + _mass * _mass), 0.5) - _mass ; }

  const ::geoalgo::Vector& Particle::Vertex()   const
  { return _vtx; }
  const ::geoalgo::Vector& Particle::Momentum() const
  { return _mom; }
  float Particle::RecoScore()
  {
    return RelationshipScore(_node_id); 
  }
  
  const std::vector< ::ertool::NodeID_t >& Particle::Children() const
  { return _child_v; }
  
  bool Particle::RelationAssessed() const
  { return _generation != kDefaultGeneration; }
  
  bool Particle::Primary() const
  { return _primary; }

  bool Particle::Descendant() const
  { return (!(_node_id == _ancestor_id && RelationAssessed())); }
  
  bool Particle::Lonely() const
  { return (_child_v.size()<1); }

  bool Particle::HasChild(const NodeID_t id)
  {
    if( id > _child_v.back()  ) return false;
    if( id < _child_v.front() ) return false;
    auto low = std::lower_bound(_child_v.begin(), _child_v.end(), id);
    return ((*low) == id);
  }
  
  bool Particle::HasRecoObject()
  { return (_reco_type == kShower || _reco_type == kTrack); }

 
  void Particle::SetRecoInfo(const RecoType_t reco_type, const RecoID_t reco_id )
  {
    if( reco_type == kINVALID_RECO_TYPE )
      throw ERException("RecoType_t cannot be kINVALID_RECO_TYPE!");
    
    if( reco_type == kInvisible && reco_id != kINVALID_RECO_ID )
      throw ERException("Invisible type particle cannot be associated with reco object!");

    if( reco_type != kInvisible && reco_id == kINVALID_RECO_ID )
      throw ERException("Shower/Track particle must be associated with a valid RecoID_t!");

    _reco_type = reco_type;
    _reco_id   = reco_id;
  }

  void Particle::SetParticleInfo( const int pdg_code,
				  const double mass,
				  const ::geoalgo::Vector& vtx,
				  const ::geoalgo::Vector& mom,
				  const float score)
  {
    _vtx.compat(vtx);
    _mom.compat(mom);
    _pdg_code  = pdg_code;
    _mass      = mass;
    _vtx       = vtx;
    _mom       = mom;
    SetScore(_node_id,score);
  }

  void Particle::SetScore(const NodeID_t id, const float score)
  {
    if(id == kINVALID_NODE_ID) throw ERException("Cannot set a score for an invalid particle ID...");
    if(_score_v.size() <= id) _score_v.resize(id+1,-1);
    _score_v[id] = score;
  }
  
  void Particle::AddChild(const NodeID_t id, const float score)
  {
    SetScore(id,score);
    if( _child_v.empty() || _child_v.back() < id) {
      _child_v.push_back(id);
      return;
    }
    auto low = std::lower_bound(_child_v.begin(),_child_v.end(),id);
    if((*low) == id) return;

    size_t low_index = low - _child_v.begin();
    _child_v.resize(_child_v.size()+1);
    for(size_t index = _child_v.size()-1; index > low_index; --index)
      _child_v[index] = _child_v[index-1];
    _child_v[low_index] = id;
  }

  void Particle::RemoveChild(const NodeID_t child)
  {
    auto low = std::lower_bound(_child_v.begin(),_child_v.end(),child);

    if( (*low) != child ) return;

    _child_v.erase(low);
  }

  void Particle::UpdateAfterRemoval(const NodeID_t removed)
  {
    if( _node_id == removed )
      throw ERException("Not yet removed!");

    if( _node_id     > removed ) --_node_id;
    if( _parent_id   > removed ) --_parent_id;
    if( _ancestor_id > removed ) --_ancestor_id;

    for(auto& child_id : _child_v) {

      if( child_id > removed ) --child_id;

    }
    for(size_t index = removed; (index+1) < _score_v.size(); ++index)
      _score_v[index] = _score_v[index+1];

    if(_score_v.size())
       _score_v.erase(--_score_v.end());
  }



  void Particle::Print(std::string& res, std::string prefix) const
  {
    res+="Printing members of particle \n";
    if (PdgCode() == kINVALID_INT)
      res += prefix + "unknown" + " ... ";
    else
      res += "Pdg " + prefix + std::to_string(PdgCode()) + " ... \n";
    if (Vertex() == kINVALID_VERTEX)
      res += "[unknown] [cm]";
    else
      res += "Vtx " + std::to_string(Vertex()[0]) + " [cm] : " + std::to_string(Vertex()[1]) + " [cm] : " + std::to_string(Vertex()[2]) + " [cm] \n";
    if (Momentum() == kINVALID_MOMENTUM)
      res += "--> unknown" ;
    else
      res += "Kinetic Energy " + std::to_string( KineticEnergy() );
    res += " MeV \n";

    prefix += "  ";

  }

  std::string Particle::Print() const
  {
    std::string res;
    Print(res," ");
    return res;    
  }

}


#endif
