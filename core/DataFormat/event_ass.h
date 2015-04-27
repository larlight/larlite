/**
 * \file event_ass.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def for event-wise association class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_EVENT_ASS_H
#define LARLITE_EVENT_ASS_H

#include "data_base.h"
#include "larlite_dataformat_utils.h"
#include <TString.h>
namespace larlite{

  /**
     \class event_ass
     A base coass for event-wise data holder class
  */
  class event_ass : public event_base {

  private:
    
    std::map<larlite::product_id,std::map<larlite::product_id,size_t> > _ass_map_key;
    std::vector<larlite::AssSet_t> _ass_data;

  public:
    
    /// Default constructor
    event_ass(const std::string name = "noname");
    /// Default destructor
    virtual ~event_ass(){}
    /// Clear method
    virtual void clear_data();

    //
    // Setter functions
    //
    /// Association setter
    void set_association(const product_id& id_a,
			 const product_id& id_b,
			 const AssSet_t& ass_a2b,
			 const bool overwrite=true);

    //
    // Info getter functions
    //
    /// Query the total number of association IDs stored
    size_t size() const;

    /// Query the number of associated objects for data product with larlite::product_id == id_a
    size_t size_association(const product_id& id_a,
			    const product_id& id_b);

    /**
       Query the key for AssSet_t object that maps association of id_a => id_b. \n
       Note one is encouraged to use this method to query the key only once in  \n
       the event loop
    */
    AssID_t association_id(const product_id& id_a,
			   const product_id& id_b) const;

    /**
       Query the key for AssSet_t object that maps association of id_a => one of \n
       the type specified in "type" argument. If you have more than one association \n
       of the specified type, then the first in the list is returned. Strictly speaking\n
       you do not have a control on the "which one". But in return you do not have to bother\n
       knowing the product id of the associated object. For instance, if you know there is \n
       only one association between a cluster=>hit, you may use data::kHit type to get this \n
       association. That way you do not have to have a user input the hit producer label.\n
       Obviously this is a dirty method but who doesn't like a dirty method? 
     */
    AssID_t association_id(const product_id& id_a,
			   const unsigned short type) const;
    
    /// larlite::AssSet_t getter given an explicit larlite::product_id combination
    const AssSet_t& association(const product_id& id_a,
				const product_id& id_b) const;

    /// Getter for associated data products' key info (product_id)
    const std::vector<std::pair<larlite::product_id,larlite::product_id> > association_keys() const;

    /// Getter for associated data products' key info (product_id)
    const std::vector<larlite::product_id> association_keys(const larlite::product_id& id) const;

    /// List association
    void list_association() const;

  protected:
    /// larlite::AssSet_t getter given an id
    const AssSet_t& association(const AssID_t id) const;

  };
}
#endif
/** @} */ // end of doxygen group 
