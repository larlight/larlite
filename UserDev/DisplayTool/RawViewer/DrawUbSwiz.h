/**
 * \file DrawUbSwiz.h
 *
 * \ingroup EventViewer
 * 
 * \brief Class def header for a class DrawUbSwiz
 *
 * @author cadams
 */

/** \addtogroup EventViewer

    @{*/

#ifndef LARLITE_DRAWUBSWIZ_H
#define LARLITE_DRAWUBSWIZ_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "RawBase.h"

#include "TTree.h"
#include "TGraph.h"

struct _object;
typedef _object PyObject;

#ifndef __CINT__
#include "Python.h"
#include "numpy/arrayobject.h"
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#endif

#include "DataFetcher.h"


namespace evd {
  /**
     \class DrawUbSwiz
     User custom analysis class made by SHELL_USER_NAME
   */
  class DrawUbSwiz :  public RawBase{
  
  public:

    /// Default constructor
    DrawUbSwiz();

    /// Default destructor
    virtual ~DrawUbSwiz(){}

    /** IMPLEMENT in DrawUbSwiz.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in DrawUbSwiz.cc! 
        Analyze a data event-by-event  
    */
    // virtual bool analyze(larlite::storage_manager* storage);

    /** IMPLEMENT in DrawUbSwiz.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetCorrectData(bool _doit = true){_correct_data = _doit;}
    void SetSaveData(bool _doit = true){_save_data = _doit;}

    void SetStepSizeByPlane(int stepSize, int plane);

    void nextEvent();
    void prevEvent();
    void goToEvent(size_t e);

    void setInput(std::string s);

    unsigned int run(){return _run;}
    unsigned int event_no(){return _event_no;}
    unsigned int subrun(){return _subrun;}
    int current_event() const{return _current_event;}
    int n_events() const{return _n_events;}

  private:

    void readData();

    size_t _n_events;
    // this is the event in the file (0 -> n_events)
    size_t _current_event;
    // this is the official event:
    size_t _event_no;
    size_t _run;
    size_t _spill;
    size_t _subrun;

    // This function corrects the data by removing correlated noise
    // It can also save out the information
    void correctData();

    float getCorrelation(const std::vector<float> &, const std::vector<float> &);
    float getMedian( std::vector<float> & vals);

    bool _correct_data;
    bool _save_data;

    std::string _input_file;

    std::vector<std::vector<float> > pedestalByPlane;
    std::vector<std::vector<float> > rmsByPlane;
    std::vector<std::vector<float> > rmsByPlaneCorrected;

    std::vector<std::vector<int  > > badWireMapByPlane;

    // int run, subrun, event;

    std::vector<int> stepSize;

    DataFetcher  df;

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
