import sys, os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import ertool
ertool.Manager

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("singleE_selection_ana_out.root")


# Possible filter to select true events
#my_proc.enable_filter(True)
#pdgsel = fmwk.PDGSelection()
#pdgsel.Select(11,pdgsel.kGENERATOR,1)
#my_proc.add_process(pdgsel)

my_algo = ertool.AlgoSingleE()
my_algo.Reset()
my_algo.setVerbose(True)
my_ana = fmwk.ExampleERSelection()


# Set Producers
# First Argument: True = MC, False = Reco
#my_ana.SetShowerProducer(True,"mcreco");
#my_ana.SetTrackProducer(True,"mcreco");
#my_ana.SetVtxProducer(True,"generator");

#my_ana.SetShowerProducer(False,"mergeall");
my_ana.SetShowerProducer(False,"showerreco");
my_ana.SetTrackProducer(False,"");
my_ana.SetVtxProducer(False,"");

my_ana._mgr.SetAlgo(my_algo)

my_ana._mode =True # True = Select. False = Fill mode
my_proc.add_process(my_ana)

counter = 0
while (counter < 1000):
    try:
        counter = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        counter += 1
    my_proc.process_event(counter)

#my_proc.run()


my_algo.Reset()



# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

