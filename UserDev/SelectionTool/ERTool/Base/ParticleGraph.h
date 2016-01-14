/**
 * \file ParticleGraph.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class ParticleGraph
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_PARTICLEGRAPH_H
#define ERTOOL_PARTICLEGRAPH_H

#include "Particle.h"
#include "RecoObjBase.h"
#include <deque>
namespace ertool {
  class Manager;
  namespace io {
    class IOHandler;
    class EmptyInput;
  }
}

namespace ertool {

  class ParticleGraph {
    friend class Manager;
    friend class io::IOHandler;
    friend class io::EmptyInput;
  public:
    
    ParticleGraph() : _particle_v() {}

    Particle& CreateParticle();
    Particle& GetParticle(const NodeID_t id);
    const Particle& GetParticle(const NodeID_t id) const;
    const std::deque< ::ertool::Particle >& GetParticleArray() const;

    size_t GetNumParticles () const;
    size_t GetNumPrimaries () const;
    const std::vector<NodeID_t> GetAllDescendantNodes(const NodeID_t target) const;
    const std::vector<NodeID_t> GetSiblingNodes(const NodeID_t target) const;
    const std::vector<NodeID_t> GetParticleNodes ( const RecoType_t type      = kINVALID_RECO_TYPE,
						   const bool unassessed_only = false,
						   const int pdg_code = 0 ) const;
    const std::vector<NodeID_t> GetPrimaryNodes ( const RecoType_t type = kINVALID_RECO_TYPE,
						  const bool unassessed_only = false,
						  const int pdg_code = 0) const;
    const std::vector<NodeID_t> GetBaseNodes ( const RecoType_t type = kINVALID_RECO_TYPE,
					       const bool unassessed_only = false,
					       const int pdg_code = 0) const;
    const CombinationSet_t GetNodeCombinations( const size_t combination_size,
						const RecoType_t type = kINVALID_RECO_TYPE,
						const bool unassessed_only = false,
						const int pdg_code = 0) const;
    void SetFlashID      ( const NodeID_t  node,
			   const FlashID_t flash);
    void SetParentage    ( const NodeID_t parent,
			   const NodeID_t child,
			   const float score = kDEFAULT_SCORE);
    void SetSiblings     ( const NodeID_t his_id,
			   const NodeID_t her_id,
			   const float score = kDEFAULT_SCORE);
    void SetPrimary      ( const NodeID_t node );
    void SameParticle    ( const NodeID_t a,      const NodeID_t b      );


    std::string Diagram() const;
    std::string Diagram(const NodeID_t id) const;

  private:
    Particle& CreateParticle (const RecoObjBase& reco_obj);
    void ValidNode           (const NodeID_t& id         ) const;
    void UpdateGeneration    (const NodeID_t target, const Generation_t gen );
    void UpdateFlashID       (const NodeID_t target, const FlashID_t flash  );
    void UpdateParentID      (const NodeID_t target, const NodeID_t parent  );
    void UpdateAncestorID    (const NodeID_t target, const NodeID_t ancestor);
    void UpdateRecoInfo      (const NodeID_t target, const RecoType_t reco_type, const RecoID_t reco_id );    
    void AddChild            (const NodeID_t target, const NodeID_t child, const float score = kDEFAULT_SCORE);
    void Diagram             (const NodeID_t target, std::string& res, std::string prefix="") const;
    void Reset();

    std::deque< ::ertool::Particle > _particle_v;
    
  };

}

#endif
/** @} */ // end of doxygen group 

