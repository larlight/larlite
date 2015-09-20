

# This class defines the merging algorithms, their parameters, and returns
# a configuration to use.  It replaces the old files "simple_batch_merge"
# and "clusterCrawlerShowerArgo."
#

# Import the libs needed:
from cpp_classes import argomerge
from ROOT import larlite, cmtool


def getSmallClustMerger(maxHitsProhib=5,
                        maxHitsSmall=1,
                        maxDist=0.5,
                        maxDistAv=2.4,
                        minHits=1):

    merger = larlite.ClusterMerger()
    ########################################
    # PROHIBIT ALGORITHMS
    ########################################
    prohib_array = cmtool.CBAlgoArray()
    big_prohibit = argomerge.CBAlgoProhibitBigToBig()
    big_prohibit.SetMaxHits(maxHitsProhib)
    prohib_array.AddAlgo(big_prohibit, False)

    ########################################
    # MERGE ALGORITHMS
    ########################################
    algo_array = cmtool.CBAlgoArray()
    singleToBig = argomerge.CBAlgoMergeSingleToBig()
    singleToBig.SetMaxDistance(maxDist)
    singleToBig.SetMaxAverageDistance(maxDistAv)
    singleToBig.SetMaxSmallClustHits(maxHitsSmall)

    algo_array.AddAlgo(singleToBig)

    merger.GetManager().AddMergeAlgo(algo_array)
    merger.GetManager().AddSeparateAlgo(prohib_array)
    merger.GetManager().MergeTillConverge(False)
    merger.GetManager().SetMinNHits(minHits)
    return merger


def getExtendBlobMerger(prohibitBig=True,
                        bignessProhibit=25,
                        mode=0):
    merger = larlite.ClusterMerger()
    ########################################
    # PROHIBIT ALGORITHMS
    ########################################
    prohib_array = cmtool.CBAlgoArray()

    # Prohibit Merging Track to track:
    # t2t_prohibit = argomerge.CBAlgoProhibitTrackToTrack()
    # t2t_prohibit.SetMinHits(10)
    # t2t_prohibit.SetMinMHitWiresFraction(0.5)
    # t2t_prohibit.SetMinPrincipal(0.99)
    # t2t_prohibit.SetMinLengthWidthRatio(10)
    # t2t_prohibit.SetMode(argomerge.CBAlgoProhibitTrackToTrack.kBOTH)
    # # t2t_prohibit.SetDebug(True)
    # prohib_array.AddAlgo(t2t_prohibit, False)

    if (prohibitBig):
        big_prohibit = argomerge.CBAlgoProhibitBigToBig()
        big_prohibit.SetMaxHits(bignessProhibit)
        prohib_array.AddAlgo(big_prohibit, False)

    # Want to add a prohibit function that stops if
    # start to start point distance is too close
    # s2s_prohibit = argomerge.CBAlgoProhibitBigStart()
    # s2s_prohibit.SetMinSeparation(1.0)
    # s2s_prohibit.SetMinHits(bignessProhibit)
    # prohib_array.AddAlgo(s2s_prohibit, False)

    ########################################
    # MERGE ALGORITHMS
    ########################################
    algo_array = cmtool.CBAlgoArray()

    blob = argomerge.CBAlgoMergeExtendBlob()
    blob.set_principal_ev_cut(0.9)
    blob.set_rms_scale(2.0)
    blob.set_length_jump_scale(0.25)
    blob.set_min_hits_to_project_from(30)
    blob.set_mode(mode)
    blob.set_debug(False)
    # blob.SetMaxDistance(shortestDist)
    algo_array.AddAlgo(blob)
    merger.GetManager().AddMergeAlgo(algo_array)
    if prohibitBig:
        merger.GetManager().AddSeparateAlgo(prohib_array)
    merger.GetManager().MergeTillConverge(False)
    if prohibitBig:
        merger.GetManager().SetMinNHits(3)
    else:
        merger.GetManager().SetMinNHits(30)

    return merger


