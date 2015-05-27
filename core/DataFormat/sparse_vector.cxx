#ifndef LARLITE_SPARSE_VECTOR_CXX
#define LARLITE_SPARSE_VECTOR_CXX

#include "sparse_vector.h"
#include "DataFormatException.h"

namespace larlite{

  //
  // sparse_vector
  //
  
  template <class T>
  void sparse_vector<T>::add_range(unsigned int start, const std::vector<T>& data)
  {
    if(!data.size())
      throw DataFormatException("<<sparse_vector>>::add_range Cannot add 0-length data!");
    
    if(_ranges.empty()) {
      _ranges.emplace_back(start,data);
      return;
    }
    
    auto& last = _ranges.back();
    if(last < start) {
      if( last.size() > (start - last.begin_index()) )
	throw DataFormatException("<<sparse_vector>>::add_range Cannot add an overlapping range!");
      // Append if possible
      if( (last.begin_index() + last.size()) == start ){
	last.reserve(last.size()+data.size());
	for(auto const& v : data)
	  last.push_back(v);
      }else
	_ranges.emplace_back(start,data);
      return;
    }
    
    auto low = std::lower_bound(_ranges.begin(),_ranges.end(),start);
    
    if( data.size() > ((*low).begin_index() - start) )
      throw DataFormatException("<<sparse_vector>>::add_range Cannot add an overlapping range!");
    
    auto pos = low - _ranges.begin();
    
    if( pos && ( _ranges[pos-1].size() > (start - _ranges[pos-1].begin_index()) ))
      throw DataFormatException("<<sparse_vector>>::add_range Cannot add an overlapping range!");

    // Prepend if possible
    if( (start + data.size()) == (*low).begin_index() ) {
      sparse_data<T> tmp;
      tmp.reserve(data.size()+(*low).size());
      for(auto const& v : data   ) tmp.push_back(v);
      for(auto const& v : (*low) ) tmp.push_back(v);
      _ranges[pos] = tmp;
    }
    // Append if possible
    else if( pos && ( (_ranges[pos-1].begin_index() + _ranges[pos-1].size()) == start ) ){
      auto& sub = _ranges[pos-1];
      sub.reserve(sub.size() + data.size());
      for(auto const& v : data) sub.push_back(v);
    }else
      _ranges.insert(low,sparse_data<T>(start,data));
  }

  template class sparse_vector<float>;
}


#endif
