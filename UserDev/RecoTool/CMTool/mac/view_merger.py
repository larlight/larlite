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
larlight.storage_manager.get().set_data_to_read(larlight.DATA.MCTruth,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Shower,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Calorimetry,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.UserInfo,False)

my_proc.set_ana_output_file("")

raw_viewer   = larlight.ClusterViewer()
merge_viewer = larlight.MergeViewer()

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
merge_viewer.GetManager().AddPriorityAlgo(cmtool.CPAlgoIgnoreTracks())



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

merge_viewer.GetManager().AddSeparateAlgo(prohib_array)

########################################
# MERGE ALGORITHMS
########################################
algo_array = cmtool.CBAlgoArray()

COM_algo = cmtool.CBAlgoCenterOfMassSmall()
COM_algo.SetDebug(False)
COM_algo.SetVerbose(False)
COM_algo.UseCOMInPoly(True)
COM_algo.UseCOMClose(True)
COM_algo.UseCOMNearClus(True)
COM_algo.SetMaxDistance(20.)
COM_algo.SetMaxCOMDistance(20.)
COM_algo.SetMaxHitsSmallClus(40)
algo_array.AddAlgo(COM_algo,False)

merge_viewer.GetManager().AddMergeAlgo(algo_array)
# done attaching merge algos
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

    print "    Hit enter to go next event..."
    sys.stdin.readline()


# done!
