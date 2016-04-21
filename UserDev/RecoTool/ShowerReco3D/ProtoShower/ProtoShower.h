#ifndef PROTOSHOWER_H
#define PROTOSHOWER_H

#include <TVector3.h>
#include <vector>
#include "LArUtil/PxUtils.h"
#include "Base/GeoTypes.h"
#include "ClusterRecoUtil/Base/ClusterParams.h"
#include "Cluster3DRecoUtil/Base/Cluster3DParams.h"

namespace protoshower {


class ProtoShower
{
  friend class ProtoShowerHelper;

public:
  ProtoShower() {};
  ~ProtoShower() {};

  const std::vector<cluster::cluster_params> & params() const {return _params;}
  const cluster3D::cluster3D_params & params3D() const {return _params3D;}
  const std::vector<TVector3> & vertexes() const {return _vertexes;}

  // getters
  bool hasCluster2D() const {return _hasCluster2D;}
  bool hasCluster3D() const {return _hasCluster3D;}
  bool hasVertex()    const {return _hasVertex;}
  // setters
  void hasCluster2D(bool on) { _hasCluster2D = on; }
  void hasCluster3D(bool on) { _hasCluster3D = on; }
  void hasVertex   (bool on) { _hasVertex    = on; }

  // This is the set of 2D cluster params
  std::vector<cluster::cluster_params> _params;
  // This is the 3D cluster params, just one set of spacepoints
  cluster3D::cluster3D_params _params3D;
  // list of 3D vertices associated to this ProtoShower
  std::vector<TVector3>  _vertexes;

protected:

  bool _hasCluster2D;
  bool _hasCluster3D;
  bool _hasVertex;



  // Not sure what to do with vertexes yet

};

}

#endif