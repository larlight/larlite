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

# open file on which to write values
#myfile = open("misid_vs_posRes.txt","a")
#posres = float(sys.argv[-1])
#print "Position resolution set to: ", posres
#myfile.write(str(posres)+"\t")


# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Create algorithm
my_algo = ertool.AlgoPi0()
my_algo.setVerbose(False)
my_algo.setMinShrEnergy(10)
my_algo.setMaxShrEnergy(1000)
my_algo.setIPMax(10)
my_algo.setMinFitMass(50)
my_algo.setMaxFitMass(200)
my_algo.setAngleMax(3.14)
# Create filter
#my_filter = ertool.FilterECut()
#my_filter.SetECut(10) #cut on 10 MeV
# Create analysis unit
my_anaunit = fmwk.Pi0Reco()#ExampleERSelection()
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

my_ana = ertool.ERAnaPi0Selection()#ExampleERSelection()
my_ana.SetDebug(True)

# Create Pi0 Filter, set energy containment requirement, if you wish
pi0_filter = fmwk.Pi0ContainmentFilter();
pi0_filter.SetEC(0.5)
pi0_filter.SetEnergyCut(0.)

my_anaunit._mgr.SetAna(my_ana)
#my_anaunit._mgr.SetFilter(my_filter)
my_anaunit._mgr.SetAlgo(my_algo)
my_anaunit._mgr._training_mode =False


# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("selection.root")

# here set E-cut for Helper & Ana modules
my_anaunit._mgr._mc_for_ana = True

# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
my_anaunit.SetShowerProducer(True,"mcreco");
#my_anaunit.SetShowerProducer(False,"newdefaultreco");
#my_anaunit.SetShowerProducer(False,"pandoraNuShower");
#my_anaunit.SetShowerProducer(False,"mergeall");
#my_anaunit.SetTrackProducer(True,"mcreco");
#my_anaunit.SetTrackProducer(False,"stitchkalmanhit");
#my_anaunit.SetVtxProducer(True,"generator");
#my_anaunit.SetShowerProducer(False,"showerreco");
my_anaunit.SetTrackProducer(False,"");
my_anaunit.SetVtxProducer(False,"");
# ************************************************

my_proc.add_process(pi0_filter)
my_proc.add_process(my_anaunit)

my_proc.run()

# save efficiency to file
#eff = my_ana.getEfficiency()
#myfile.write(str(eff)+"\n")

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

