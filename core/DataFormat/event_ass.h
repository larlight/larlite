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
    std::vector<std::pair<larlite::product_id,larlite::product_id> > _ass_ids;
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

    /// Getter for associated data products' key info (product_id)
    const std::vector<std::pair<larlite::product_id,larlite::product_id> > association_keys() const;

    /// Getter for associated data products' key info (product_id)
    const std::vector<larlite::product_id> association_keys(const larlite::product_id& id) const;

    /// Getter for associated data products' key info (product_id)
    const std::pair<larlite::product_id,larlite::product_id>& association_keys(const AssID_t id) const;

    /// List association
    void list_association() const;

    /**
       larlite::AssSet_t getter given an explicit larlite::product_id combination. \n
       larlite::DataFormatException thrown if not found.
    */
    const AssSet_t& association(const product_id& id_a,
				const product_id& id_b) const;
    /**
       larlite::AssSet_t getter given an explicit larlite::product_id combination. \n
       larlite::DataFormatException thrown if not found.
    */
    const AssSet_t& association(const AssID_t id) const;

    /**
       Query the key for AssSet_t object that maps association of id_a => id_b. \n
       If the return is kINVALID_ASS, then there is no proper association found. \n
    */
    AssID_t assid(const product_id& id_a, const product_id& id_b) const;

    AssID_t find_one_assid(const data::DataType_t type_a,
			   const data::DataType_t type_b) const;
    
    AssID_t find_one_assid(const product_id& id_a,
			   const data::DataType_t type_b) const;
    
    AssID_t find_one_assid(const data::DataType_t type_a,
			   const product_id& id_b) const;

    AssID_t find_unique_assid(const data::DataType_t type_a,
			      const data::DataType_t type_b) const;
    
    AssID_t find_unique_assid(const product_id& id_a,
			      const data::DataType_t type_b) const;
    
    AssID_t find_unique_assid(const data::DataType_t type_a,
			      const product_id& id_b) const;

    std::vector<AssID_t> find_all_assid(const data::DataType_t type_a,
					const data::DataType_t type_b) const;
    
    std::vector<AssID_t> find_all_assid(const product_id& id_a,
					const data::DataType_t type_b) const;
    
    std::vector<AssID_t> find_all_assid(const data::DataType_t type_a,
					const product_id& id_b) const;
    
  };
}
#endif
/** @} */ // end of doxygen group 
