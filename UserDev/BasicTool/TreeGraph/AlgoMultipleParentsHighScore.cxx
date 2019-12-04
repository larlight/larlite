#ifndef ALGOMULTIPLEPARENTSHIGHSCORE_CXX
#define ALGOMULTIPLEPARENTSHIGHSCORE_CXX

#include "AlgoMultipleParentsHighScore.h"

namespace geotree{

  AlgoMultipleParentsHighScore::AlgoMultipleParentsHighScore(NodeCollection* coll)
  {
    _coll = coll;
    _name = "MultipleParentsHighScore";
  }

  void AlgoMultipleParentsHighScore::FindBestParent(const NodeID_t& id, const std::vector<NodeID_t>& parents)
  {

    // since we are starting fresh, clear correlations currently stored
    ClearCorrelations();

    // find which node in the list of parents has the highest score. Remove all other correlations
    double highScore = 0;
    NodeID_t bestParent = -1;
    for (size_t n=0; n < parents.size(); n++){
      // geto score
      double thisScore = _coll->GetNode(id).getScore(parents[n]);
      if (_verbose) { std::cout << "\tID: " << id << "\tparent: " << parents[n] << "\tScore: " << thisScore << std::endl; }
      if (thisScore > highScore){
	highScore = thisScore;
	bestParent = parents[n];
      }// if temporary best parent
    }// for all parents

    if (_verbose) { std::cout << "Best Parent: " << bestParent << std::endl; }
    // we have the best parent
    // now edit correlations appropriately
    // i.e. remove all correlations with nodes
    // that are not best parent
    std::pair<NodeID_t,NodeID_t> nodePair;
    for (size_t n=0; n < parents.size(); n++){
      if (parents[n] != bestParent){
	// make node-pair
	nodePair = std::make_pair(id,parents[n]);
	// make correlation
	// give a score of -1 so that we know to remove this correlation
	Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
	_corr_v[nodePair] = corr;
      }// if not best parent
    }// for all parents

    return;
  }

}

#endif
