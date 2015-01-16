import sys

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
my_proc.set_ana_output_file("PEselection_ana_out.root")


# Possible filter to select true events
#my_proc.enable_filter(True)
#pdgsel = fmwk.PDGSelection()
#pdgsel.Select(11,pdgsel.kGENERATOR,1)
#my_proc.add_process(pdgsel)

my_algo = ertool.AlgoLonelyE()
my_algo.LoadParams()
my_algo.Reset()
my_algo.setDebug(True)

my_filter = ertool.AFilterECut()
my_filter.SetECut(40.) # MeV

my_ana = fmwk.ExampleERSelection()
# OPTIONAL:
my_ana.SetShowerProducer(False,"showerreco");
my_ana.SetTrackProducer(False,"stitchkalmanhit");
#my_ana.SetShowerProducer(True,"mcreco");
#my_ana.SetTrackProducer(True,"mcreco");
my_ana.SetVtxProducer(False,"");
#my_ana.RecoProducer("","showerreco") # call if using Reco objects
#my_ana.AddGeneratorProducer("generator") # call if MC vertex info should be used (for rad-length to be used w/ Reco, for example)
my_ana._mgr.SetAlgo(my_algo)
my_ana._mgr.SetFilter(my_filter)
my_ana._mode =True # True = Select. False = Fill mode
my_proc.add_process(my_ana)
my_proc.run()


my_algo.Reset()



# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

