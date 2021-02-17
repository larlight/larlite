#ifndef BASICTOOL_PYGEOOBJ_CXX
#define BASICTOOL_PYGEOOBJ_CXX

#include "PyGeoObj.h"

namespace larpy {

  PyObject* PyGeoObj::Convert(const larlite::geoalgo::Point_t& pt       ) const
  {
    PyObject* pvec = PyList_New(pt.size());

    for(size_t i=0; i<pt.size(); ++i) {

      if(PyList_SetItem(pvec, i, PyFloat_FromDouble(pt[i]))) {

	Py_DECREF(pvec);
	throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
      }
    }
    return pvec;
  }

  /*
  PyObject* PyGeoObj::Convert(const std::string& col  ) const
  {
    PyObject* pvec = PyList_New(col.size());

    for(size_t i=0; i<col.size(); ++i) {

      if( PyList_SetItem(pvec, i, col[i]) ) {

	Py_DECREF(pvec);
	throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
      }
    }
    return pvec;
  }
  */
  PyObject* PyGeoObj::Convert(const larlite::geoalgo::LineSegment_t& pt ) const
  {
    PyObject* pres = PyList_New(3);

    PyObject* x = PyList_New(2);
    if( PyList_SetItem(x,0,PyFloat_FromDouble(pt.Start()[0])) ||
	PyList_SetItem(x,1,PyFloat_FromDouble(pt.End()[0]  )) ||
	PyList_SetItem(pres,0,x) ) {
      Py_DECREF(pres);
      Py_DECREF(x);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }

    PyObject* y = PyList_New(2);
    if( PyList_SetItem(y,0,PyFloat_FromDouble(pt.Start()[1])) ||
	PyList_SetItem(y,1,PyFloat_FromDouble(pt.End()[1]  )) ||
	PyList_SetItem(pres,1,y) ) {
      Py_DECREF(pres);
      Py_DECREF(y);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }

    PyObject* z = PyList_New(2);
    if( PyList_SetItem(z,0,PyFloat_FromDouble(pt.Start()[2])) || 
	PyList_SetItem(z,1,PyFloat_FromDouble(pt.End()[2]  )) ||
	PyList_SetItem(pres,2,z) ) {
      Py_DECREF(pres);
      Py_DECREF(z);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }
    
    return pres;
  }


  PyObject* PyGeoObj::Convert(const larlite::geoalgo::HalfLine_t& pt ) const
  {
    PyObject* pres = PyList_New(3);

    PyObject* x = PyList_New(2);
    if( PyList_SetItem(x,0,PyFloat_FromDouble(pt.Start()[0])) ||
	PyList_SetItem(x,1,PyFloat_FromDouble(pt.Start()[0] + (pt.Dir()[0]*70)  )) ||
	PyList_SetItem(pres,0,x) ) {
      Py_DECREF(pres);
      Py_DECREF(x);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }

    PyObject* y = PyList_New(2);
    if( PyList_SetItem(y,0,PyFloat_FromDouble(pt.Start()[1])) ||
	PyList_SetItem(y,1,PyFloat_FromDouble(pt.Start()[1] + (pt.Dir()[1]*70)  )) ||
	PyList_SetItem(pres,1,y) ) {
      Py_DECREF(pres);
      Py_DECREF(y);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }

    PyObject* z = PyList_New(2);
    if( PyList_SetItem(z,0,PyFloat_FromDouble(pt.Start()[2])) || 
	PyList_SetItem(z,1,PyFloat_FromDouble(pt.Start()[2] + (pt.Dir()[2]*70)  )) ||
	PyList_SetItem(pres,2,z) ) {
      Py_DECREF(pres);
      Py_DECREF(z);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }
    
    return pres;
  }


