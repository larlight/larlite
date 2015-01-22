#ifndef FUZZYCLUSTERSHOWER_CXX
#define FUZZYCLUSTERSHOWER_CXX

#include "FuzzyClusterShower.h"

namespace larlite {

  FuzzyClusterShower::FuzzyClusterShower() 
    : ClusterMergerArray() 
  { 
    _name="FuzzyClusterShower"; 

    //
    // Specific configuration for FuzzyClusterShower
    //
    // 0) configure I/O
    // 1) configure 1st stage merger
    // 2) configure 2nd stage merger
    //

    //
    // I/O configuration
    //
    SaveOutputCluster(true);
    SetInputProducer("fuzzycluster");
    SetOutputProducer("mergedfuzzy");
    
    //
    // Configure 1st stage merging
    //
    auto& merger_1st = GetManager(0);
    merger_1st.MergeTillConverge(true);
    //merger_1st.DebugMode(::cmtool::CMergeManager::kPerIteration);

    // Prohibit algorithms    
    auto prohib_algo_1_1 = new ::cmtool::CBAlgoTrackSeparate;
    prohib_algo_1_1->SetUseEP(true);

    auto prohib_algo_1_2 = new ::cmtool::CBAlgoOutOfConeSeparate;
    prohib_algo_1_2->SetMaxAngleSep(20.);

    auto prohib_algo_1 = new ::cmtool::CBAlgoArray;
    prohib_algo_1->AddAlgo(prohib_algo_1_1,false);
    prohib_algo_1->AddAlgo(prohib_algo_1_2,false);

    merger_1st.AddSeparateAlgo( prohib_algo_1 );

    // Merge algorithms
    auto merge_algo_1_1 = new ::cmtool::CBAlgoShortestDist;
    merge_algo_1_1->SetMinHits(10);
    merge_algo_1_1->SetSquaredDistanceCut(5.);

    auto merge_algo_1_2 = new ::cmtool::CBAlgoStartTrack;

    auto merge_algo_1_3 = new ::cmtool::CBAlgoPolyContain;

    auto merge_algo_1 = new ::cmtool::CBAlgoArray;
    merge_algo_1->AddAlgo(merge_algo_1_1,false);
    merge_algo_1->AddAlgo(merge_algo_1_2,false);
    merge_algo_1->AddAlgo(merge_algo_1_3,false);

    merger_1st.AddMergeAlgo( merge_algo_1 );

    // Configure 2nd stage merging
    auto& merger_2nd = GetManager(1);
    merger_2nd.MergeTillConverge(true);
    //merger_1st.DebugMode(::cmtool::CMergeManager::kPerIteration);
    
    // Prohibit algorithms
    auto prohib_algo_2_1 = new ::cmtool::CBAlgoTrackSeparate;
    prohib_algo_2_1->SetDebug(false);
    prohib_algo_2_1->SetVerbose(false);
    prohib_algo_2_1->SetUseEP(true);

    auto prohib_algo_2_2 = new ::cmtool::CBAlgoOutOfConeSeparate;
    prohib_algo_2_2->SetDebug(false);
    prohib_algo_2_2->SetVerbose(false);
    prohib_algo_2_2->SetMaxAngleSep(20.);

    auto prohib_algo_2_3 = new ::cmtool::CBAlgoAngleIncompat;
    prohib_algo_2_3->SetMinHits(50);
    prohib_algo_2_3->SetAllow180Ambig(true);
    prohib_algo_2_3->SetUseOpeningAngle(false);
    prohib_algo_2_3->SetAngleCut(10.);
    prohib_algo_2_3->SetMinLength(20.);
    prohib_algo_2_3->SetDebug(false);

    auto prohib_algo_2 = new ::cmtool::CBAlgoArray;
    prohib_algo_2->AddAlgo(prohib_algo_2_1,false);
    prohib_algo_2->AddAlgo(prohib_algo_2_2,false);
    prohib_algo_2->AddAlgo(prohib_algo_2_3,false);

    merger_2nd.AddSeparateAlgo(prohib_algo_2);

    // Merge algorithms
    auto merge_algo_2_1 = new ::cmtool::CBAlgoCenterOfMass;
    merge_algo_2_1->SetDebug(false);
    merge_algo_2_1->SetVerbose(false);
    merge_algo_2_1->UseCOMInPoly(true);
    merge_algo_2_1->UseCOMInCone(true);
    merge_algo_2_1->UseCOMNearClus(true);
    merge_algo_2_1->SetLengthReach(3.);

    auto merge_algo_2_2 = new ::cmtool::CBAlgoPolyOverlap;
    merge_algo_2_2->SetMinNumHits(0);

    auto merge_algo_2_3 = new ::cmtool::CBAlgoPolyShortestDist;
    merge_algo_2_3->SetMinNumHits(30);
    merge_algo_2_3->SetMaxNumHits(9999);
    merge_algo_2_3->SetMinDistSquared(1.);
    merge_algo_2_3->SetDebug(false);

    auto merge_algo_2 = new ::cmtool::CBAlgoArray;
    merge_algo_2->AddAlgo(merge_algo_2_1,false);
    merge_algo_2->AddAlgo(merge_algo_2_2,false);
    merge_algo_2->AddAlgo(merge_algo_2_3,false);
    
    merger_2nd.AddMergeAlgo(merge_algo_2);

    // Prohibit algorithms
    auto priority_algo_2 = new ::cmtool::CPAlgoIgnoreTracks;

    merger_2nd.AddPriorityAlgo(priority_algo_2);
    
    // Done configuring default set of algorithms
  }

  bool FuzzyClusterShower::initialize() {

    return ClusterMergerArray::initialize();

  }
  
  bool FuzzyClusterShower::analyze(storage_manager* storage) {
  
    return ClusterMergerArray::analyze(storage);
    
  }

  bool FuzzyClusterShower::finalize() {

    return ClusterMergerArray::finalize();
  }

}
#endif
