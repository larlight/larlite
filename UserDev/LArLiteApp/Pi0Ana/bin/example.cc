//
// Example C++ routine to run your analysis module, Pi0Ana.
//

#include "Analysis/Analysis-TypeDef.h"
#include "Pi0Ana/Pi0Ana-TypeDef.h"

int main(int argc, char** argv){

  if(argc<2) {

    std::cerr
      << std::endl
      << Form("Usage 1: %s $INPUT_ROOT_FILE",argv[0]) << std::endl
      << Form("Usage 2: %s $INPUT_ROOT_FILE $TDIRECTORY_NAME",argv[0]) << std::endl
      << std::endl;

    return 1;
  }

  std::cout
    << std::endl
    << "This is \"Analysis\" package simple test routine." << std::endl
    << std::endl;

  // Create ana_processor instance
  larlite::ana_processor my_proc;

  // Set input root file
  my_proc.add_input_file(argv[1]);

  // Specify IO mode
  my_proc.set_io_mode(larlite::storage_manager::kREAD);

  // Specify input TDirectory name if given
  if(argc>2)
    my_proc.set_input_rootdir(argv[2]);

  // Specify output root file name
  my_proc.set_ana_output_file("from_test_ana_you_can_remove_me.root");

  // Attach a template process
  my_proc.add_process(new larlite::sample_ana);

  std::cout
    << std::endl
    << "Finished configuring ana_processor. Start event loop!"<< std::endl
    << std::endl;

  // Let's run it.
  my_proc.run();

  // done!
  std::cout
    << std::endl
    << "Finished running ana_processor event loop!" << std::endl
    << std::endl;
  return 0;
}
