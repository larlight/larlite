/**
 * \file DrawRawDigit.h
 *
 * \ingroup EventViewer
 * 
 * \brief Class def header for a class DrawRawDigit
 *
 * @author cadams
 */

/** \addtogroup EventViewer

    @{*/

#ifndef LARLITE_DRAWRAWDIGIT_H
#define LARLITE_DRAWRAWDIGIT_H

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


namespace evd {
  /**
     \class DrawRawDigit
     User custom analysis class made by SHELL_USER_NAME
   */
  class DrawRawDigit : public larlite::ana_base, public RawBase{
  
  public:

    /// Default constructor
    DrawRawDigit();

    /// Default destructor
    virtual ~DrawRawDigit(){}

    /** IMPLEMENT in DrawRawDigit.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in DrawRawDigit.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlite::storage_manager* storage);

    /** IMPLEMENT in DrawRawDigit.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetCorrectData(bool _doit = true){_correct_data = _doit;}
    void SetSaveData(bool _doit = true){_save_data = _doit;}

    void SetStepSizeByPlane(int stepSize, int plane);

  private:

    // This function corrects the data by removing correlated noise
    // It can also save out the information
    void correctData();

    float getCorrelation(const std::vector<float> &, const std::vector<float> &);
    float getMedian( std::vector<float> & vals);

    bool _correct_data;
    bool _save_data;


    std::vector<std::vector<float> > pedestalByPlane;
    std::vector<std::vector<float> > rmsByPlane;
    std::vector<std::vector<float> > rmsByPlaneCorrected;

    std::vector<std::vector<int  > > badWireMapByPlane;

    int run, subrun, event;

    std::vector<int> stepSize;

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
