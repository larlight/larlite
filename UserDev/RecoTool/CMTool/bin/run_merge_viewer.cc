#include <TStyle.h>
#include "LArUtil-TypeDef.hh"
#include "Analysis-TypeDef.hh"
#include "ClusterRecoUtil-TypeDef.hh"
#include "CMTool-TypeDef.hh"
#include <iostream>

int main(int argc, char** argv) {

  larlight::ana_processor ana_proc;
  
  ana_proc.set_io_mode(larlight::storage_manager::READ);
  
  ana_proc.add_input_file(argv[1]);

  larlight::storage_manager::get()->set_in_rootdir("scanner");

  ana_proc.set_ana_output_file("");
  
  auto raw_viewer   = new larlight::ClusterViewer();

  auto merge_viewer = new larlight::MergeViewer();

  auto mc_viewer    = new larlight::MCShowerClusterViewer();

  merge_viewer->GetManager().AddMergeAlgo(new cmtool::CBAlgoFake());

  ana_proc.add_process(raw_viewer);

  ana_proc.add_process(merge_viewer);

  ana_proc.add_process(mc_viewer);

  gStyle->SetOptStat(0);

  size_t ctr=0;

  while(1) {
    
    ana_proc.process_event(ctr);
    
    raw_viewer->DrawAllClusters();
    
    merge_viewer->DrawAllClusters();
    
    mc_viewer->DrawAllClusters();
    
    //std::string input;
    //std::cin>>input;
    
    ctr++;
  }

  return 1;
}


