#ifndef OPT0FINDER_BASEALGORITHM_CXX
#define OPT0FINDER_BASEALGORITHM_CXX

#include "BaseAlgorithm.h"
#include "OpT0FinderException.h"
namespace flashana {

  BaseAlgorithm::BaseAlgorithm(const Algorithm_t type,const std::string name)
    : _type(type)
    , _name(name)
  {}

  Algorithm_t BaseAlgorithm::AlgorithmType() const
  { return _type; }

  const std::string& BaseAlgorithm::AlgorithmName() const
  { return _name; }

  double BaseAlgorithm::OpDetX(size_t i) const
  { return _opdet_x_v.at(i); }

  double BaseAlgorithm::OpDetY(size_t i) const
  { return _opdet_y_v.at(i); }

  double BaseAlgorithm::OpDetZ(size_t i) const
  { return _opdet_z_v.at(i); }
    
  const std::vector<double>& BaseAlgorithm::OpDetXArray() const
  { return _opdet_x_v; }
  const std::vector<double>& BaseAlgorithm::OpDetYArray() const
  { return _opdet_y_v; }
  const std::vector<double>& BaseAlgorithm::OpDetZArray() const
  { return _opdet_z_v; }

  void BaseAlgorithm::SetOpDetPositions( const std::vector<double>& pos_x,
					 const std::vector<double>& pos_y,
					 const std::vector<double>& pos_z )
  {
    if(pos_x.size() != pos_y.size() || pos_x.size() != pos_z.size() )
      throw OpT0FinderException("Unmatching optical detector position array length!");
    _opdet_x_v = pos_x;
    _opdet_y_v = pos_y;
    _opdet_z_v = pos_z;

  }
}

#endif