  PyObject* PyGeoObj::Convert(const larlite::geoalgo::AABox_t& box) const
  {

    double x_min = box.Min()[0];
    double y_min = box.Min()[1];
    double z_min = box.Min()[2];
    double x_max = box.Max()[0];
    double y_max = box.Max()[1];
    double z_max = box.Max()[2];

    std::vector< larlite::geoalgo::LineSegment_t> lines;
    lines.reserve(12);

    lines.push_back(larlite::geoalgo::LineSegment_t(x_min,y_min,z_min, x_min,y_min,z_max));
    lines.push_back(larlite::geoalgo::LineSegment_t(x_min,y_min,z_min, x_min,y_max,z_min));
    lines.push_back(larlite::geoalgo::LineSegment_t(x_min,y_min,z_min, x_max,y_min,z_min));

    lines.push_back(larlite::geoalgo::LineSegment_t(x_min,y_min,z_max, x_max,y_min,z_max));
    lines.push_back(larlite::geoalgo::LineSegment_t(x_min,y_min,z_max, x_min,y_max,z_max));
    
    lines.push_back(larlite::geoalgo::LineSegment_t(x_max,y_min,z_min, x_max,y_min,z_max));
    lines.push_back(larlite::geoalgo::LineSegment_t(x_max,y_min,z_min, x_max,y_max,z_min));

    lines.push_back(larlite::geoalgo::LineSegment_t(x_min,y_max,z_min, x_min,y_max,z_max));
    lines.push_back(larlite::geoalgo::LineSegment_t(x_min,y_max,z_min, x_max,y_max,z_min));

    lines.push_back(larlite::geoalgo::LineSegment_t(x_max,y_max,z_max, x_min,y_max,z_max));
    lines.push_back(larlite::geoalgo::LineSegment_t(x_max,y_max,z_max, x_max,y_min,z_max));
    lines.push_back(larlite::geoalgo::LineSegment_t(x_max,y_max,z_max, x_max,y_max,z_min));

    PyObject* pres = PyTuple_New(lines.size());
    bool status = true;
    for(size_t i=0; i<lines.size(); ++i) {

      auto const& l = lines[i];

      PyObject* pyline = PyTuple_New(3);
      for(size_t j=0; j<3; ++j) {

	PyObject* pypt = PyTuple_New(2);
	if( PyTuple_SetItem(pypt,0,PyFloat_FromDouble(l.Start()[j])) ||
	    PyTuple_SetItem(pypt,1,PyFloat_FromDouble(l.End()[j])) ) {
	  Py_DECREF(pypt);
	  status=false;
	  break;
	}
	if(PyTuple_SetItem(pyline,j,pypt)) {
	  status=false;
	  break;
	}
      }
      if(!status) { Py_DECREF(pyline); break; }
      
      if(PyTuple_SetItem(pres,i,pyline)) { status = false; break; }
    }
    if(!status) {
      Py_DECREF(pres);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }
    
    return pres;
    /*
    // Polygon along z//z
    std::vector<std::pair<double,double> > poly_pt_v;
    poly_pt_v.reserve(6);
    if( zaxis == "z") {
      poly_pt_v.push_back(std::make_pair(x_min,y_min));
      poly_pt_v.push_back(std::make_pair(x_max,y_min));
      poly_pt_v.push_back(std::make_pair(x_max,y_max));
      poly_pt_v.push_back(std::make_pair(x_min,y_max));
      poly_pt_v.push_back(std::make_pair(x_min,y_min));
      poly_pt_v.push_back(std::make_pair(z_min,z_max));
    }else if( zaxis == "x" ) {
      poly_pt_v.push_back(std::make_pair(y_min,z_min));
      poly_pt_v.push_back(std::make_pair(y_max,z_min));
      poly_pt_v.push_back(std::make_pair(y_max,z_max));
      poly_pt_v.push_back(std::make_pair(y_min,z_max));
      poly_pt_v.push_back(std::make_pair(y_min,z_min));
      poly_pt_v.push_back(std::make_pair(x_min,x_max));
    }else if( zaxis == "y" ) {
      poly_pt_v.push_back(std::make_pair(x_min,z_min));
      poly_pt_v.push_back(std::make_pair(x_max,z_min));
      poly_pt_v.push_back(std::make_pair(x_max,z_max));
      poly_pt_v.push_back(std::make_pair(x_min,z_max));
      poly_pt_v.push_back(std::make_pair(x_min,z_min));
      poly_pt_v.push_back(std::make_pair(y_min,y_max));
    }else
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");

    // Create 5-points to define one plane polygon
    PyObject* poly_xy = PyTuple_New(5);
    bool status = true;
    for(size_t i=0; i<5; ++i) {
      PyObject* poly_tmp = PyTuple_New(2);
      if( PyTuple_SetItem(poly_tmp,0,PyFloat_FromDouble(poly_pt_v[i].first)) ||
	  PyTuple_SetItem(poly_tmp,1,PyFloat_FromDouble(poly_pt_v[i].second)) ) {
	Py_DECREF(poly_tmp);
	status = false;
	break;
      }
      if( PyTuple_SetItem(poly_xy,i,poly_tmp) ) {
	status=false;
	break;
      }
    }
    if(!status) {
      Py_DECREF(poly_xy);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");	
    }

    // Create z-axis point tuple
    PyObject* poly_z = PyTuple_New(2);
    if( PyTuple_SetItem(poly_z,0,PyFloat_FromDouble(poly_pt_v[5].first)) ||
	PyTuple_SetItem(poly_z,1,PyFloat_FromDouble(poly_pt_v[5].second)) ) {
      Py_DECREF(poly_xy);
      Py_DECREF(poly_z);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }

    // Create result
    PyObject* pres = PyTuple_New(2);
    if( PyTuple_SetItem(pres,0,poly_xy) ) {
      Py_DECREF(pres);
      Py_DECREF(poly_z);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }
    if( PyTuple_SetItem(pres,1,poly_z) ) {
      Py_DECREF(pres);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> failed!");
    }
    */
    return pres;
  }

