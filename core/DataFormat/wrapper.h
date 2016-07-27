/**
 * \file wrapper.h
 *
 * \ingroup DataFormat
 *
 * \brief Wraps data stored in ROOT
 *
 * @author W. David Dagenhart - Fermilab 2015
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_core_DataFormat_wrapper_H
#define LARLITE_core_DataFormat_wrapper_H

#include "data_base.h"
#include "data_type.h"

#include <map>
#include <string>
#include <vector>
#include <utility>

namespace larlite {

  // Function overloads for the clear function. For vectors
  // and maps, this calls the clear function. Other classes
  // are initialized with the default constructor, and
  // fundamental types are zero initialized.
  template <typename S>
  void
  clear(S & s) { s = S{}; }

  template <typename S>
  void
  clear(std::vector<S> & s) { s.clear(); }

  template <typename U, typename W>
  void
  clear(std::map<U, W> & m) { m.clear(); }

  // -------------------------------------------------------

  template <typename T>
  class wrapper : public event_base {
  public:

    wrapper(std::string name="noname") :
      event_base(larlite::data_type<wrapper<T> >(), name), obj() { }

    virtual ~wrapper() { }

    T* product();
    T* operator->();

    T const* product() const;
    T const* operator->() const;

    void set(T t) { obj = std::move(t); }

    void clear_data() {
      event_base::clear_data();
     larlite::clear(obj);
    }

  private:
    T obj;
  };

  template <typename T>
  T* wrapper<T>::product() {
    return &obj;
  }

  template <typename T>
  T* wrapper<T>::operator->() {
    return product();
  }

  template <typename T>
  T const* wrapper<T>::product() const {
    return &obj;
  }

  template <typename T>
  T const* wrapper<T>::operator->() const {
    return product();
  }
}
#endif
