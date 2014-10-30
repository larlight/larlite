import ROOT,sys
ROOT.gSystem.Load("libCMTool")
from ROOT import *
from ROOT import larlight as fmwk

mgr = fmwk.ana_processor()

mgr.add_input_file(sys.argv[1])

mgr.set_input_rootdir("scanner")

mgr.set_output_file("")

mgr.set_io_mode(fmwk.storage_manager.READ)

mgr.set_ana_output_file("")

proc = fmwk.ClusterMatcher()

#priority_algo = cmtool.CPAlgoPolyArea()
#priority_algo.SetMinArea(1e9)

#proc.GetManager().AddPriorityAlgo(priority_algo)

algo = cmtool.CFAlgoTimeProf()

#proc.GetManager().AddMatchAlgo(cmtool.CFAlgoZOverlap())
proc.SetClusterType(fmwk.DATA.RyanCluster)
proc.GetManager().AddMatchAlgo(algo)

#proc.GetManager().DebugMode(cmtool.CMatchManager.kPerIteration)
#proc.GetManager().DebugMode(cmtool.CMatchManager.kPerAlgoSet)
proc.GetManager().DebugMode(cmtool.CMatchManager.kPerMerging)

mgr.add_process(proc)

print "Start..."

ctr=15

while mgr.process_event(ctr):
    ctr+=1
    print 

    bk=proc.GetManager().GetBookKeeper()

    res = bk.GetResult()

    print "Found %d matched pairs ... listing each pair below..." % res.size()

    for x in xrange(res.size()):

        for y in xrange(res.at(x).size()):

            print res.at(x).at(y),
        print

    print "Waiting user hitting an enter..."
    sys.stdin.readline()



