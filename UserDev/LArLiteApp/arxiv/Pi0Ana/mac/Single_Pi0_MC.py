import sys, os
from ROOT import gSystem
gSystem.Load("libLArLite_Analysis")
from ROOT import larlite as fmwk
fmwk.ana_processor
gSystem.Load("libLArLiteApp_Pi0Ana")
#from ROOT import ertool
#from ROOT import geoalgo
from seltool import ertool
#ertool.Manager


if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

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
#my_algo.setMinShrEnergy(10)
my_algo.setMinShrEnergy(0)
my_algo.setMaxShrEnergy(1000)
my_algo.setIPMax(10)
my_algo.setMinFitMass(50)
my_algo.setMaxFitMass(200)
my_algo.setAngleMax(3.14)




# Create filter
my_anaunit = fmwk.Pi0Reco()#ExampleERSelection()
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Set up ana for 1pi0 selection
my_ana = ertool.ERAna1PI0()


my_anaunit._mgr.AddAna(my_ana)
my_anaunit._mgr.AddAlgo(my_algo)
#my_anaunit._mgr.AddCfgFile("/home/ryan/TESTERMOD/UserDev/LArLiteApp/ERToolBackend/mac/new_empart.txt")
my_anaunit._mgr._training_mode =False


# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("singlepi0_mc.root")

# here set E-cut for Helper & Ana modules
my_anaunit._mgr._mc_for_ana = True

# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
#my_anaunit.SetShowerProducer(True,"mcreco");
#my_anaunit.SetShowerProducer(False,"newdefaultreco");
#my_anaunit.SetShowerProducer(False,"pandoraNuShower");
#my_anaunit.SetShowerProducer(False,"mergeall");
#my_anaunit.SetTrackProducer(True,"mcreco");
#my_anaunit.SetTrackProducer(False,"stitchkalmanhit");
#my_anaunit.SetVtxProducer(True,"generator");
#my_anaunit.SetShowerProducer(False,"showerrecofuzzy");
#my_anaunit.SetShowerProducer(False,"showerreco");

my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetTrackProducer(True,"mcreco");
#my_anaunit.SetVtxProducer(True,"generator");

#my_anaunit.SetShowerProducer(False,"showerrecofuzzy");
#my_anaunit.SetTrackProducer(False,"");
#my_anaunit.SetVtxProducer(False,"");
# ************************************************

my_proc.add_process(my_anaunit)

my_proc.run()


# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

