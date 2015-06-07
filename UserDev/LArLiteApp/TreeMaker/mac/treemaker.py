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
#from algoviewer import view, setDisplay

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("treeMaker.root")

my_algo = ertool.AlgoMakeTree()
my_algo.setVerbose(True)
my_anaunit = fmwk.ExampleERSelection()
#### SET PRODUCERS HERE ####
# MC PRODUCERS
my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetTrackProducer(True,"mcreco");
# RECO PRODUCERS
#my_ana.SetShowerProducer(False,"showerreco");
#my_ana.SetTrackProducer(False,"");
#############################
my_anaunit._mgr.SetAlgo(my_algo)
my_proc.add_process(my_anaunit)
my_proc.run()
sys.exit(0)

#create display
display = setDisplay("TreeMaker")

counter = 0
while (counter < 1000):
    try:
        counter = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        counter = counter + 1
    my_proc.process_event(counter)
    part_mc   = my_ana.GetParticles(True)
    data_mc   = my_ana.GetData(True)
    view(data_mc,part_mc,display)
    for x in xrange(part_mc.size()):
        print part_mc[x].Diagram()
    print counter

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

