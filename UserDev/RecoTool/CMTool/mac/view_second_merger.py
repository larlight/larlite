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
my_proc = larlight.ana_processor()
my_proc.set_verbosity(larlight.MSG.DEBUG)

my_proc.set_io_mode(larlight.storage_manager.READ)

my_proc.add_input_file(filename)

larlight.storage_manager.get().set_in_rootdir("scanner")

my_proc.set_ana_output_file("")

raw_viewer   = larlight.ClusterViewer()
merge_viewer = larlight.MergeViewer()

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

# Remove tracks with priority algo!
merge_viewer.GetManager().AddPriorityAlgo(cmtool.CPAlgoIgnoreTracks())

# PROHIBIT ALGORITHMS
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

merge_viewer.GetManager().AddSeparateAlgo(prohib_array)

# MERGE ALGORITHMS
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

merge_viewer.GetManager().AddMergeAlgo(algo_array)

########################################
# done attaching merge algos
########################################

########################################
# cmerge manager parameters
########################################
merge_viewer.GetManager().MergeTillConverge(True)


########################################
# Configure process & run
########################################

my_proc.add_process(raw_viewer)

my_proc.add_process(merge_viewer)

raw_viewer.SetInputProducer("mergedfuzzycluster")
merge_viewer.SetInputProducer("mergedfuzzycluster")

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
