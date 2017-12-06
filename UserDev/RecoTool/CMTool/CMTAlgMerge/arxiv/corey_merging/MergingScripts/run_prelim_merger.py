import ROOT,sys
ROOT.gSystem.Load("libClusterRecoUtil")
from ROOT import *
from ROOT import larlight as fmwk

mgr = fmwk.ana_processor()

#arg1 should be input file name
mgr.add_input_file(sys.argv[1])

mgr.set_input_rootdir("scanner")
mgr.set_output_rootdir("scanner")
#arg2 should be output file name
mgr.set_output_file(sys.argv[2])

mgr.set_io_mode(fmwk.storage_manager.BOTH)

mgr.set_ana_output_file("")

proc = fmwk.ClusterMerger()

#set the input cluster type... prelim merger starts with 
#FuzzyCluster and outputs Cluster
proc.SetClusterType(larlight.DATA.FuzzyCluster)
proc.SaveOutputCluster(True)


########################################
# attach merge algos here
########################################

## PROHIBIT ALGOS ##
prohib_array = cmtool.CBAlgoArray()

tracksep_prohibit = cmtool.CBAlgoTrackSeparate()
tracksep_prohibit.SetUseEP(True)
prohib_array.AddAlgo(tracksep_prohibit,False)

outofcone_prohibit = cmtool.CBAlgoOutOfConeSeparate()
outofcone_prohibit.SetMaxAngleSep(20.)
prohib_array.AddAlgo(outofcone_prohibit,False)

proc.GetManager().AddSeparateAlgo(prohib_array)

## MERGE ALGOS ##
merge_array = cmtool.CBAlgoArray()

shortdist_alg = cmtool.CBAlgoShortestDist()
shortdist_alg.SetMinHits(10)
shortdist_alg.SetSquaredDistanceCut(5.)
merge_array.AddAlgo(shortdist_alg,False)

trackblob = cmtool.CBAlgoStartTrack()
merge_array.AddAlgo(trackblob,False)

pcontain = cmtool.CBAlgoPolyContain()
merge_array.AddAlgo(pcontain,False)

proc.GetManager().AddMergeAlgo(merge_array)

########################################
# done attaching merge algos
########################################

########################################
# cmerge manager parameters
########################################
proc.GetManager().MergeTillConverge(True)


mgr.add_process(proc)

mgr.run()


