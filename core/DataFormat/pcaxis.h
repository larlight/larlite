////////////////////////////////////////////////////////////////////////////
// \version $Id: PCAxis.h,v 1.0 2010/02/15 20:32:46 usher Exp $
//
// \brief Definition of a Principal Component Axis object for LArSoft
//
// \author usher@slac.stanford.edu
//
////////////////////////////////////////////////////////////////////////////

#ifndef LARLITE_PCAxis_H
#define LARLITE_PCAxis_H

#include <vector>
#include "data_base.h"

namespace larlite
{

  class pcaxis : public data_base
  {
  public:
    
    typedef std::vector<std::vector<double> > EigenVectors;
    
    pcaxis();
    
  private:
    
    bool         fSvdOK;            /// SVD Decomposition was successful
    int          fNumHitsUsed;      /// Number of hits in the decomposition
    double       fEigenValues[3];   /// Eigen values from SVD decomposition
    EigenVectors fEigenVectors;     /// The three principle axes
    double       fAvePosition[3];   /// Average position of hits fed to PCA
    double       fAveHitDoca;       /// Average doca of hits used in PCA
    size_t       fID;               ///< axis ID
    
  public:
    
    pcaxis(bool ok,
	   int nHits,
	   const double* eigenValues,
	   const EigenVectors& eigenVecs,
	   const double* avePos,
	   const double aveHitDoca = 9999.,
	   size_t id=0);

    void clear_data() { data_base::clear_data(); }
    
    inline bool                getSvdOK()        const {return fSvdOK;}
    inline int                 getNumHitsUsed()  const {return fNumHitsUsed;}
    inline const double*       getEigenValues()  const {return fEigenValues;}
    inline const EigenVectors& getEigenVectors() const {return fEigenVectors;}
    inline const double*       getAvePosition()  const {return fAvePosition;}
    inline const double        getAveHitDoca()   const {return fAveHitDoca;}
    inline size_t              getID()           const {return fID;}
    
    //friend std::ostream&  operator << (std::ostream & o, const pcaxis& a);
    friend bool operator < (const pcaxis& a, const pcaxis& b);
    
  };

  /**
     \class event_pcaxis
     A collection storage class of multiple pcaxis
  */
  class event_pcaxis : public std::vector<larlite::pcaxis>,
		       public event_base {
    
  public:
    
    /// Default constructor
    event_pcaxis(std::string name="noname") : event_base(data::kPCAxis,name) {clear_data();}
    
    /// Default copy constructor
    event_pcaxis(const event_pcaxis& original)
      : std::vector<larlite::pcaxis>(original), event_base(original)
    {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
    /// Default destructor
    ~event_pcaxis(){}

  private:
    
  };
  
}

#endif // PCAxis_H
