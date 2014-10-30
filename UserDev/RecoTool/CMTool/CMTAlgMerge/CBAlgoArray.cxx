#ifndef RECOTOOL_CBALGOARRAY_CXX
#define RECOTOOL_CBALGOARRAY_CXX

#include "CBAlgoArray.h"

namespace cmtool {

  //------------------------------------------
  CBAlgoArray::CBAlgoArray() : CBoolAlgoBase()
  //------------------------------------------
  {
    _algo_array.clear();
    _ask_and.clear();
  }

  //-----------------------
  void CBAlgoArray::Reset()
  //-----------------------
  {
    for(auto &algo : _algo_array) algo->Reset();
  }

  //-------------------------------------------------------------------------------------
  void CBAlgoArray::EventBegin(const std::vector<cluster::ClusterParamsAlg> &clusters)
  //-------------------------------------------------------------------------------------
  {
    for(auto &algo : _algo_array) algo->EventBegin(clusters);
  }

  //--------------------------
  void CBAlgoArray::EventEnd()
  //--------------------------
  {
    for(auto &algo : _algo_array) algo->EventEnd();
  }

  //-------------------------------------------------------------------------------------
  void CBAlgoArray::IterationBegin(const std::vector<cluster::ClusterParamsAlg> &clusters)
  //-------------------------------------------------------------------------------------
  {
    for(auto &algo : _algo_array) algo->IterationBegin(clusters);
  }

  //--------------------------
  void CBAlgoArray::IterationEnd()
  //--------------------------
  {
    for(auto &algo : _algo_array) algo->IterationEnd();
  }

  //--------------------------------------------------------------------
  bool CBAlgoArray::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			 const ::cluster::ClusterParamsAlg &cluster2)
  //--------------------------------------------------------------------
  {
    bool status = true;
    
    for(size_t i=0; i<_algo_array.size(); ++i) {

      if(!i) status = _algo_array.at(i)->Bool(cluster1,cluster2);

      else {

	//
	// Break before executing algo if possible
	//

	// Case 1: if AND and status==false, then break
	if(  _ask_and.at(i) && !status ) break;

	// Case 2: if OR and status==true, then break
	if( !_ask_and.at(i) &&  status ) break;

	// Case 3: the remaining algorithms are all OR condition and stauts==true
	if( i > _last_and_algo_index && status) break;

	//
	// Execute algorithm
	//
	if( _ask_and.at(i) ) 

	  status = status && _algo_array.at(i)->Bool(cluster1,cluster2);

	else 
	  
	  status = status || _algo_array.at(i)->Bool(cluster1,cluster2);
	
      }
    }

    return status;
  }

  //------------------------
  void CBAlgoArray::Report()
  //------------------------
  {
    for(auto &algo : _algo_array) algo->Report();
  }
    
}
#endif
