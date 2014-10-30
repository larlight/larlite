import ROOT,sys
ROOT.gSystem.Load("libClusterRecoUtil")
from ROOT import *
from ROOT import larlight as fmwk

mgr = fmwk.ana_processor()

mgr.add_input_file(sys.argv[1])

mgr.set_input_rootdir("scanner")

mgr.set_output_file("out.root")

mgr.set_io_mode(fmwk.storage_manager.BOTH)

mgr.set_ana_output_file("")

proc = fmwk.ClusterMerger()

proc.SaveOutputCluster(True)

proc.GetManager().AddMergeAlgo(cmtool.CBAlgoFake())

proc.GetManager().AddSeparateAlgo(cmtool.CBAlgoFake())

#proc.GetManager().DebugMode(cmtool.CMergeManager.kPerIteration)
#proc.GetManager().DebugMode(cmtool.CMergeManager.kPerAlgoSet)
#proc.GetManager().DebugMode(cmtool.CMergeManager.kPerMerging)

#mgr.set_verbosity(fmwk.MSG.DEBUG)

mgr.add_process(proc)

mgr.run(0,50)


