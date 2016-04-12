#ifndef LARLITE_SPARSE_VECTOR_H
#define LARLITE_SPARSE_VECTOR_H

#include <vector>
#include <algorithm>

namespace larlite {

  template <class T>
  class sparse_data : public std::vector<T> {
  protected:
    unsigned int   _start_tick;

  public:

    sparse_data(unsigned int start_tick=0,
		unsigned int data_size=0)
      : std::vector<T>(data_size)
      , _start_tick(start_tick)
    {}

    sparse_data(unsigned int start_tick,
		const std::vector<T>& data)
      : std::vector<T>(data)
      , _start_tick(start_tick)
    {}

    sparse_data(const sparse_data& orig)
      : std::vector<T>(orig)
      , _start_tick(orig._start_tick)
    {}

    unsigned int begin_index() const { return _start_tick; }
    
    inline bool operator< (const sparse_data& rhs) const
    { return _start_tick < rhs.begin_index(); }

    inline bool operator< (const unsigned int tick) const
    { return _start_tick < tick; }

    const std::vector<float>& data() const { return (*this); }

  };

  template <class T>
  class sparse_vector {
  public:

    sparse_vector() : _ranges()
    {}

    sparse_vector(const sparse_vector& orig)
      : _ranges(orig._ranges)
    {}

    const std::vector<larlite::sparse_data<T> >& get_ranges() const
    { return _ranges; }

    size_t size() const { return _ranges.size(); }

    void add_range(unsigned int start, const std::vector<T>& data);

  protected:
    std::vector<larlite::sparse_data<T> > _ranges;

  };
}

#endif
