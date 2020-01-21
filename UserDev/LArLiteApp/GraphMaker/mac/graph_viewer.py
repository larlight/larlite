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
from seltool.erviewer import ERViewer
from seltool.algoviewer import viewAll

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in range(len(sys.argv)-1):
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
my_algo.setVerbose(True)
my_anaunit = fmwk.ExampleERSelection()
#### SET PRODUCERS HERE ####
# MC PRODUCERS
my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetTrackProducer(True,"mcreco");
# RECO PRODUCERS
#my_anaunit.SetShowerProducer(False,"showerreco");
#my_anaunit.SetTrackProducer(False,"");
#############################
my_anaunit._mgr.AddAlgo(my_algo)
my_anaunit._mgr._mc_for_ana = True
my_proc.add_process(my_anaunit)
#my_proc.run()
#sys.exit(0)

mcviewer   = ERViewer('MC Info')
recoviewer = ERViewer('RECO Info')

counter = 0
while (counter < 1000):
    try:
        counter = eval(input('Hit Enter to continue to next evt, or type in an event number to jump to that event:'))
    except SyntaxError:
        counter = counter + 1
    my_proc.process_event(counter)
    data_reco = my_anaunit.GetData()
    part_reco = my_anaunit.GetParticles()
    data_mc   = my_anaunit.GetData(True)
    part_mc   = my_anaunit.GetParticles(True)
    viewAll(mcviewer,data_mc,part_mc,recoviewer,data_reco,part_reco)
    print(counter)

# done!
print()
print("Finished running ana_processor event loop!")
print()

sys.exit(0)

