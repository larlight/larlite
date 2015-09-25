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
from seltool.primaryfinderDef import GetPrimaryFinderInstance
from seltool.trackpidDef import GetTrackPidInstance
from seltool.cosmictaggerDef import GetCosmicTaggerInstance
from seltool.primarycosmicDef import GetPrimaryCosmicFinderInstance

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("output/singleE_nc_gamma_selection.root")

# Get Default CCSingleE Algorithm instance
# this information is loaded from:
# $LARLITE_BASEDIR/python/seltool/GetCCSingleEInstance
my_algo = GetCCSingleEInstance()
#my_algo.setVerbose(False)

# primary finder algorithm
# this information is loaded from:
# $LARLITE_BASEDIR/python/seltool/GetPrimaryFinderInstance
primary_algo = GetPrimaryFinderInstance()

# primary cosmic algoithm 
# this information is loaded from:
# $LARLITE_BASEDIR/python/seltool/primarycosmicDef.py
cosmicprimary_algo = GetPrimaryCosmicFinderInstance()
cosmicsecondary_algo = ertool.ERAlgoCRSecondary()
cosmicorphanalgo = ertool.ERAlgoCROrphan()
# track PID algorithm
# this information is loaded from:
# $LARLITE_BASEDIR/python/seltool/GetTrackPidInstance
pid_algo = GetTrackPidInstance()
#pid_algo.setVerbose(False)

# cosmic tagger algo
cos_algo = GetCosmicTaggerInstance()
#cos_algo.setVerbose(False)

# here set E-cut for Helper & Ana modules
#This cut is applied in helper... ertool showers are not made if the energy of mcshower or reco shower
#is below this threshold. This has to be above 0 or else the code may segfault. This is not a "physics cut".
#Do not change this value unless you know what you are doing.
Ecut = 20 # in MeV

# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
#my_anaunit.SetShowerProducer(True,"mcreco");
#my_anaunit.SetShowerProducer(False,"showerreco");
#my_anaunit.SetShowerProducer(False,"newdefaultreco");
#my_anaunit.SetShowerProducer(False,"pandoraNuShower");
#my_anaunit.SetShowerProducer(False,"mergeall");

#my_anaunit.SetTrackProducer(True,"mcreco");
#my_anaunit.SetTrackProducer(False,"stitchkalmanhit");
#my_anaunit.SetTrackProducer(False,"costrk");
#my_anaunit.SetVtxProducer(True,"generator");
# ************************************************

#NC beam
ncfilter = fmwk.MC_NC_gamma_Filter()

nc_ana = ertool.ERAnaLowEExcess()
nc_ana.SetTreeName("beamNC")
#nc_ana.SetDebug(False)
nc_ana.SetECut(Ecut)

nc_anaunit = fmwk.ExampleERSelection()
nc_anaunit._mgr.AddAlgo(cos_algo)
nc_anaunit._mgr.AddAlgo(cosmicprimary_algo)
nc_anaunit._mgr.AddAlgo(cosmicsecondary_algo)
nc_anaunit._mgr.AddAlgo(cosmicorphanalgo)
nc_anaunit._mgr.AddAlgo(primary_algo)
nc_anaunit._mgr.AddAlgo(pid_algo)
nc_anaunit._mgr.AddAlgo(my_algo)
nc_anaunit._mgr.AddAna(nc_ana)
nc_anaunit._mgr._profile_mode = True

nc_anaunit.SetMinEDep(Ecut)
nc_anaunit._mgr._mc_for_ana = True
nc_anaunit.SetShowerProducer(True,"mcreco")
nc_anaunit.SetTrackProducer(True,"mcreco")

# Add MC filter and analysis unit
# to the process to be run

#my_proc.add_process(nuefilter)
#my_proc.add_process(nue_anaunit)
#my_proc.add_process(numufilter)
#my_proc.add_process(numu_anaunit)
my_proc.add_process(ncfilter)
my_proc.add_process(nc_anaunit)
#my_proc.add_process(Cosfilter)
#my_proc.add_process(cos_anaunit)
#my_proc.add_process(dirtfilter)
#my_proc.add_process(dirt_anaunit)
my_proc.run()
#my_proc.run(0,500)

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

