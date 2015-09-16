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
#ertool.Manager

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# track PID algorithm
pid_algo = ertool.ERAlgoTrackPid()
pid_algo.setVerbose(False)

# primary finder algorithm
primary_algo = ertool.ERAlgoCRPrimary()
secondary_algo = ertool.ERAlgoCRSecondary()
orphan = ertool.ERAlgoCROrphan()
fmatch = ertool.ERAlgoFlashMatch()

orphan_algo = ertool.ERAlgoCROrphan()

flash_algo  = ertool.ERAlgoFlashMatch() 

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("cosmic_trees.root")

Ecut = 20 # in MeV

my_ana = ertool.ERAnaCRTagger()

my_anaunit = fmwk.ExampleERSelection()
my_anaunit._mgr.AddAlgo(pid_algo)
my_anaunit._mgr.AddAlgo(primary_algo)
my_anaunit._mgr.AddAlgo(secondary_algo)
my_anaunit._mgr.AddAlgo(orphan)
my_anaunit._mgr.AddAlgo(fmatch)
my_anaunit._mgr.AddAna(my_ana)
my_anaunit._mgr._verbosity = 0
#my_anaunit._mgr.AddCfgFile('new_empart.txt')
my_anaunit.SetMinEDep(Ecut)
my_anaunit._mgr._mc_for_ana = False

# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
#my_anaunit.SetShowerProducer(True,"")
my_anaunit.SetShowerProducer(False,"")
#my_anaunit.SetShowerProducer(False,"showerrecopandora")
#my_anaunit.SetShowerProducer(False,"newdefaultreco");
#my_anaunit.SetShowerProducer(False,"pandoraNuShower");
#my_anaunit.SetShowerProducer(False,"mergeall");
#my_anaunit.SetTrackProducer(True,"mcreco");
my_anaunit.SetTrackProducer(False,"trackkalmanhit");
#my_anaunit.SetTrackProducer(False,"costrk");
#my_anaunit.SetVtxProducer(True,"generator");
# ************************************************

my_proc.add_process(my_anaunit)

my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

