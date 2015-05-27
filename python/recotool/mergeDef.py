
########################################################################
# This module contains the default preliminary merging settings
# Not meant to be run as a stand alone script, instead mean to be imported
# The "Configure" functions hold the definitions of different stage merge algos
# and parameters. The "Get" functions are wrappers for the "Configure" functions
# These are separated so a user can pass objects to Configure function that
# inherit from ClusterMerger, IE to configure MergeViewer objects.
########################################################################

from larlite import larlite as fmwk
from cpp_classes import cmtool


def DebugMultipleManagers(merger_instance=0,producer='fuzzycluster',saveOutput=True):
    if not merger_instance:
        raise Exception('To call ConfigureDefaultMergerInstance function, you need to hand it a ClusterMerger() instance! Or, an instance of something that inherits from ClusterMerger.')

    ######################################
    # attach 1st stage  merge algos here #
    ######################################

    ## PROHIBIT ALGOS ##
    prohib_array = cmtool.CBAlgoArray()
    
    tracksep_prohibit = cmtool.CBAlgoTrackSeparate()
    tracksep_prohibit.SetUseEP(True)
    prohib_array.AddAlgo(tracksep_prohibit,False)
    
    outofcone_prohibit = cmtool.CBAlgoOutOfConeSeparate()
    outofcone_prohibit.SetMaxAngleSep(20.)
    prohib_array.AddAlgo(outofcone_prohibit,False)
    
    merger_instance.GetManager().AddSeparateAlgo(prohib_array)

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
    
    merger_instance.GetManager().AddMergeAlgo(merge_array)

    merger_instance.GetManager().MergeTillConverge(True)

    merger_instance.SetInputProducer(producer)
    merger_instance.SetOutputProducer('fuzzyclustermerger')
    merger_instance.SaveOutputCluster(saveOutput)

    merger_instance.GetManager(1).AddMergeAlgo(cmtool.CBAlgoMergeAll())


def ConfigureMergeAllInstance(merger_instance=0, producer="fuzzycluster",saveOutput=True):
    if not merger_instance:
        raise Exception('To call ConfigureDefaultMergerInstance function, you need to hand it a ClusterMerger() instance! Or, an instance of something that inherits from ClusterMerger.')

    merger_instance.GetManager().AddMergeAlgo(cmtool.CBAlgoMergeAll())
    merger_instance.GetManager().MergeTillConverge(True)
    merger_instance.SetInputProducer(producer)
    merger_instance.SaveOutputCluster(saveOutput)

def ConfigureDefaultMergerInstance(merger_instance=0,producer='fuzzycluster',saveOutput=True):
    #need to build in protection if user doesn't supply merger instance
    if not merger_instance:
        raise Exception('To call ConfigureDefaultMergerInstance function, you need to hand it a ClusterMerger() instance! Or, an instance of something that inherits from ClusterMerger.')

    ######################################
    # attach 1st stage  merge algos here #
    ######################################

    ## PROHIBIT ALGOS ##
    prohib_array = cmtool.CBAlgoArray()
    
    tracksep_prohibit = cmtool.CBAlgoTrackSeparate()
    tracksep_prohibit.SetUseEP(True)
    prohib_array.AddAlgo(tracksep_prohibit,False)
    
    outofcone_prohibit = cmtool.CBAlgoOutOfConeSeparate()
    outofcone_prohibit.SetMaxAngleSep(20.)
    prohib_array.AddAlgo(outofcone_prohibit,False)
    
    merger_instance.GetManager().AddSeparateAlgo(prohib_array)

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
    
    merger_instance.GetManager().AddMergeAlgo(merge_array)

    merger_instance.GetManager().MergeTillConverge(True)

    merger_instance.SetInputProducer(producer)
    merger_instance.SetOutputProducer('fuzzyclustermerger')
    merger_instance.SaveOutputCluster(saveOutput)

    ########################################
    # done attaching 2nd stage merge algos #
    ########################################

    #####################################
    # attach 2nd stage merge algos here #
    #####################################

    ########################################
    # Remove tracks with priority algo!
    ########################################
    merger_instance.GetManager(1).AddPriorityAlgo(cmtool.CPAlgoIgnoreTracks())

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

    merger_instance.GetManager(1).AddSeparateAlgo(prohib_array)

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

    merger_instance.GetManager(1).AddMergeAlgo(algo_array)
    merger_instance.GetManager(1).MergeTillConverge(True)

    ########################################
    # done attaching 2nd stage merge algos #
    ########################################

def ConfigurePrelimMergerInstance(merger_instance=0, producer="fuzzycluster",saveOutput=True):
    if not merger_instance:
        raise Exception('To call ConfigureDefaultMergerInstance function, you need to hand it a ClusterMerger() instance! Or, an instance of something that inherits from ClusterMerger.')

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
    
    merger_instance.GetManager().AddSeparateAlgo(prohib_array)

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
    
    merger_instance.GetManager().AddMergeAlgo(merge_array)

    merger_instance.GetManager().MergeTillConverge(True)

    merger_instance.SetInputProducer(producer)
    merger_instance.SaveOutputCluster(saveOutput)

def ConfigureSecondMergerInstance(merger_instance=0, producer="mergedfuzzycluster",saveOutput=True):
    if not merger_instance:
        raise Exception('To call ConfigureDefaultMergerInstance function, you need to hand it a ClusterMerger() instance! Or, an instance of something that inherits from ClusterMerger.')

    merger_instance.SetInputProducer(producer)
    merger_instance.SaveOutputCluster(saveOutput)


    ########################################
    # attach merge algos here
    ########################################

    ########################################
    # Remove tracks with priority algo!
    ########################################
    merger_instance.GetManager().AddPriorityAlgo(cmtool.CPAlgoIgnoreTracks())

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

    merger_instance.GetManager().AddSeparateAlgo(prohib_array)

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

    merger_instance.GetManager().AddMergeAlgo(algo_array)
    merger_instance.GetManager().MergeTillConverge(True)

    ########################################
    # done attaching merge algos
    ########################################

def GetMergeAllInstance(producer='fuzzycluster',saveOutput=True):
    merger_instance = fmwk.ClusterMerger()
    ConfigureMergeAllInstance(merger_instance,producer=producer,saveOutput=saveOutput)
    return merger_instance

def GetDefaultMergerInstance(producer='fuzzycluster',saveOutput=True):
    merger_instance = fmwk.ClusterMerger()
    ConfigureDefaultMergerInstance(merger_instance,producer=producer,saveOutput=saveOutput)
    return merger_instance

def GetPrelimMergerInstance(producer='fuzzycluster',saveOutput=True):
    merger_instance = fmwk.ClusterMerger()
    ConfigurePrelimMergerInstance(merger_instance,producer=producer,saveOutput=saveOutput)
    return merger_instance

def GetSecondMergerInstance(producer='fuzzycluster',saveOutput=True):
    merger_instance = fmwk.ClusterMerger()
    ConfigureSecondMergerInstance(merger_instance,producer=producer,saveOutput=saveOutput)
    return merger_instance
