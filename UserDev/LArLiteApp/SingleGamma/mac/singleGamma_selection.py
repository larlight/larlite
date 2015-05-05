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
my_algo = ertool.AlgoSingleE()
my_algo.useRadLength(True)
my_algo.setVerbose(False)
my_algo.setRejectLongTracks(True)
my_algo.setVtxToTrkStartDist(1)  #1
my_algo.setVtxToTrkDist(1)	 #1
my_algo.setVtxToShrStartDist(50) #50
my_algo.setMaxIP(1)		 #1
my_algo.setVtxProximityCut(5)	 #5
my_algo.setEThreshold(0.)	 #100
#my_algo.setBDtW(10)
#my_algo.setBDtTW(10)
#my_algo.LoadParams()
# Create ERTool filter
my_filter = ertool.FilterTrackLength()
my_filter.setLengthCut(0.3)


# Create MC Filter
# This filter is if you want to look at CC1E events
#MCfilter = fmwk.MC_CC1E_Filter();
# Filter for looking at NC1Gamma
MCfilter = fmwk.MC_NC1Gamma_Filter();

#Set flip to FALSE if you are looking for efficiency, TRUE if you are looking for MID efficiency
MCfilter.flip(False)
#MCfilter.flip(True)
#MCfilter.SetFilterEnergy(.02) 
# Use this filter instead if you want to look at CCQE channel events with specified number of mcshowers and
# mctracks above specified energies coming from the neutrino
#MCfilter = fmwk.MC_CCQE_Filter();
#MCfilter.set_n_mcshowers(1)
#MCfilter.set_min_mcshower_E(50.)
#MCfilter.set_n_mctracks(0)
#MCfilter.set_min_mctrack_E(10.)


# Set input root file
for x in xrange(len(sys.argv)-1):
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
my_ana.SetDebug(False)
my_ana.SetECut(Ecut)

my_anaunit = fmwk.ExampleERSelection()
my_anaunit._mgr.SetAlgo(my_algo)
my_anaunit._mgr.SetFilter(my_filter)
my_anaunit._mgr.SetAna(my_ana)
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
my_proc.add_process(MCfilter)
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
