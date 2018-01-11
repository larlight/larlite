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
my_proc = larlite.ana_processor()
my_proc.set_verbosity(larlite.msg.kDEBUG)

my_proc.set_io_mode(larlite.storage_manager.kREAD)

my_proc.add_input_file(filename)

larlite.storage_manager.get().set_in_rootdir("scanner")

my_proc.set_ana_output_file("")
larutil.LArUtilManager.Reconfigure(larlite.geo.kArgoNeuT)

raw_viewer   = larlite.ClusterViewer()

#decide if to show hit charge OR MCShowers on RHS of TCanvas [default: false]
#raw_viewer.SetDrawShowers(True)

#if you're showing hit charge, whether to use log z scale [default: true]
#raw_viewer.SetHitsLogZ(False)

my_proc.add_process(raw_viewer)

raw_viewer.SetClusterProducer("ccMerged6")

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

    print "Just showed Event %d. Hit enter to go next event..." % user_input_evt_no
    sys.stdin.readline()


# done!
