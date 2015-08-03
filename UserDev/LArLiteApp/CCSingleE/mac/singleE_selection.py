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
from seltool.ccsingleeDef import GetCCSingleEInstance
#ertool.Manager

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Get Default CCSingleE Algorithm instance
# this sets default parameters
# this information is loaded from:
# $LARLITE_BASEDIR/python/seltool/GetCCSingleEInstance
# and the algorithm instance is the return of the
# function GetCCSingleEInstance()
my_algo = GetCCSingleEInstance()
my_algo.setVerbose(False)

# primary finder algorithm
primary_algo = ertool.AlgoPrimaryFinder()
primary_algo.setVtxToTrkStartDist(1)
primary_algo.setVtxToTrkDist(1)
primary_algo.setVtxToShrStartDist(50)
primary_algo.setMaxIP(1)

# track PID algorithm
pid_algo = ertool.ERAlgoTrackPid()
pid_algo.setVerbose(False)

# Create ERTool filter
# This filter removes any track that
# is less than 3 mm in length
# these tracks exist in "perfect reco"
# but at this stage it is unreasonable
# to assume we will be able to
# reconstruct them

# Create MC Filter
# This filter is if you want to look at CC1E events
MCfilter = fmwk.MC_CC1E_Filter();
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
my_proc.set_ana_output_file("singleE_selection.root")

# here set E-cut for Helper & Ana modules
#This cut is applied in helper... ertool showers are not made if the energy of mcshower or reco shower
#is below this threshold. This has to be above 0 or else the code may segfault. This is not a "physics cut".
#Do not change this value unless you know what you are doing.
Ecut = 20 # in MeV

my_ana = ertool.ERAnaSingleE()
my_ana.SetDebug(False)
my_ana.SetECut(Ecut)

my_anaunit = fmwk.ExampleERSelection()
my_anaunit._mgr.AddAlgo(primary_algo)
my_anaunit._mgr.AddAlgo(pid_algo)
my_anaunit._mgr.AddAlgo(my_algo)
my_anaunit._mgr.AddAna(my_ana)
#my_anaunit._mgr.AddCfgFile('new_empart.txt')
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
#my_anaunit.SetTrackProducer(False,"costrk");
#my_anaunit.SetVtxProducer(True,"generator");
# ************************************************

# Add MC filter and analysis unit
# to the process to be run
#my_proc.add_process(MCfilter)
my_proc.add_process(my_anaunit)

my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

