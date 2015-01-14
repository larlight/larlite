# Load libraries
import ROOT, sys, os
from ROOT import *

# Now import ana_processor & your class. For this example, ana_base.
gSystem.Load("libCMTool")
from ROOT import *
if len(sys.argv) != 2:
    print
    print "*** Improper usage. Usage: python viewclusters.py /path/to/input/file.root ***"
    print


filename = sys.argv[1]
#filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlight_11.root"
my_proc = larlite.ana_processor()
my_proc.set_verbosity(larlite.msg.kDEBUG)

my_proc.set_io_mode(larlite.storage_manager.kREAD)

my_proc.add_input_file(filename)

# larlite.storage_manager.get().set_in_rootdir("scanner")
# larlite.storage_manager.get().set_data_to_read(larlite.data.kMCTruth,"mctruth")
# larlite.storage_manager.get().set_data_to_read(larlite.data.kShower,False)
# larlite.storage_manager.get().set_data_to_read(larlite.data.kCalorimetry,False)
# larlite.storage_manager.get().set_data_to_read(larlite.data.kUserInfo,False)

my_proc.set_ana_output_file("")

raw_viewer   = larlite.ClusterViewer()
merge_viewer = larlite.MergeViewer()

########################################
# merge viewer parameters
########################################
merge_viewer.SetPrintClusterInfo(True)
merge_viewer.SetMinHitsToDraw(0)
merge_viewer.SetDrawPolygon(True)
merge_viewer.SetDrawOnlyTracks(False)

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

merge_viewer.GetManager().AddSeparateAlgo(prohib_array)

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

merge_viewer.GetManager().AddMergeAlgo(merge_array)

########################################
# done attaching merge algos
########################################

########################################
# cmerge manager parameters
########################################
merge_viewer.GetManager().MergeTillConverge(True)



my_proc.add_process(raw_viewer)

my_proc.add_process(merge_viewer)

raw_viewer.SetInputProducer("fuzzycluster")
merge_viewer.SetInputProducer("fuzzycluster")

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

    merge_viewer.DrawAllClusters();

    print "Just displayed Event %d.  Hit enter to go next event..." % user_input_evt_no
    sys.stdin.readline()


# done!
