////////////////////////////////////////////////////////////////////
//
// Definition of track data from minos
//
//  kinga.partyka@yale.edu
//  mitchell.soderberg@yale.edu
//
////////////////////////////////////////////////////////////////////
#include "data_base.h"
#include "Base/GeoConstants.h"
#include <vector>

///ArgoNeuT specific analysis
namespace larlite{
   
   class minos : public data_base {
      
   public:

      
      minos();
      ~minos();
      
      void SetMatched(int matched){fmatched = matched;}
      int matched(){return fmatched;}
         
      int frun;
      int fsubRun;
      int fsnarl;
      double futc;
      double fday;
      float ftrkIndex;
      float ftrkChi2;
      double futc1;
      double fnearns;
      float fnearsec;
      float ftrkE;
      float fshwE;
      double fcrateT0;
      double ftmframe;
      double fyear;
      double foffset; 
      float fdtnear;
      float ftrkErange;
      double fsgate53;
      float ftrkqp;
      float ftrkeqp;
      float ftrkVtxX;
      float ftrkVtxY;
      float ftrkVtxZ;
      double ftrkVtxeX;
      double ftrkVtxeY;
      float fcharge;
      float ftrkmom;
      float ftrkVtxT;
      float ftrkTimeT0;
      float ftrkdcosx;
      float ftrkdcosy;
      float ftrkdcosz;
      double fmonth;
      float ftrtgtd;
      float ftortgt;
      float ftor101;
      float ftr101d;
      int fgoodspill;
      int ftrkcontained;
      int fgoodbeam;
      int fntrkstp;
      std::vector<float> ftrkstpX;
      std::vector<float> ftrkstpY;
      std::vector<float> ftrkstpZ;
      std::vector<float> ftrkstpU;
      std::vector<float> ftrkstpV;
      
      //MC only (try commenting out and remaking all of T962/* if there are problems) .
      float fmcIndex;
      double fmcPDG;
      double fmcPx;
      double fmcPy;
      double fmcPz;
      double fmcEne;
      double fmcMass;
      double fmcVtxX;
      double fmcVtxY;
      double fmcVtxZ;
      friend std::ostream& operator << (std::ostream& o, const minos& m);
       void SetmcPx(double mcpx){fmcPx = mcpx;}
       void SetmcPy(double mcpy){fmcPy = mcpy;}
       void SetmcPz(double mcpz){fmcPz = mcpz;}
   private:    
    
     //double fmcPx;
     //double fmcPy;
     //double fmcPz;
     int fmatched; //our variable to store whether minos track is matched to ArgoNeuT track
   
   };   
 
 /**
     \class event_minos
     A collection storage class of multiple hits.
  */
  class event_minos : public std::vector<larlite::minos>, 
          public event_base {
    
  public:
    
    /// Default constructor
    event_minos(std::string name="noname") : event_base(data::kMinos,name) { clear_data(); }
    
    /// Default copy constructor
    event_minos(const event_minos& original) : std::vector<larlite::minos>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_minos(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
  };

}

////////////////////////////////////////////////////////////////////////
