#include "Analysis-TypeDef.hh"
#include "CMTool-TypeDef.hh"
#include "TSystem.h"

int main(int argc, char** argv) {

  larlight::ana_processor mgr;

  mgr.add_input_file(argv[1]);
    
  mgr.set_input_rootdir("scanner");

  mgr.set_output_file("");

  mgr.set_io_mode(larlight::storage_manager::READ);
  
  mgr.set_ana_output_file("");
  
  larlight::ClusterMatcher* proc = new larlight::ClusterMatcher;;

  proc->GetManager().AddMatchAlgo(new cmtool::CFAlgoTimeProf);

  proc->SetClusterType(larlight::DATA::RyanCluster);

  //proc->GetManager().DebugMode(cmtool::CMergeManager::kPerIteration);
  //proc.GetManager().DebugMode(cmtool::CMergeManager::kPerAlgoSet);
  proc->GetManager().DebugMode(cmtool::CMergeManager::kPerMerging);

  mgr.add_process(proc);

  mgr.run(0,5000);

  return 1;
}