  PyObject* PyGeoObj::Convert(const larlite::geoalgo::Trajectory_t& trj  ) const
  {
    if(trj.size()<2)
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> Can't process trajectory w/ size <2!");

    PyObject* pres = PyList_New(3);
    PyObject* x    = PyList_New(trj.size());
    PyObject* y    = PyList_New(trj.size());
    PyObject* z    = NULL;
    
    bool fill_z = (trj[0].size()>2);
    if(fill_z)
      z = PyList_New(trj.size());
    double tmp_z = 0.;

    for(size_t i=0; i<trj.size(); ++i) {

      auto const& tmp_pt = trj[i];
      if( PyList_SetItem(x,i,PyFloat_FromDouble(tmp_pt[0])) ) {
	Py_DECREF(pres);
	Py_DECREF(y);
	Py_DECREF(z);
	throw larlite::geoalgo::GeoAlgoException("<<Convert>> Failed!");
      }
      if( PyList_SetItem(y,i,PyFloat_FromDouble(tmp_pt[1])) ) {
	Py_DECREF(pres);
	Py_DECREF(x);
	Py_DECREF(z);
	throw larlite::geoalgo::GeoAlgoException("<<Convert>> Failed!");
      }
      if(fill_z) tmp_z = tmp_pt[2];

      if( PyList_SetItem(z,i,PyFloat_FromDouble(tmp_z)) ) {
	Py_DECREF(pres);
	Py_DECREF(x);
	Py_DECREF(y);
	throw larlite::geoalgo::GeoAlgoException("<<Convert>> Failed!");
      }
    }

    if( PyList_SetItem(pres,0,x) ) {
	Py_DECREF(pres);
	Py_DECREF(y);
	Py_DECREF(z);
	throw larlite::geoalgo::GeoAlgoException("<<Convert>> Failed!");
    }

    if( PyList_SetItem(pres,1,y) ) {
	Py_DECREF(pres);
	Py_DECREF(x);
	Py_DECREF(z);
	throw larlite::geoalgo::GeoAlgoException("<<Convert>> Failed!");
    }

    if( PyList_SetItem(pres,2,z) ) {
	Py_DECREF(pres);
	Py_DECREF(x);
	Py_DECREF(y);
	throw larlite::geoalgo::GeoAlgoException("<<Convert>> Failed!");
    }
    
    return pres;
  }

  PyObject* PyGeoObj::Convert(const std::map<larlite::geoalgo::Point_t,std::string>& labels) const
  {
    PyObject* pres  = PyList_New(labels.size());

    size_t ctr=0;
    bool failed = false;
    for(auto const& pt_label : labels) {

      if(pt_label.first.size()!=2 && pt_label.first.size()!=3) {
	failed = true;
	break;
      }

      PyObject* tmp_label = PyList_New(4);
      if( PyList_SetItem(tmp_label,0,PyFloat_FromDouble(pt_label.first[0])) ||
	  PyList_SetItem(tmp_label,1,PyFloat_FromDouble(pt_label.first[1])) ||
	  PyList_SetItem(tmp_label,2,PyFloat_FromDouble(pt_label.first[2])) ||
#ifdef LARLITE_PY3
	  PyList_SetItem(tmp_label,3,PyUnicode_FromString(pt_label.second.c_str()))
#else
	  PyList_SetItem(tmp_label,3,PyString_FromString(pt_label.second.c_str()))	  
#endif
	  ) {

	Py_DECREF(tmp_label);
	failed = true;
	break;
      }else if( PyList_SetItem(pres,ctr,tmp_label) ) {
	failed = true;
	break;
      }
      ++ctr;
    }

    if(failed) {
      Py_DECREF(pres);
      throw larlite::geoalgo::GeoAlgoException("<<Convert>> Failed!");
    }

    return pres;
  }

}
#endif
