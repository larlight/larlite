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
my_proc.enable_filter(True)

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("trial.root")


#Setup EVENT Filter
MCfilter = fmwk.MC_CCnumu_Filter();#MC_CC1E_Filter();
MCfilter.flip(False)


##############################################
#    Setup Analaysis - MC vs Selected Objects
##############################################

# Create algorithm
my_algo = ertool.AlgoSingleE()
my_algo.useEMPart(True)
my_algo.setVerbose(False)
my_algo.setVtxToTrkStartDist(1)
my_algo.setVtxToTrkDist(1)
my_algo.setVtxToShrStartDist(50)
my_algo.setMaxIP(1)
#my_algo.setVerbose(True)

# Create Analysis
my_ana = ertool.ERAnanumuCC_MisID()

#Build Analysis Unit
my_anaunit = fmwk.ExampleERSelection()
my_anaunit._mgr.SetAlgo(my_algo)
my_anaunit._mgr.SetAna(my_ana)
my_anaunit._mgr._mc_for_ana = True

# Set Producers
# First Argument: True = MC, False = Reco
# Second Argument: producer module label
my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetTrackProducer(True,"mcreco");
my_anaunit.SetVtxProducer(True,"generator");

#Setup Process Order
my_proc.add_process(MCfilter)
my_proc.add_process(my_anaunit)

# run!
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

