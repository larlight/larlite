# Load libraries
import ROOT, sys, os
from ROOT import *


#filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlight_11.root"
my_proc = larlite.ana_processor()
my_proc.set_verbosity(larlite.msg.kDEBUG)

my_proc.set_io_mode(larlite.storage_manager.kREAD)

for x in range(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

my_proc.set_ana_output_file("")
larutil.LArUtilManager.Reconfigure(larlite.geo.kArgoNeuT)

raw_viewer   = larlite.ClusterViewer()
merge_viewer = larlite.MergeViewer()

########################################
# decide what to show on display
########################################
#SetDrawShowers: requires MC info in hadded files
raw_viewer.SetDrawShowers(False)
raw_viewer.SetDrawStartEnd(False)

merge_viewer.SetPrintClusterInfo(True)
merge_viewer.SetDrawStartEnd(False)
merge_viewer.SetDrawPolygon(False)

########################################
# attach merge algos here
########################################


########################################
# Remove tracks with priority algo!
########################################
# priority_alg = cmtool.CPAlgoIgnoreTracks()
# priority_alg.SetMinCharge(10000)
# priority_alg.SetMinHits(10)
# merge_viewer.GetManager().AddPriorityAlgo(priority_alg)



########################################
# PROHIBIT ALGORITHMS
########################################
prohib_array = cmtool.CBAlgoArray()

tracksep_prohibit = cmtool.CBAlgoTrackSeparate()
tracksep_prohibit.SetDebug(False)
tracksep_prohibit.SetVerbose(False)
tracksep_prohibit.SetMinAngleDiff(5)
tracksep_prohibit.SetUseEP(False)
prohib_array.AddAlgo(tracksep_prohibit,False)

# outofcone_prohibit = cmtool.CBAlgoOutOfConeSeparate()
# outofcone_prohibit.SetDebug(False)
# outofcone_prohibit.SetVerbose(False)
# outofcone_prohibit.SetMaxAngleSep(20.)
# prohib_array.AddAlgo(outofcone_prohibit,False)

# angle_prohibit = cmtool.CBAlgoAngleIncompat()
# #this only applies if both clusters have >50 hits
# angle_prohibit.SetMinHits(50)
# angle_prohibit.SetAllow180Ambig(True)
# angle_prohibit.SetUseOpeningAngle(False)
# #this prohibits clusters w/ angles different than 10 degrees
# angle_prohibit.SetAngleCut(10.)
# angle_prohibit.SetMinLength(20.)
# angle_prohibit.SetDebug(False)
# prohib_array.AddAlgo(angle_prohibit,False)

merge_viewer.GetManager().AddSeparateAlgo(prohib_array)

########################################
# MERGE ALGORITHMS
########################################
algo_array = cmtool.CBAlgoArray()


ALL_algo = cmtool.CBAlgoMergeSmallToTrack()
ALL_algo.SetDebug(True)
# ALL_algo.SetMinModHitDens(1.5)
# ALL_algo.SetMinPrincipal(10)
# ALL_algo.SetMaxHit(13)
# ALL_algo.SetMaxWidth(5)
# ALL_algo.SetMaxLength(15)
ALL_algo.SetMaxClosestDist(2.5);
algo_array.AddAlgo(ALL_algo)


merge_viewer.GetManager().AddMergeAlgo(algo_array)
merge_viewer.GetManager().MergeTillConverge(True)

# done attaching merge algos
########################################
merge_viewer.GetManager().MergeTillConverge(False)


my_proc.add_process(raw_viewer)

my_proc.add_process(merge_viewer)

raw_viewer.SetClusterProducer("cccluster")
merge_viewer.SetInputProducer("cccluster")

gStyle.SetOptStat(0)

#start on first event always
user_input_evt_no = -1;

while true:

    try:
        user_input_evt_no = eval(input('Hit Enter to continue to next evt, or type in an event number to jump to that event:'))
    except SyntaxError:
        user_input_evt_no = user_input_evt_no + 1

    my_proc.process_event(user_input_evt_no)

    raw_viewer.DrawAllClusters();

    merge_viewer.DrawAllClusters();


#    for plane in xrange(larutil.Geometry.GetME().Nplanes()):
#
#        print "    Plane:", plane
#        
#        for cindex in xrange(merge_viewer.ClusterCount(plane)):
#
#            print "        Cluster:",cindex
#            merge_viewer.DrawOneCluster(plane,cindex)
#            sys.stdin.readline()
#    

    print("    Hit enter to go next event...")
    sys.stdin.readline()


# done!
