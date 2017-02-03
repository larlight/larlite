/**
 * \file mceventweight.h
 *
 * \brief This data product stores the weights applied to MC events
 *
 * @author A. Mastbaum, mastbaum@uchicago.edu
*/

#ifndef LARLITE_MCEVENTWEIGHT_H
#define LARLITE_MCEVENTWEIGHT_H

#include "TTree.h"
#include "Base/MCConstants.h"
#include <vector>
#include <utility>
#include "data_base.h"

namespace larlite {

  class mceventweight : public data_base {

  public:

    mceventweight() : data_base(data::kMCEventWeight) {
      clear_data();
    }

    /// Default destructor
    virtual ~mceventweight() {}

    mceventweight(const std::map<std::string, std::vector<double> > weights);

    void clear_data();

    // Setters
    void SetWeights(const std::map<std::string, std::vector<double> > weights);

    // Getters
    std::map<std::string, std::vector<double> > GetWeights() const;

  private:

    /**
     * The event weights.
     *
     * There are several reweighting functions, which are identified by a
     * string name. For some of them, you run lots of trials (universes).
     * This is a mapping from the name to a vector full of the weights
     * obtained in the trials.
     */
    std::map<std::string, std::vector<double> > _weights;

  };

  /**
   * \class event_mceventweight
   *
   * A collection storage class of multiple mceventweights.
   */
  class event_mceventweight
      : public std::vector<larlite::mceventweight>, public event_base {

  public:

    /// Default constructor
    event_mceventweight(std::string name="noname")
        : event_base(data::kMCEventWeight, name) {
      clear_data();
    }

    /// Default copy constructor
    event_mceventweight(const event_mceventweight& original)
        : std::vector<larlite::mceventweight>(original), event_base(original) {}

    /// Default destructor
    ~event_mceventweight() {}

    /// Method to clear currently held data contents in the buffer
    virtual void clear_data() {
      event_base::clear_data();
      clear();
    }

  };

}

#endif  // LARLITE_MCEVENTWEIGHT_H

