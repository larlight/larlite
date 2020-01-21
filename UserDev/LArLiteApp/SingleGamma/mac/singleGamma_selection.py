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

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Create algorithm
my_algo = ertool.AlgoSingleGamma()
my_algo.useRadLength(True)
my_algo.setVerbose(True)
#my_algo.setRejectLongTracks(False)
my_algo.setVtxToTrkStartDist(1)  #1
my_algo.setVtxToTrkDist(1)	 #1
my_algo.setVtxToShrStartDist(50) #50
my_algo.setMaxIP(1)		 #1
my_algo.setVtxProximityCut(5)	 #5
my_algo.setEThreshold(0.)	 #100
#my_algo.setBDtW(10)
#my_algo.setBDtTW(10)

# Create MC Filter
#MCfilter = fmwk.MC_NCNGamma_Filter();
#MCfilter.setMaxNEvents(1000);
#MCfilter.flip(False)

# Set input root file
for x in range(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("singleGamma_selection.root")

# here set E-cut for Helper & Ana modules
#This cut is applied in helper... ertool showers are not made if the energy of mcshower or reco shower
#is below this threshold. This has to be above 0 or else the code may segfault. This is not a "physics cut".
#Do not change this value unless you know what you are doing.
Ecut = 20 # in MeV

my_ana = ertool.ERAnaSingleE()
#my_ana.SetDebug(False)
#my_ana.SetECut(Ecut)

my_anaunit = fmwk.ExampleERSelection()
my_anaunit._mgr.AddAlgo(my_algo)
#my_anaunit._mgr.SetFilter(my_filter)
my_anaunit._mgr.AddAna(my_ana)

#This cut is applied in helper... ertool showers are not made if the energy of mcshower or reco shower
#is below this threshold. This has to be above 0 or else the code may segfault. This is not a "physics cut".
#Do not change this value unless you know what you are doing.
my_anaunit.SetMinEDep(Ecut)
my_anaunit._mgr._mc_for_ana = True

# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
my_anaunit.SetShowerProducer(True,"mcreco");
#my_anaunit.SetShowerProducer(False,"showerreco");
#my_anaunit.SetShowerProducer(False,"newdefaultreco");
#my_anaunit.SetShowerProducer(False,"pandoraNuShower");
#my_anaunit.SetShowerProducer(False,"mergeall");
my_anaunit.SetTrackProducer(True,"mcreco");
#my_anaunit.SetTrackProducer(False,"stitchkalmanhit");
#my_anaunit.SetVtxProducer(True,"generator");
# ************************************************
#my_proc.add_process(MCfilter)
my_proc.add_process(my_anaunit)

my_proc.run()

# save efficiency to file
#eff = my_ana.getEfficiency()
#myfile.write(str(eff)+"\n")

# done!
print()
print("Finished running ana_processor event loop!")
print()

#my_algo.StoreParams()
sys.exit(0)

