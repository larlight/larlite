import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import ertool
ertool.Manager()
from ROOT import larlite as fmwk
fmwk.geo.PlaneID


# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("CCSingleMuReco_hist.root")

my_algo = ertool.AlgoCCSingleMu()
my_algo.Reset()

my_filter = ertool.FilterFidVolume()

my_ana = fmwk.CCSingleMuReco()
# Set Producers
# First Argument: True = MC, False = Reco
my_ana.SetShowerProducer(True,"mcreco");
my_ana.SetTrackProducer(True,"mcreco");
my_ana.SetVtxProducer(True,"generator");

#my_ana.SetShowerProducer(False,"showerreco");
#my_ana.SetTrackProducer(False,"");
#my_ana.SetVtxProducer(False,"");

my_ana._mgr.SetAlgo(my_algo)
my_ana._mgr.SetFilter(my_filter)
my_proc.add_process(my_ana)
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

