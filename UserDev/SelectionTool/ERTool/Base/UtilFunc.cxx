#ifndef ERTOOL_UTILFUNC_CXX
#define ERTOOL_UTILFUNC_CXX

#include "UtilFunc.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include "ERException.h"
#include "Message.h"

namespace ertool {

  double ParticleMass(const int pdgcode)
  {
    switch(pdgcode) {

      // electron
    case  11:
    case -11:
      return 0.510998910;
      // muon
    case  13:
    case -13:
      return 105.65838934;
      // gamma
    case 22:
      return 0.0;
      // neutrinos
    case 12:
    case 14:
      return 0.0;
      // proton
    case  2212:
      return 938.272046;
      // neutron
    case  2112:
      return 939.565378;
      // pi0
    case  111:
      return 134.9766;
      // pi +/-
    case  211:
    case -211:
      return 139.57018;
      // K0
    case  311:
      return 497.648;
      // K +/-
    case  321:
    case -321:
      return 493.667;
      // Delta
    case 2114:
    case 2214:
    case 2224:
    case 1114:
      return 1232.0;
      // rho-0
    case 113:
      return 775.49;
      // rho-+
    case 123:
      return 775.4;
      // phi
    case 333:
      return 1019.445;
      // eta
    case 221:
      return 547.862;
    default:
      std::stringstream msg;
      msg << "PDG Code " << pdgcode << " is not handled yet! Please implement :) ";
      msg::send(msg::kERROR,__FUNCTION__,msg.str());
    }
    return -1;
  }

  std::vector<Combination_t> Combination(const size_t n, 
					 const size_t m)
  {
    
    if(n<m){
      std::ostringstream msg;
      msg << "<<" << __FUNCTION__ << ">>"
	  << " Cannot make a combination of " << m 
	  << " elements from the pool of " << n << std::endl;
      throw ERException(msg.str());      
    }

    std::vector<Combination_t> result;

    std::vector<bool> comb_index_v(n);
    std::fill(comb_index_v.begin() + m, comb_index_v.end(), true);

    do {

      Combination_t index_v;
      index_v.reserve(m);

      for (size_t i = 0; i < comb_index_v.size(); ++i)

	if (!comb_index_v[i]) index_v.push_back(i);
      
      result.push_back(index_v);

    } while (std::next_permutation(comb_index_v.begin(), comb_index_v.end()));

    return result;
  }

  namespace msg {

    void send(Level_t level, const std::string& msg)
    {
      Message::get()->send_msg(level,msg);
    }
    
    void send(Level_t level, const std::string& where, const std::string& msg)
    {
      Message::get()->send_msg(level,where,msg);
    }

  }
}

#endif
