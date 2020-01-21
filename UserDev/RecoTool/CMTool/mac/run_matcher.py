import sys,os
from larlite import larlite as fmwk
from recotool.matchDef import *


def MatchAlgo():

    palgo_array = cmtool.CPAlgoArray()
    
    palgo1 = cmtool.CPAlgoNHits()
    palgo1.SetMinHits(25)
    
    palgo2 = cmtool.CPAlgoIgnoreTracks()
    
    palgo_array.AddAlgo(palgo1)
    palgo_array.AddAlgo(palgo2)

    algo_array = cmtool.CFAlgoArray()
    #algo_array.SetMode(cmtool.CFAlgoArray.kPositiveAddition)

    main_algo = cmtool.CFAlgoTimeOverlap()
    #main_algo = cmtool.CFAlgo3DAngle()
    #main_algo = cmtool.CFAlgoStartPointCompat()
    #main_algo = cmtool.CFAlgoWireOverlap()
    #main_algo = cmtool.CFAlgoQRatio()
    #main_algo = cmtool.CFAlgoStartPointMatch()
    #main_algo = cmtool.CFAlgoTimeProf()
    #main_algo = cmtool.CFAlgoZOverlap()
    #main_algo = cmtool.CFAlgoChargeDistrib()
    #main_algo = cmtool.CFAlgoShowerCompat()
    #main_algo = cmtool.CFAlgoStartTimeCompat()
    #main_algo = cmtool.CFAlgoVolumeOverlap()


    #main_algo.RequireThreePlanes(False)
    algo_array.AddAlgo(main_algo)
    #algo_array.AddAlgo(cmtool.CFAlgoTimeProf())
    #algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
    #algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())

    return palgo_array, algo_array

mgr = fmwk.ana_processor()

#args should be input file name
for x in range(len(sys.argv)-3):

    mgr.add_input_file(sys.argv[x+1])

# second to last argument is output pfpart producer
match_producer = sys.argv[-2]

#last arg should be output file name
out_file = sys.argv[-1]


if os.path.isfile(out_file):
    print()
    print('ERROR: output file already exist...')
    print()
    sys.exit(0)

mgr.set_output_file(out_file)

mgr.set_io_mode(fmwk.storage_manager.kBOTH)

mgr.set_ana_output_file("")

# DefaultMatch is the function called from $LARLITE/python/recot...
palgo_array, algo_array = DefaultMatch()
# MatchAlgo is the function defined in this script
#palgo_array, algo_array = MatchAlgo()

match_maker = fmwk.ClusterMatcher()

match_maker.GetManager().AddPriorityAlgo(palgo_array)
match_maker.GetManager().AddMatchAlgo(algo_array)

match_maker.SetClusterProducer("mccluster")
match_maker.SetOutputProducer(match_producer)
match_maker.SaveOutputCluster(True)

#mgr.set_data_to_write(fmwk.data.kCluster,"mccluster")
mgr.set_data_to_write(fmwk.data.kPFParticle,match_producer)
mgr.set_data_to_write(fmwk.data.kAssociation,match_producer)

mgr.add_process(match_maker)

mgr.run()


