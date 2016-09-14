#ifndef OPT0FINDER_BASEALGORITHM_CXX
#define OPT0FINDER_BASEALGORITHM_CXX

#include "BaseAlgorithm.h"
#include "OpT0FinderException.h"
namespace flashana {

  BaseAlgorithm::BaseAlgorithm(const Algorithm_t type,const std::string name)
    : _type(type)
    , _name(name)
    , _active_volume( 0.0,    -116.5,  0.0,
		      256.35,  116.5,  1036.8 )
    , _drift_velocity(0.1114359)
  {}

  Algorithm_t BaseAlgorithm::AlgorithmType() const
  { return _type; }

  void BaseAlgorithm::Configure(const Config_t &pset)
  {
    this->set_verbosity((msg::Level_t)(pset.get<unsigned int>("Verbosity",(unsigned int)(msg::kNORMAL))));
    this->_Configure_(pset);
  }

  size_t BaseAlgorithm::NOpDets() const
  {
    return _opdet_x_v.size();
  }

  const std::string& BaseAlgorithm::AlgorithmName() const
  { return _name; }

  double BaseAlgorithm::DriftVelocity() const
  { return _drift_velocity; }

  double BaseAlgorithm::OpDetX(size_t i) const
  { return _opdet_x_v.at(i); }

  double BaseAlgorithm::OpDetY(size_t i) const
  { return _opdet_y_v.at(i); }

  double BaseAlgorithm::OpDetZ(size_t i) const
  { return _opdet_z_v.at(i); }

  double BaseAlgorithm::ActiveXMax() const
  { return _active_volume.Max()[0]; }
  double BaseAlgorithm::ActiveYMax() const
  { return _active_volume.Max()[1]; }
  double BaseAlgorithm::ActiveZMax() const
  { return _active_volume.Max()[2]; }

  double BaseAlgorithm::ActiveXMin() const
  { return _active_volume.Min()[0]; }
  double BaseAlgorithm::ActiveYMin() const
  { return _active_volume.Min()[1]; }
  double BaseAlgorithm::ActiveZMin() const
  { return _active_volume.Min()[2]; }

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
  
  void BaseAlgorithm::SetActiveVolume( const double xmin, const double xmax,
				       const double ymin, const double ymax,
				       const double zmin, const double zmax )
  {
    _active_volume.Min(xmin,ymin,zmin);
    _active_volume.Max(xmax,ymax,zmax);
  }

  void BaseAlgorithm::SetDriftVelocity( const double v )
  { _drift_velocity = v; }
  
}

#endif
