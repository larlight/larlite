# Load libraries
import ROOT, sys, os
from ROOT import *


#filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlight_11.root"
my_proc = larlite.ana_processor()
my_proc.set_verbosity(larlite.msg.kDEBUG)

my_proc.set_io_mode(larlite.storage_manager.kREAD)

for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

my_proc.set_ana_output_file("")

raw_viewer   = larlite.ClusterViewer()
merge_viewer = larlite.MergeViewer()
ConfigureMergeAllInstance(merge_viewer)

########################################
# decide what to show on display
########################################
#SetDrawShowers: requires MC info in hadded files
raw_viewer.SetDrawShowers(False)
raw_viewer.SetDrawStartEnd(False)

merge_viewer.SetPrintClusterInfo(True)
merge_viewer.SetDrawStartEnd(False)
merge_viewer.SetDrawPolygon(True)

my_proc.add_process(raw_viewer)

my_proc.add_process(merge_viewer)

raw_viewer.SetClusterProducer("fuzzycluster")
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
