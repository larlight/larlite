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
match_viewer = larlight.MatchViewer()
#mc_viewer    = larlight.MCShowerClusterViewer()
clusterz = cluster.CRUHelper()

match_viewer.SetPrintClusterInfo(True)
#match_viewer.ShowShowers(False)
#match_viewer.SetDrawShowers(True)
########################################
# attach match algos here
########################################


priority_algo = cmtool.CPAlgoNHits()
priority_algo.SetMinHits(22)
match_viewer.GetManager().AddPriorityAlgo(priority_algo)

algo_array = cmtool.CFAlgoArray()

angleAlg = cmtool.CFAlgo3DAngle()
angleAlg.SetRatio(0.1)
angleAlg.SetDebug(True)

timeAlg = cmtool.CFAlgoTimeOverlap()
timeAlg.SetDebug(True)

volAlg = cmtool.CFAlgoVolumeOverlap()

algo_array.AddAlgo(angleAlg)
algo_array.AddAlgo(timeAlg)
#algo_array.AddAlgo(volAlg)

match_viewer.GetManager().AddMatchAlgo(algo_array)

########################################
# done attaching match algos
########################################

my_proc.add_process(raw_viewer)

my_proc.add_process(match_viewer)

#my_proc.add_process(mc_viewer)

raw_viewer.SetClusterType(larlight.DATA.Cluster)
#raw_viewer.SetClusterType(larlight.DATA.MCShowerCluster)

match_viewer.SetClusterType(larlight.DATA.Cluster)
#match_viewer.SetClusterType(larlight.DATA.MCShowerCluster)

gStyle.SetOptStat(0)

####
# want to mark begin points
####
#algo = cluster.ClusterParamsExecutor()
#result = algo.GetParams()



#start on first event always
user_input_evt_no = -1;

counter = 0

while true:

    try:
        user_input_evt_no = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        user_input_evt_no = user_input_evt_no + 1

    my_proc.process_event(user_input_evt_no)

    raw_viewer.DrawAllClusters();

    match_viewer.DrawAllClusters();

    #mc_viewer.DrawAllClusters();

    print "Event ID is: " ,counter
    counter+=1
    print "    Hit enter to go next event..."
    sys.stdin.readline()


# done!