def getMergeToTrunk(shortestDist, maxClusterSize, prohibitBig=False):
    merger = larlite.ClusterMerger()
    ########################################
    # PROHIBIT ALGORITHMS
    ########################################
    prohib_array = cmtool.CBAlgoArray()

    # Prohibit Merging Track to track:
    t2t_prohibit = argomerge.CBAlgoProhibitTrackToTrack()
    t2t_prohibit.SetMinHits(20)
    t2t_prohibit.SetMinMHitWiresFraction(0.2)
    t2t_prohibit.SetMinPrincipal(0.98)
    t2t_prohibit.SetMinLengthWidthRatio(10)
    t2t_prohibit.SetMode(argomerge.CBAlgoProhibitTrackToTrack.kBOTH)
    # t2t_prohibit.SetDebug(True)
    # prohib_array.AddAlgo(t2t_prohibit, False)

    if (prohibitBig):
        big_prohibit = argomerge.CBAlgoProhibitBigToBig()
        big_prohibit.SetMaxHits(maxClusterSize)
        prohib_array.AddAlgo(big_prohibit, False)

    # Want to add a prohibit function that stops if
    # start to start point distance is too close
    s2s_prohibit = argomerge.CBAlgoProhibitBigStart()
    s2s_prohibit.SetMinSeparation(2.5)
    # s2s_prohibit.SetDebug(True)
    prohib_array.AddAlgo(s2s_prohibit, False)

    ########################################
    # MERGE ALGORITHMS
    ########################################
    algo_array = cmtool.CBAlgoArray()

    SDAlg = argomerge.CBAlgoMergeShortestDistance()
    # SDAlg.SetDebug(True)
    SDAlg.SetMaxDistance(shortestDist)
    algo_array.AddAlgo(SDAlg)

    merger.GetManager().AddMergeAlgo(algo_array)
    merger.GetManager().AddSeparateAlgo(prohib_array)
    merger.GetManager().MergeTillConverge(True)
    merger.GetManager().SetMinNHits(3)
    return merger


def getInlineMerger(maxInlineDist=0.6,
                    useAllHits=True,
                    hitFraction=0.35,
                    minHits=5,
                    bignessProhibit=25):
    merger = larlite.ClusterMerger()
    ########################################
    # PROHIBIT ALGORITHMS
    ########################################
    prohib_array = cmtool.CBAlgoArray()
    # big_prohibit = argomerge.CBAlgoProhibitBigToBig()
    # big_prohibit.SetMaxHits(40)
    # prohib_array.AddAlgo(big_prohibit,False)
    # Want to add a prohibit function that stops if
    # start to start point distance is too close

    s2s_prohibit = argomerge.CBAlgoProhibitBigStart()
    s2s_prohibit.SetMinSeparation(1.0)
    s2s_prohibit.SetMinHits(bignessProhibit)
    # s2s_prohibit.SetDebug(True)
    prohib_array.AddAlgo(s2s_prohibit, False)

    ########################################
    # MERGE ALGORITHMS
    ########################################
    algo_array = cmtool.CBAlgoArray()
    inline = argomerge.CBAlgoMergeInline()
    inline.SetMaxAverageMinDistance(maxInlineDist)
    inline.SetUseAllHits(useAllHits)
    inline.SetHitFraction(hitFraction)
    algo_array.AddAlgo(inline)

    merger.GetManager().AddMergeAlgo(algo_array)
    merger.GetManager().AddSeparateAlgo(prohib_array)
    merger.GetManager().MergeTillConverge(False)
    merger.GetManager().SetMinNHits(minHits)
    return merger


def getStartTrackMerger():
    merger = larlite.ClusterMerger()
    ########################################
    # PROHIBIT ALGORITHMS
    ########################################
    # prohib_array = argomerge.CBAlgoArray()

    # # Prohibit Merging Track to track:
    # t2t_prohibit = argomerge.CBAlgoProhibitTrackToTrack()
    # t2t_prohibit.SetMinHits(10)
    # t2t_prohibit.SetMinMHitWiresFraction(0.5)
    # t2t_prohibit.SetMinPrincipal(0.99)
    # t2t_prohibit.SetMinLengthWidthRatio(10)
    # t2t_prohibit.SetMode(argomerge.CBAlgoProhibitTrackToTrack.kBOTH)
    # # t2t_prohibit.SetDebug(True)
    # prohib_array.AddAlgo(t2t_prohibit, False)

    # if (prohibitBig):
    #   big_prohibit = argomerge.CBAlgoProhibitBigToBig()
    #   big_prohibit.SetMaxHits(10)
    #   prohib_array.AddAlgo(big_prohibit, False)

    # # Want to add a prohibit function that stops if
    # # start to start point distance is too close
    # s2s_prohibit = argomerge.CBAlgoProhibitStartToStart()
    # s2s_prohibit.SetMinSeparation(1.0)
    # # s2s_prohibit.SetDebug(True)
    # prohib_array.AddAlgo(s2s_prohibit, False)

    ########################################
    # MERGE ALGORITHMS
    ########################################
    algo_array = cmtool.CBAlgoArray()

    SDAlg = cmtool.CBAlgoStartTrack()
    # SDAlg.SetDebug(True)
    # SDAlg.SetMaxDistance(shortestDist)
    algo_array.AddAlgo(SDAlg)

    merger.GetManager().AddMergeAlgo(algo_array)
    # merger.GetManager().AddSeparateAlgo(prohib_array)
    merger.GetManager().MergeTillConverge(True)
    merger.GetManager().SetMinNHits(5)
    return merger


