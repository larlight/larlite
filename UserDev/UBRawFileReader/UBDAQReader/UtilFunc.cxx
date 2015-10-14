#ifndef UTILFUNC_CXX
#define UTILFUNC_CXX

#include "UtilFunc.h"
#include <unistd.h>
#include <libpq-fe.h>
#include "TString.h"
#include <iostream>
namespace ubdaq {

  unsigned int RollOver(unsigned int ref,
			unsigned int subject,
			unsigned int nbits) 
  {
    // Return "ref" which lower "nbits" are replaced by that of "subject"                                      
    // Takes care of roll over effect.                                                                         
    // For speed purpose we only accept pre-defined nbits values.                                              
  
    unsigned int diff=0; // max diff should be (2^(nbits)-2)/2                                                       
    unsigned int mask=0; // mask to extract lower nbits from subject ... should be 2^(nbits)-1                       
    if      (nbits==3) {diff = 3; mask = 0x7;}
    else if (nbits==4) {diff = 0x7; mask = 0xf;}
    //else if (nbits==4) {diff = 0x7; mask = 0xf;}                                                             
    //    else if (nbits==4) {nbits=3; diff = 0x7; mask = 0x7;}                                                
    else {
      std::cerr<<"\033[93m<<ERROR>>\033[00m"<<" Only supported for nbits = [3,4]!"<<std::endl;
      throw std::exception();
    }
  
    subject = ( (ref>>nbits) << nbits) + (subject & mask);
    //std::cout<<ref<<" : "<<subject<<" : "<<nbits<< " : "<<diff<<std::endl;                                   
    // If exactly same, return                                                                                 
    if(subject == ref) return subject;
  
    // If subject is bigger than ref by a predefined diff value, inspect difference                            
    else if ( subject > ref && (subject - ref) > diff) {
    
      // Throw an exception if difference is exactly diff+1                                                    
      if ( (subject - ref) == diff+1 ) {
	std::cerr<<"\033[93m<<ERROR>>\033[00m"<<Form(" Unexpected diff: ref=%d, subject=%d",ref,subject)<<std::endl;
	throw std::exception();
      }

      // Else we have to subtract (mask+1)                                                                     
      else{
	//std::cout<<Form("Correcting %d to %d",subject,(subject-(mask+1)))<<std::endl;                        
	subject = subject - (mask + 1);
      }
    
    }
    // If subject is smaller than ref by a predefined diff value, inspect difference                           
    else if ( subject < ref && (ref - subject) > diff) {
    
      // Throw an exception if difference is exactly diff+1                                                    
      if ( (ref - subject) == diff+1 ) {
	std::cerr<<"\033[93m<<ERROR>>\033[00m"<<Form(" Unexpected diff: ref=%d, subject=%d",ref,subject)<<std::endl;
	throw std::exception();
      }
      else{
	//std::cout<<Form("Correcting %d to %d",subject,(subject + (mask+1)))<<std::endl;                      
	subject = subject + (mask + 1);
      }
    }
    return subject;
  }


  std::vector<short> decodeChannelTrailer(unsigned short last_adc, unsigned short data)
  {
    // bug fix for missing channel trailer in TPC Data.
    // undoes the hack that fixed the above where the last word is used as the trailer
    // we then use the fake trailer, or frailer, combine it with the last word in the channel data window, or lachadawin, 
    // to recover the end of the channel waveform.

    //std::vector<unsigned short> res;
    std::vector<short> res;
    if(data>>12 == 0x0) {
      //std::cout << "Non-huffman data word..." << std::endl;
      res.push_back(  (short) data & 0xfff);
      return res;
    }
    if(data>>14 == 0x2) {
      //std::cout << "Huffman data word..." << std::endl;
      size_t zero_count=0;
      for(int index=13; index>=0; --index) {
	if(!(data>>index & 0x1)) zero_count +=1;
	else {
	  switch(zero_count){
	          
	  case 0:
	    break;
	  case 1:
	    last_adc -= 1; break;
	  case 2:
	    last_adc += 1; break;
	  case 3:
	    last_adc -= 2; break;
	  case 4:
	    last_adc += 2; break;
	  case 5:
	    last_adc -= 3; break;
	  case 6:
	    last_adc += 3; break;
	  default:

	    std::cerr << "Unexpected 0-count for huffman word: "
		      << "\033[95m"
		      << zero_count << " zeros in the word 0x"
		      << std::hex
		      << data
		      << std::dec
		      << "\033[00m"
		      << std::endl;
	    std::cerr << "Binary representation of the whole word: "
		      << "\033[00m";
	    for(int i=15; i>=0; --i)
	      std::cout << ((data>>i) & 0x1);
	    std::cout << "\033[00m" << std::endl;
	    throw std::exception();
	  }
	  res.push_back((short)last_adc);
	  zero_count = 0;
	}
      }
      return res;
    }

    std::cerr << "\033[93mERROR\033[00m Unexpected upper 4 bit: 0x"
	      << std::hex
	      << ((data >> 12) & 0xf)
	      << std::dec
	      << std::endl;
    throw std::exception();
  }

