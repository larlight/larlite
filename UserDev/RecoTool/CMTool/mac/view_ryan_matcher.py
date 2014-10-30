# Load libraries
import ROOT, sys, os
from ROOT import *

# Now import ana_processor & your class. For this example, ana_base.
gSystem.Load("libCMTool")
from ROOT import *
if len(sys.argv) != 2:
#if not len(sys.argv) in [2,3]:
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

match_viewer.SetPrintClusterInfo(True)

#Show Showers: requires MC info in hadded files
match_viewer.SetDrawShowers(False)

########################################
# attach match algos here
########################################

#prio_algo = cmtool.CPAlgoNHits()
#match_viewer.GetManager().AddPriorityAlgo(prio_algo)


#myalg = cmtool.CFAlgoStartPointCompat()



#myalg = cmtool.CFAlgoTimeOverlap()
myalg = cmtool.CFAlgoTimeProf()
myalg.SetVerbose(True)

match_viewer.GetManager().AddMatchAlgo(myalg)

########################################
# done attaching match algos
########################################

my_proc.add_process(raw_viewer)

my_proc.add_process(match_viewer)

raw_viewer.SetClusterType(larlight.DATA.RyanCluster)

match_viewer.SetClusterType(larlight.DATA.RyanCluster)

gStyle.SetOptStat(0)

#start on first event always
user_input_evt_no = -1;

while true:

    try:
        user_input_evt_no = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        #user_input_evt_no = user_input_evt_no + 1
        user_input_evt_no = user_input_evt_no + 12

    try:
        my_proc.process_event(user_input_evt_no)

        raw_viewer.DrawAllClusters();

        match_viewer.DrawAllClusters();

        bk = match_viewer.GetManager().GetBookKeeper()

        res = bk.GetResult()

        print "\033[93m","Found %d matched pairs!","\033[00m"
        for x in xrange(res.size()):

            print 'Pair',x,
            for y in xrange(res.at(x).size()):

                print res.at(x).at(y),

            print

        print "    Hit enter to go next event..."
    except larutil.LArUtilException:
        print "skipping"
    sys.stdin.readline()


# done!
