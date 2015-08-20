# Load libraries
import ROOT, sys, os
from ROOT import *
gSystem.Load('libRecoTool_CMToolApp.so')
# Now import ana_processor & your class. For this example, ana_base.
if len(sys.argv) < 2:
    print
    print "*** Improper usage. Usage: python %s /path/to/input/file.root ***" % sys.argv[0]
    print


my_proc = larlite.ana_processor()
my_proc.set_verbosity(larlite.msg.kDEBUG)

my_proc.set_io_mode(larlite.storage_manager.kREAD)

#args should be input file name
for x in xrange(len(sys.argv)-1):

    my_proc.add_input_file(sys.argv[x+1])

my_proc.set_ana_output_file("")

raw_viewer   = larlite.ClusterViewer()

my_proc.add_process(raw_viewer)

raw_viewer.SetClusterProducer("fuzzycluster")
#raw_viewer.SetClusterProducer("michel")

gStyle.SetOptStat(0)

#start on first event always
user_input_evt_no = 0;

while True:

    try:
        user_input_evt_no = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        user_input_evt_no = user_input_evt_no + 1

    my_proc.process_event(user_input_evt_no)

    #raw_viewer.DrawAllClusters();


    plane = 2
    for cindex in xrange(raw_viewer.ClusterCount(plane)):

       print "        Cluster:",cindex
       raw_viewer.DrawOneCluster(plane,cindex)
       sys.stdin.readline()


    print "Just showed Event %d. Hit enter to go next event..." % user_input_evt_no
    sys.stdin.readline()


# done!
