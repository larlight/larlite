#KAZU, USE THIS LIKE:
#python tmp_view_cherrypicked_pi0s.py $YOUR_DATA_PATH/MCC5/SecondMergedFiles/pdg_bnb_nc/combined_cluster_*


# Load libraries
import ROOT, sys, os
from ROOT import *

# Now import ana_processor & your class. For this example, ana_base.
#gSystem.Load("libCMTool")
from ROOT import *

good_pi0_evt_nos = [556, 1580, 1779, 2107, 3283]

filename = sys.argv[1]
my_proc = larlight.ana_processor()
my_proc.set_verbosity(larlight.MSG.DEBUG)

my_proc.set_io_mode(larlight.storage_manager.READ)


for blah in xrange(len(sys.argv)):
    if blah == 0: continue

    my_proc.add_input_file(sys.argv[blah])


larlight.storage_manager.get().set_in_rootdir("scanner")

my_proc.set_ana_output_file("")

raw_viewer   = larlight.ClusterViewer()

#decide if to show hit charge OR MCShowers on RHS of TCanvas [default: false]
#raw_viewer.SetDrawShowers(True)

#if you're showing hit charge, whether to use log z scale [default: true]
#raw_viewer.SetHitsLogZ(False)

my_proc.add_process(raw_viewer)

raw_viewer.SetClusterType(larlight.DATA.Cluster)

gStyle.SetOptStat(0)

#start on first event always
for my_evt in good_pi0_evt_nos:

    my_proc.process_event(my_evt-1)

    raw_viewer.DrawAllClusters();

    sys.stdin.readline()


# done!
