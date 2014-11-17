import ROOT,sys
from ROOT import *
from ROOT import larlite as fmwk

mgr = fmwk.ana_processor()

#arg1 should be input file name
mgr.add_input_file(sys.argv[1])

#arg2 should be output file name
mgr.set_output_file(sys.argv[2])

mgr.set_io_mode(fmwk.storage_manager.kBOTH)

mgr.set_ana_output_file("")

proc = fmwk.ClusterMerger()

#secondmerger starts with Cluster data type (not Fuzzy!)
#and outputs Cluster... it overwrites now
proc.SetClusterProducer("mergedfuzzycluster")
proc.SaveOutputCluster(True)


########################################
# attach merge algos here
########################################

########################################
# Remove tracks with priority algo!
########################################
proc.GetManager().AddPriorityAlgo(cmtool.CPAlgoIgnoreTracks())



########################################
# PROHIBIT ALGORITHMS
########################################
prohib_array = cmtool.CBAlgoArray()

tracksep_prohibit = cmtool.CBAlgoTrackSeparate()
tracksep_prohibit.SetDebug(False)
tracksep_prohibit.SetVerbose(False)
tracksep_prohibit.SetUseEP(True)
prohib_array.AddAlgo(tracksep_prohibit,False)

outofcone_prohibit = cmtool.CBAlgoOutOfConeSeparate()
outofcone_prohibit.SetDebug(False)
outofcone_prohibit.SetVerbose(False)
outofcone_prohibit.SetMaxAngleSep(20.)
prohib_array.AddAlgo(outofcone_prohibit,False)

angle_prohibit = cmtool.CBAlgoAngleIncompat()
#this only applies if both clusters have >50 hits
angle_prohibit.SetMinHits(50)
angle_prohibit.SetAllow180Ambig(True)
angle_prohibit.SetUseOpeningAngle(False)
#this prohibits clusters w/ angles different than 10 degrees
angle_prohibit.SetAngleCut(10.)
angle_prohibit.SetMinLength(20.)
angle_prohibit.SetDebug(False)
prohib_array.AddAlgo(angle_prohibit,False)

proc.GetManager().AddSeparateAlgo(prohib_array)

########################################
# MERGE ALGORITHMS
########################################
algo_array = cmtool.CBAlgoArray()

COM_algo = cmtool.CBAlgoCenterOfMass()
COM_algo.SetDebug(False)
COM_algo.SetVerbose(False)
COM_algo.UseCOMInPoly(True)
COM_algo.UseCOMInCone(True)
COM_algo.UseCOMNearClus(True)
COM_algo.SetLengthReach(3.)
algo_array.AddAlgo(COM_algo,False)

overlapalg = cmtool.CBAlgoPolyOverlap()
overlapalg.SetMinNumHits(0)
algo_array.AddAlgo(overlapalg,False)

polyshortalg_bigclusters = cmtool.CBAlgoPolyShortestDist()
#this one is for big-ish clusters
polyshortalg_bigclusters.SetMinNumHits(30)
polyshortalg_bigclusters.SetMaxNumHits(9999)
#two polygons that have points SUPER close have distsquared of ~0.5
polyshortalg_bigclusters.SetMinDistSquared(1.)
polyshortalg_bigclusters.SetDebug(False)
algo_array.AddAlgo(polyshortalg_bigclusters,False)

proc.GetManager().AddMergeAlgo(algo_array)

########################################
# done attaching merge algos
########################################

########################################
# CMergeManager options
########################################
proc.GetManager().MergeTillConverge(True)

mgr.add_process(proc)

mgr.run()


