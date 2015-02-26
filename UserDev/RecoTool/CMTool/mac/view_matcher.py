# Load libraries
import ROOT, sys, os
from ROOT import *


#filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlite_11.root"
my_proc = larlite.ana_processor()
my_proc.set_verbosity(larlite.msg.kDEBUG)
my_proc.set_io_mode(larlite.storage_manager.kREAD)

for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

my_proc.set_ana_output_file("")

# 
# Attach Matching algorithm
#
palgo_array = cmtool.CPAlgoArray()

palgo1 = cmtool.CPAlgoNHits()
palgo1.SetMinHits(25)

palgo2 = cmtool.CPAlgoIgnoreTracks()

palgo_array.AddAlgo(palgo1)
palgo_array.AddAlgo(palgo2)


#Andrzej: The algorithms below are ranked by their effectiveness-- TimeOverlap is best, 
#then 3DAngle, then StartPoint . Right now, only TimeOverlap is called.
algo_array = cmtool.CFAlgoArray()
#algo_array.SetMode(cmtool.CFAlgoArray.kPositiveAddition)
algo_array.AddAlgo(cmtool.CFAlgoTimeOverlap())
#algo_array.AddAlgo(cmtool.CFAlgoTimeProf())
#algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
#algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())

raw_viewer   = larlite.ClusterViewer()
match_viewer = larlite.MatchViewer()


########################################
# decide what to show on display
########################################
raw_viewer.SetDrawShowers(False)
raw_viewer.SetDrawStartEnd(False)

match_viewer.SetPrintClusterInfo(True)
#Show Showers: requires MC info in hadded files
match_viewer.SetDrawShowers(False)

########################################
# attach match algos here
########################################

match_viewer.GetManager().AddPriorityAlgo(palgo_array)
match_viewer.GetManager().AddMatchAlgo(algo_array)

########################################
# done attaching match algos
########################################

my_proc.add_process(raw_viewer)

my_proc.add_process(match_viewer)

raw_viewer.SetClusterProducer("mergedfuzzy")

match_viewer.SetClusterProducer("mergedfuzzy")

gStyle.SetOptStat(0)

#start on first event always
user_input_evt_no = -1;

while true:

    try:
        user_input_evt_no = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        user_input_evt_no = user_input_evt_no + 1

    my_proc.process_event(user_input_evt_no)

    raw_viewer.DrawAllClusters();

    match_viewer.DrawAllClusters();

    print "    Hit enter to go next event..."
    sys.stdin.readline()


# done!
