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
ConfigureSecondMergerInstance(merge_viewer)

########################################
# merge viewer parameters
########################################
merge_viewer.SetPrintClusterInfo(True)
merge_viewer.SetMinHitsToDraw(0)
merge_viewer.SetDrawPolygon(True)
merge_viewer.SetDrawOnlyTracks(False)


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
