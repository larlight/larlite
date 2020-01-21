import ROOT,sys
ROOT.gSystem.Load("libClusterRecoUtil")
from ROOT import *
from ROOT import larlight as fmwk

mgr = fmwk.ana_processor()

mgr.add_input_file(sys.argv[1])

mgr.set_input_rootdir("scanner")

mgr.set_output_file("")

mgr.set_io_mode(fmwk.storage_manager.READ)

mgr.set_ana_output_file("")

proc = fmwk.ClusterMerger()

proc.GetManager().AddMergeAlgo(cmtool.CBAlgoFake())

proc.GetManager().AddSeparateAlgo(cmtool.CBAlgoFake())

proc.GetManager().DebugMode(cmtool.CMergeManager.kPerIteration)
#proc.GetManager().DebugMode(cmtool.CMergeManager.kPerAlgoSet)
#proc.GetManager().DebugMode(cmtool.CMergeManager.kPerMerging)

mgr.add_process(proc)

while mgr.process_event():

    print() 
    bk=proc.GetManager().GetBookKeeper()

    res = bk.GetResult()

    for x in range(res.size()):

        for y in range(res.at(x).size()):

            print(res.at(x).at(y), end=' ')
        print()

    sys.stdin.readline()



