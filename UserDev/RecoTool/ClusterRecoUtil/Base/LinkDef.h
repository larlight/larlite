//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class Polygon2D+;

#pragma link C++ namespace cluster+;


#pragma link C++ class std::vector<larutil::PxHit>+;
#pragma link C++ class std::vector<const larutil::PxHit*>+;

#pragma link C++ class cluster::ClusterParamsAlg+;
#pragma link C++ class std::vector<::cluster::ClusterParamsAlg>+;

#pragma link C++ class cluster::cluster_params+;
#pragma link C++ class std::vector<cluster::cluster_params>+;

#pragma link C++ class cluster::CRUException+;
#pragma link C++ class cluster::CRUHelper+;


//#pragma link C++ class cluster::DefaultParamsAlg+;
//ADD_NEW_CLASS ... do not change this line
#endif



















