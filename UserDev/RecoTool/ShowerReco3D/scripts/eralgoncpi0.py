import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from seltool import ertool
from larlite import larlite as fmwk
from recotool import showerreco

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

my_proc.set_io_mode(fmwk.storage_manager.kREAD)

my_proc.set_ana_output_file("ertool_hist.root")

algo_pi0 = ertool.ERAlgoNCPi0()
algo_pi0.setMinShrEnergy(50.)
algo_pi0.setMinOpeningAngle(0.3)
algo_pi0.setMaxEnergyAsy(0.8)
algo_pi0.setMaxIP(10)

#my_ana = ertool.ERAnaPi0Chain()
#my_ana._mode =True # True = Select. False = Fill mode
# Create larlite interfce analysis unit for ERTool
my_anaunit = fmwk.ExampleERSelection()

# Set Producers
# First Argument: True = MC, False = Reco
# Second Argument: producer module label
my_anaunit.SetShowerProducer(False,"showerreco");
my_anaunit.SetTrackProducer(False,"");

# Implement manager
my_anaunit._mgr.AddAlgo(algo_pi0)
#my_anaunit._mgr.AddAna(my_ana)
my_proc.add_process(my_anaunit)

# run!
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

