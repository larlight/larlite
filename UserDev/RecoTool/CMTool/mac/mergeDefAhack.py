
########################################################################
# This module contains the function definitions to guide merging and view
# output at each stage 
########################################################################

from larlite import larlite as fmwk
from recotool.cpp_classes import cmtool
from ROOT import larlite

def getShortestDist(producer='cccluster'):
#
    merger_instance = larlite.ClusterMerger()

    ## PROHIBIT ALGOS ##
    prohib_array = cmtool.CBAlgoArray()
    tracksep_prohibit = cmtool.CBAlgoTrackSeparate()
    tracksep_prohibit.SetUseEP(True)
    prohib_array.AddAlgo(tracksep_prohibit,False)
    
    outofcone_prohibit = cmtool.CBAlgoOutOfConeSeparate()
    outofcone_prohibit.SetMaxAngleSep(20.)
    prohib_array.AddAlgo(outofcone_prohibit,False)
    
    ## MERGE ALGOS ##
    merge_array = cmtool.CBAlgoArray()
    shortdist_alg = cmtool.CBAlgoShortestDist()
    shortdist_alg.SetMinHits(10)
    shortdist_alg.SetSquaredDistanceCut(5.)
    merge_array.AddAlgo(shortdist_alg,False)

    merger_instance.GetManager().AddMergeAlgo(merge_array)
    merger_instance.GetManager().AddSeparateAlgo(prohib_array)
    merger_instance.GetManager().MergeTillConverge(False)
    #merger_instance.GetManager().SetMinNHits(minHits)

    return merger_instance


def getStartTrack():

    merger_instance = larlite.ClusterMerger()
    merge_array = cmtool.CBAlgoArray()

    trackblob = cmtool.CBAlgoStartTrack()
    merge_array.AddAlgo(trackblob,False)
    
    merger_instance.GetManager().AddMergeAlgo(merge_array)
    merger_instance.GetManager().MergeTillConverge(False)

    return merger_instance

def getPolyContain():

    merger_instance = larlite.ClusterMerger()

    merge_array = cmtool.CBAlgoArray()
    pcontain = cmtool.CBAlgoPolyContain()
    merge_array.AddAlgo(pcontain,False)
    
    priorityAlg = cmtool.CPAlgoIgnoreTracks()

    merger_instance.GetManager().AddPriorityAlgo(priorityAlg)
    merger_instance.GetManager().AddMergeAlgo(merge_array)
    merger_instance.GetManager().MergeTillConverge(False)

    return merger_instance

    ########################################
    # Remove tracks with priority algo!
    ########################################

def getCOM():

    merger_instance = larlite.ClusterMerger()

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

    ########################################
    merge_array = cmtool.CBAlgoArray()
    COM_algo = cmtool.CBAlgoCenterOfMass()
    COM_algo.SetDebug(False)
    COM_algo.SetVerbose(False)
    COM_algo.UseCOMInPoly(True)
    COM_algo.UseCOMInCone(True)
    COM_algo.UseCOMNearClus(True)
    COM_algo.SetLengthReach(3.)
    merge_array.AddAlgo(COM_algo,False)

    merger_instance.GetManager().MergeTillConverge(False)
    merger_instance.GetManager().AddSeparateAlgo(prohib_array)
    merger_instance.GetManager().AddMergeAlgo(merge_array)

    return merger_instance

def getPolyOverlap():

    merger_instance = larlite.ClusterMerger()
    algo_array = cmtool.CBAlgoArray()

    overlapalg = cmtool.CBAlgoPolyOverlap()
    overlapalg.SetMinNumHits(0)
    algo_array.AddAlgo(overlapalg,False)

    merger_instance.GetManager().MergeTillConverge(False)
    merger_instance.GetManager().AddMergeAlgo(algo_array)

    return merger_instance


def getPolyShortestDist():

    merger_instance = larlite.ClusterMerger()
    algo_array = cmtool.CBAlgoArray()

    overlapalg = cmtool.CBAlgoPolyOverlap()

    polyshortalg_bigclusters = cmtool.CBAlgoPolyShortestDist()
    #this one is for big-ish clusters
    polyshortalg_bigclusters.SetMinNumHits(30)
    polyshortalg_bigclusters.SetMaxNumHits(9999)
    #two polygons that have points SUPER close have distsquared of ~0.5
    polyshortalg_bigclusters.SetMinDistSquared(2.)
    polyshortalg_bigclusters.SetDebug(False)
    algo_array.AddAlgo(polyshortalg_bigclusters,False)

    merger_instance.GetManager().MergeTillConverge(False)
    merger_instance.GetManager().AddMergeAlgo(algo_array)
    merger_instance.GetManager().MergeTillConverge(True)

    return merger_instance
