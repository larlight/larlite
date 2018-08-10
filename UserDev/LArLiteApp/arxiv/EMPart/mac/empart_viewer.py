import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from seltool import ertool
from larlite import larlite as fmwk
from seltool.erviewer import ERViewer
from seltool.algoviewer import viewAll
import empartDef

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("EMPartSelection.root")

my_algo = empartDef.EMPartAlgo()
my_ana = empartDef.EMPartAna()
my_anaunit = empartDef.AnaUnit()

my_anaunit._mgr.AddAlgo(my_algo)
my_anaunit._mgr.AddAna(my_ana)
my_proc.add_process(my_anaunit)

mcviewer   = ERViewer('MC Info')
recoviewer = ERViewer('RECO Info')

# start event-by-event loop
counter = 0
while (my_proc.process_event(counter)):
    print "Processing event {0}".format(counter) 
    data_reco = my_anaunit.GetData()
    part_reco = my_anaunit.GetParticles()
    data_mc   = my_anaunit.GetData(True)
    part_mc   = my_anaunit.GetParticles(True)
    viewAll(mcviewer, data_mc, part_mc,
            recoviewer, data_reco, part_reco)
    try:
        counter = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        counter = counter + 1

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