# This function actually makes an instance of each of the necessary
# mergers and returns them as an array

def argoMergeProcList(initProducer="cccluster", finalProducer="ccMergedFinal"):

    procList = []
    prevProducer = initProducer

    # Module to take all of the poorly done, vertical tracks and destroy them
    dbvc = larlite.DropBadVertClusters()
    dbvc.SetInputProducer(prevProducer)
    dbvc.SetOutputProducer("ccNoVert")
    prevProducer = "ccNoVert"
    procList.append(dbvc)

    # Module to turn all the single hits into one hit clusters
    htc = larlite.HitToCluster()
    htc.SetInputProducer(prevProducer)
    htc.SetOutputProducer("ccclusterWithSingles")
    prevProducer = "ccclusterWithSingles"
    procList.append(htc)

    # Using an iterative merging approach:
    maxClosestDistances = [0.5,   0.8,  1.0,  0.8]
    maxAverageDistances = [9999,  9999, 9999, 9999]
    maxHitsSmall = [1,     1,    3,    7]
    maxHitsProhibit = [1,     1,    3,    7]
    minHitsInCluster = [1,     1,    1,    3]

    for i in range(0, len(maxClosestDistances)):
        merger = getSmallClustMerger(
            maxHitsSmall=maxHitsSmall[i],
            maxHitsProhib=maxHitsProhibit[i],
            maxDist=maxClosestDistances[i],
            maxDistAv=maxAverageDistances[i],
            minHits=minHitsInCluster[i])
        merger.SetInputProducer(prevProducer)
        merger.SetOutputProducer("ccMergedSmall" + str(i))
        merger.SaveOutputCluster()
        prevProducer = "ccMergedSmall" + str(i)
        procList.append(merger)

    merger = getExtendBlobMerger(True)
    merger.SetInputProducer(prevProducer)
    merger.SetOutputProducer("ccMergedExtendBlobNoBig")
    prevProducer = "ccMergedExtendBlobNoBig"
    merger.SaveOutputCluster()
    procList.append(merger)

    # Add the inline merger:
    inlineMerger = getInlineMerger(maxInlineDist=0.5)
    inlineMerger.SetInputProducer(prevProducer)
    inlineMerger.SetOutputProducer("ccMergedInline")
    prevProducer = "ccMergedInline"
    inlineMerger.SaveOutputCluster()
    procList.append(inlineMerger)

    maxClosestDistances = [0.6, 0.8, 1.2, 1.3, ]
    maxClusterSizes = [20,  20,  20,  20, ]

    for i in range(0, len(maxClosestDistances)):
        merger = getMergeToTrunk(
            shortestDist=maxClosestDistances[i],
            maxClusterSize=maxClusterSizes[i],
            prohibitBig=True)
        merger.SetInputProducer(prevProducer)
        merger.SetOutputProducer("ccMergedSDnoBig" + str(i))
        merger.SaveOutputCluster()
        prevProducer="ccMergedSDnoBig" + str(i)
        procList.append(merger)

    merger=getStartTrackMerger()
    merger.SetInputProducer(prevProducer)
    merger.SetOutputProducer("ccMergedStartTrack")
    prevProducer="ccMergedStartTrack"
    merger.SaveOutputCluster()
    procList.append(merger)

    merger=getExtendBlobMerger(False, 50, 1)
    merger.SetInputProducer(prevProducer)
    merger.SetOutputProducer("ccMergedExtendBlob")
    prevProducer="ccMergedExtendBlob"
    merger.SaveOutputCluster()
    procList.append(merger)

    merger=getExtendBlobMerger(False, 100, 1)
    merger.SetInputProducer(prevProducer)
    merger.SetOutputProducer("ccMergedExtendBlob2")
    prevProducer="ccMergedExtendBlob2"
    merger.SaveOutputCluster()
    procList.append(merger)

    # Add a DropSingles module:
    drop=larlite.DropSingles()
    drop.SetInputProducer(prevProducer)
    drop.SetOutputProducer("ccMergedFinal")
    procList.append(drop)

    return procList
