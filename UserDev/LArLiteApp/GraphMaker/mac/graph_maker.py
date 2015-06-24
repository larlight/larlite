import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from seltool import ertool
from ROOT import geotree
from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("treeMaker.root")

# prepare GraphMaker manager
mgr = geotree.Manager()
# add conflict-resolving algos
mgr.setAlgoMultipleParents(geotree.AlgoMultipleParentsHighScore())
mgr.setAlgoGenericConflict(geotree.AlgoGenericConflictRemoveSibling())
mgr.setAlgoParentIsSiblingsSibling(geotree.AlgoParentIsSiblingsSibling())
mgr.setLoose(True)

my_algo = ertool.AlgoMakeGraph()
my_algo.setManager(mgr)
my_algo.setVerbose(False)
my_anaunit = fmwk.ExampleERSelection()
#### SET PRODUCERS HERE ####
# MC PRODUCERS
my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetTrackProducer(True,"mcreco");
# RECO PRODUCERS
#my_ana.SetShowerProducer(False,"showerreco");
#my_ana.SetTrackProducer(False,"");
#############################
my_anaunit._mgr.AddAlgo(my_algo)
my_anaunit._mgr._mc_for_ana = True
my_proc.add_process(my_anaunit)
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

