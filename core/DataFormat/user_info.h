/**
 * \file user_info.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for user_info 
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef LARLITE_USER_INFO_H
#define LARLITE_USER_INFO_H

#include "data_base.h"
#include <map>
#include <TString.h>

namespace larlite {
  /**
     \class user_info
     A class to hold user defined variables on the fly.
     What we mean by "on the fly"? It means you can define variable name
     and store it in the data w/o defining a dedicated data container class.
     
     The supported variable type for storage includes double, int, std::string, bool, and vector of those types.
     The variable is stored as a pair of a specific std::string key and value, both provided by a user.
     The string is then used to retrieve a variable later.
     Note that each retrieval requires std::string comparison, hence is somewhat expensive (i.e. takes time).
     But it is a matter of micro-seconds for even very large map (i.e. large map = you store a lot of variables).
     
     See user_collection class which is actually used in event loop.
  */
  class user_info : public data_base {
    
  public:
    
    /** Method to dump all key & value contents in the storage map on the screen
	This is useful to check what's in the data interactively.
    */
    void dump_contents();
    
    /// Default constructor
    user_info();
    
    /// setter for a single double variable
    inline void store(std::string key, double value)      {_d_map[key]=value;};
    /// setter for a single int variable
    inline void store(std::string key, int value)         {_i_map[key]=value;};
    /// setter for a single std::string variable
    inline void store(std::string key, std::string value) {_s_map[key]=value;};
    /// setter for a single bool variable
    inline void store(std::string key, bool value)        {_b_map[key]=value;};
    
    /// method to add double value to a double array
    void append(std::string key, double value);      
    /// method to add int value to an int array
    void append(std::string key, int value);   
    /// method to add std::string value to an std::string array      
    void append(std::string key, std::string value); 
    /// method to add bool value to an bool array
    void append(std::string key, bool value);        
    
    /// checker for an existence of a double variable key
    inline bool exist_double (std::string key) const { return _d_map.find(key)!=_d_map.end(); };
    /// checker for an existence of a string variable key
    inline bool exist_string (std::string key) const { return _s_map.find(key)!=_s_map.end(); };
    /// checker for an existence of a int variable key
    inline bool exist_int    (std::string key) const { return _i_map.find(key)!=_i_map.end(); };
    /// checker for an existence of a bool variable key
    inline bool exist_bool   (std::string key) const { return _b_map.find(key)!=_b_map.end(); };
    /// checker for an existence of a double array
    inline bool exist_darray (std::string key) const { return _darray_map.find(key)!=_darray_map.end(); };
    /// checker for an existence of a string array
    inline bool exist_sarray (std::string key) const { return _sarray_map.find(key)!=_sarray_map.end(); };
    /// checker for an existence of a int array
    inline bool exist_iarray (std::string key) const { return _iarray_map.find(key)!=_iarray_map.end(); };
    /// checker for an existence of a bool array
    inline bool exist_barray (std::string key) const { return _barray_map.find(key)!=_barray_map.end(); };
    
    /// getter for a single double variable
    double      get_double (std::string key) const;  
    /// getter for a single int variable
    int         get_int    (std::string key) const;  
    /// getter for a single std::string variable
    std::string get_string (std::string key) const;  
    /// getter for a single bool variable
    bool        get_bool   (std::string key) const;  
    
    /// getter for a double array
    std::vector<double>*      get_darray (std::string key); 
    /// getter for an int array
    std::vector<int>*         get_iarray (std::string key); 
    /// getter for a std::string array
    std::vector<std::string>* get_sarray (std::string key); 
    /// getter for a bool array
    std::vector<bool>*        get_barray (std::string key); 
    
    /// Default destructor
    virtual ~user_info(){}; 
    
    /// Clear method override
    virtual void clear_data();
    
  protected:
    
    std::map<std::string,double>       _d_map; ///< var. holder map of double
    std::map<std::string,int>          _i_map; ///< var. holder map of int
    std::map<std::string,std::string>  _s_map; ///< var. holder map of std::string
    std::map<std::string,bool>         _b_map; ///< var. holder map of bool
    std::map<std::string,std::vector<double> >      _darray_map; ///< var. holder map of double array
    std::map<std::string,std::vector<int> >         _iarray_map; ///< var. holder map of int array
    std::map<std::string,std::vector<std::string> > _sarray_map; ///< var. holder map of std::string array
    std::map<std::string,std::vector<bool> >        _barray_map; ///< var. holder map of bool array
    
  };
  


  /**
     \class event_user
     A collection storage class of multiple user_info.
     This is what created per-event if user specified to store user's information in an event loop.
     If a user just needs one user_info, it may be an array of length 1.
  */
  class event_user : public std::vector<larlite::user_info>,
		     public event_base {
    
  public:
    
    /// Default constructor
    event_user(std::string name="noname");
    
    /// Default copy constructor
    event_user(const event_user& original) : std::vector<larlite::user_info>(original),
					     event_base(original)
    {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
    /// Default destructor
    ~event_user(){}
    
  private:
    
  };
  
}
  
#endif
/** @} */ // end of doxygen group 
