//
// Example C++ routine to run instantiate your sample class
//

#include "DataFormat/storage_manager.h"
#include "FhiclLite/FhiclLiteUtilFunc.h"
#include "LEEPreCuts/LEEPreCut.h"
#include <iostream>

int main(int argc, char** argv){

  std::string cfg = argv[1];
  std::string input_filename = argv[2];

  fcllite::PSet ps_root = fcllite::CreatePSetFromFile( cfg );
  fcllite::PSet ps = ps_root.get<fcllite::PSet>("LEEPreCut");

  larlite::LEEPreCut algo;
  algo.configure( ps );
  
  larlite::storage_manager* man = new larlite::storage_manager( larlite::storage_manager::kREAD );
  man->add_in_filename( input_filename );
  man->set_data_to_read( larlite::data::kOpHit, ps.get<std::string>("OpHitProducer") );
  man->open();

  int nevents = man->get_entries();
  for ( int ientry=0; ientry<100; ientry++ ) {
    man->go_to( ientry );
    bool pass = algo.analyze( man );

    std::cout << "Run " << man->run_id() << ", Subrun " << man->subrun_id() << ", Event " << man->event_id() << ": passes precuts=" << pass << std::endl;
  }

  return 0;
}
