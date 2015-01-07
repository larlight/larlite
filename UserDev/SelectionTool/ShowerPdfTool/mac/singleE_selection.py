import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import sptool

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

my_algo = sptool.SPAlgoSingleE()
my_algo.LoadParams()
my_algo.Reset()
my_ana = fmwk.ExampleSPSelection()
# OPTIONAL:
#my_ana.RecoProducer("","showerreco") # call if using Reco objects
#my_ana.AddGeneratorProducer("generator") # call if MC vertex info should be used (for rad-length to be used w/ Reco, for example)
my_ana._mgr.SetSPAlgo(my_algo)
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