  void LoadUBChannelMap(UBChannelMap_t& fChannelMap, UBChannelReverseMap_t& fChannelReverseMap ) {

    if ( fChannelMap.size()>0 ) {
      // used prevously grabbed data to avoid repeated call to database.
      return;
    }

    PGconn *conn = PQconnectdb(getenv("SECRET_CONNSTRING"));
    if (PQstatus(conn) != CONNECTION_OK) {
      std::cerr << "Could not connect DB!"<<std::endl;
      throw std::exception();
    }
    
    fChannelMap.clear();
    fChannelReverseMap.clear();

    PGresult *res  = PQexec(conn, "BEGIN");    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) { 
      std::cerr<< "postgresql BEGIN failed";
      PQclear(res);
      PQfinish(conn);
      throw std::exception();
    }

    // Jason St. John's updated call to connections db tables,2015.08.11
    PQclear(res);
    res = PQexec(conn,
		 "SELECT crate_id, daq_slot, fem_channel, wireplane, larsoft_channel "
		 "FROM channels_m24 NATURAL JOIN asics NATURAL JOIN motherboards NATURAL JOIN motherboard_mapping NATURAL JOIN coldcables_v2 NATURAL JOIN intermediateamplifiers_v1 NATURAL JOIN servicecables NATURAL JOIN servicecards NATURAL JOIN warmcables_copy2 NATURAL JOIN adcreceivers_v1 NATURAL JOIN fecards NATURAL JOIN fem_mapping_2015_08_06 NATURAL JOIN fem_map_ranges_v1 NATURAL JOIN fem_crate_ranges NATURAL JOIN fem_slot_ranges_v1 ORDER BY crate_id, daq_slot, fem_channel;"
		 //"FROM channels NATURAL JOIN asics NATURAL JOIN motherboards NATURAL JOIN motherboard_mapping NATURAL JOIN coldcables_v2 NATURAL JOIN intermediateamplifiers_v1 NATURAL JOIN servicecables NATURAL JOIN servicecards NATURAL JOIN warmcables_copy NATURAL JOIN adcreceivers_v1 NATURAL JOIN fecards NATURAL JOIN fem_mapping_2015_08_06 NATURAL JOIN fem_map_ranges_v1 NATURAL JOIN fem_crate_ranges NATURAL JOIN fem_slot_ranges_v1 ORDER BY crate_id, daq_slot, fem_channel;"
		 );

    if ((!res) || (PQresultStatus(res) != PGRES_TUPLES_OK))
      {
	std::cerr<< "SELECT command did not return tuples properly";
	PQclear(res);
	PQfinish(conn);
	throw std::exception();
      }

    int num_records=PQntuples(res);
    for (int i=0;i<num_records;i++) {
      int crate_id     =  atoi(PQgetvalue(res, i, 0));
      int slot         = atoi(PQgetvalue(res, i, 1));
      int boardChan   = atoi(PQgetvalue(res, i, 2));
      //auto const wPl   =      PQgetvalue(res, i, 3);
      int larsoft_chan = atoi(PQgetvalue(res, i, 4));
      
      UBDaqID daq_id(crate_id,slot,boardChan);
      std::pair<UBDaqID, UBLArSoftCh_t> p(daq_id,larsoft_chan);

      if ( fChannelMap.find(daq_id) != fChannelMap.end() ){
	std::cout << __PRETTY_FUNCTION__ << ": ";
	std::cout << "Multiple entries!" << std::endl;
	std::cerr << "Multiple DB entries for same (crate,card,channel). "<<std::endl
		  << "Redefining (crate,card,channel)=>id link ("
		  << daq_id.crate<<", "<< daq_id.card<<", "<< daq_id.channel<<")=>"
		  << fChannelMap.find(daq_id)->second;
      }
      
      fChannelMap.insert( p );
      fChannelReverseMap.insert( std::pair< UBLArSoftCh_t, UBDaqID >( larsoft_chan, daq_id ) );
    }

  }// end of LoadUBChannelMap
}
#endif
