import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)
from seltool import ertool
from larlite import larlite as fmwk

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

my_ana = ertool.ERAnaCCSingleMu()

my_anaunit = fmwk.CCSingleMuReco()
# Set Producers
# First Argument: True = MC, False = Reco
my_anaunit.SetShowerProducer(True,"mcreco");
#my_ana.SetTrackProducer(True,"mcreco");
# my_anaunit.SetTrackProducer(False,"trackkalmanhit");
#my_ana.SetTrackProducer(False,"pandoraNuKHit");
my_anaunit.SetTrackProducer(False,"pandoraCosmicKHit");
my_anaunit.SetVtxProducer(True,"generator");



#my_ana.SetShowerProducer(False,"showerreco");
#my_ana.SetTrackProducer(False,"");
#my_ana.SetVtxProducer(False,"");

# Attach ERTool wrapper
my_anaunit._mgr.SetAlgo(my_algo)
my_anaunit._mgr.SetFilter(my_filter)
my_anaunit._mgr.SetAna(my_ana)
my_anaunit._mgr._mc_for_ana=True
my_proc.add_process(my_anaunit)

# run
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

