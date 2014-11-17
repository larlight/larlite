
########################################################################
# This module contains the default preliminary merging settings
# Not meant to be run as a stand alone script, instead mean to be imported
########################################################################

from ROOT import larlite as fmwk,cmtool

def GetPrelimMergerInstance(producer="fuzzycluster",saveOutput=True):
    
    result = fmwk.ClusterMerger()

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
    
    result.GetManager().AddSeparateAlgo(prohib_array)

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
    
    result.GetManager().AddMergeAlgo(merge_array)

    result.GetManager().MergeTillConverge(True)

    result.SetClusterProducer(producer)
    result.SaveOutputCluster(saveOutput)

    return result

def GetSecondMergerInstance(producer="mergedfuzzycluster",saveOutput=True):
    
    result = fmwk.ClusterMerger()
    result.SetClusterProducer(producer)
    result.SaveOutputCluster(saveOutput)


    ########################################
    # attach merge algos here
    ########################################

    ########################################
    # Remove tracks with priority algo!
    ########################################
    result.GetManager().AddPriorityAlgo(cmtool.CPAlgoIgnoreTracks())



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

    result.GetManager().AddSeparateAlgo(prohib_array)

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

    result.GetManager().AddMergeAlgo(algo_array)
    result.GetManager().MergeTillConverge(True)

    ########################################
    # done attaching merge algos
    ########################################

    return result



