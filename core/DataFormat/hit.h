/**
 * \file hit.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for hit data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_HIT_H
#define LARLITE_HIT_H

#include "data_base.h"
#include <vector>
namespace larlite{
  /**
     \class hit
     LArSoft Hit class equivalent data container
  */
  class hit : public data_base {
    
  public:
    
    /// Default constructor
    hit();
    
    /// Default destructor
    virtual ~hit(){}

    hit(const hit &original) : data_base(original),
			       fHitSignal(original.fHitSignal),
			       fStartTime(original.fStartTime),
			       fSigmaStartTime(original.fSigmaStartTime),
			       fPeakTime(original.fPeakTime),
			       fSigmaPeakTime(original.fSigmaPeakTime),
			       fEndTime(original.fEndTime),
			       fSigmaEndTime(original.fSigmaEndTime),
			       fCharge(original.fCharge),
			       fSigmaCharge(original.fSigmaCharge),
			       fMaxCharge(original.fMaxCharge),
			       fSigmaMaxCharge(original.fSigmaMaxCharge),
                               fMultiplicity(original.fMultiplicity),
                               fGoodnessOfFit(original.fGoodnessOfFit),
                               fView(original.fView),
                               fSignalType(original.fSignalType),
                               fChannel(original.fChannel),
                               fWire(original.fWire)
    {};
  

    void clear_data();
    
    // Set methods
    void set_waveform  (const std::vector<double>& wf) { fHitSignal = wf;}
    void set_times     (double start, double peak, double end)
    {fStartTime = start; fPeakTime = peak; fEndTime = end;}
    void set_times_err (double start, double peak, double end)
    {fSigmaStartTime = start; fSigmaPeakTime = peak; fSigmaEndTime = end;}
    void set_charge    (double q, double q_max)
    {fCharge=q; fMaxCharge = q_max;}
    void set_charge_err(double q, double q_max)
    {fSigmaCharge=q; fSigmaMaxCharge=q_max;}
    void set_multiplicity(int m){ fMultiplicity=m;}
    void set_channel (unsigned int ch){ fChannel=ch;}
    void set_wire    (unsigned int wire) { fWire=wire;}
    void set_fit_goodness(double v){ fGoodnessOfFit = v;}
    void set_view(geo::View_t v){ fView = v;}
    void set_sigtype(geo::SigType_t t) {fSignalType=t;}
    
    // Get Methods 
    const std::vector<double>& HitSignal()    const { return fHitSignal;      }  
    double        StartTime()                 const { return fStartTime;      }  
    double        EndTime()                   const { return fEndTime;        }
    double        PeakTime()                  const { return fPeakTime;       }
    double        SigmaStartTime()            const { return fSigmaStartTime; }
    double        SigmaEndTime()              const { return fSigmaEndTime;   }
    double        SigmaPeakTime()             const { return fSigmaPeakTime;  }
    int           Multiplicity()              const { return fMultiplicity;   }
    unsigned int          Channel()                   const { return fChannel;        }
    unsigned int          Wire()                      const { return fWire;           }
    double        Charge(bool max=false)      const { return ( max ? fMaxCharge : fCharge);}
    double        SigmaCharge(bool max=false) const { return ( max ? fSigmaMaxCharge : fSigmaCharge ); }
    double        GoodnessOfFit()             const { return fGoodnessOfFit;  }
    geo::SigType_t  SignalType()                const { return fSignalType;     }
    geo::View_t     View()                      const { return fView;           }
    
  protected:
    
    std::vector<double>   fHitSignal;      ///< vector of ADC values within the hit window
    double                fStartTime;      ///< initial tdc tick for hit 
    double                fSigmaStartTime; ///< uncertainty on initial tick
    double                fPeakTime;       ///< tdc for the peak charge deposition
    double                fSigmaPeakTime;  ///< uncertainty for tdc of the peak
    double                fEndTime;        ///< final tdc tick for hit
    double                fSigmaEndTime;   ///< uncertainty on final tick
    double                fCharge;         ///< total charge deposited for hit
    double                fSigmaCharge;    ///< uncertainty in total charge deposited
    double                fMaxCharge;      ///< maximum ADC value in hit window
    double                fSigmaMaxCharge; ///< maximum ADC value in hit window
    int                   fMultiplicity;   ///< how many hits could this one be shared with
    double                fGoodnessOfFit;  ///< how well do we believe we know this hit?
    geo::View_t             fView;           ///< view for the plane of the hit
    geo::SigType_t          fSignalType;     ///< signal type for the plane of the hit
    unsigned int                  fChannel;        ///< channel number
    unsigned int                  fWire;           ///< wire number

  private:
    
    ////////////////////////
    ClassDef(hit,3)
    ////////////////////////
      
  };
  
  /**
     \class event_hit
     A collection storage class of multiple hits.
  */
  class event_hit : public std::vector<larlite::hit>, 
		    public event_base {
    
  public:
    
    /// Default constructor
    event_hit(std::string name="noname") : event_base(data::kHit,name) { clear_data(); }
    
    /// Default copy constructor
    event_hit(const event_hit& original) : std::vector<larlite::hit>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_hit(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
    ////////////////////////
    ClassDef(event_hit,4)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
