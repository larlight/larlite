import sys, os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

if sys.argv[1] not in ['reco','mc']:
	msg = '\n'
	msg += 'Specify if you want to use "reco" or "mc" quantities in your first argument to this script!'
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

use_reco = True if sys.argv[1] == 'reco' else False

# Set input root file
for x in xrange(len(sys.argv)-2):
    my_proc.add_input_file(sys.argv[x+2])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("output/70KV/%s/singleE_dirt_selection_%s.root"%(('actual_reco' if use_reco else 'perfect_reco'),('reco' if use_reco else 'mc')))

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
Ecut = 50 # in MeV

#dirt 
dirtfilter = fmwk.MC_dirt_Filter()

dirt_ana = ertool.ERAnaLowEExcess()
dirt_ana.SetTreeName("dirt")
#dirt_ana.SetDebug(False)
dirt_ana.SetECut(Ecut)

dirt_anaunit = fmwk.ExampleERSelection()
dirt_anaunit._mgr.ClearCfgFile()
dirt_anaunit._mgr.AddCfgFile('/uboone/app/users/kaleko/larlite/UserDev/SelectionTool/ERTool/dat/ertool_default%s.cfg'%('_reco' if use_reco else ''))

if use_reco:
    dirt_anaunit.SetShowerProducer(False,'showerrecofuzzy')
    dirt_anaunit.SetTrackProducer(False,'stitchkalmanhitcc')
else:
    dirt_anaunit.SetShowerProducer(True,'mcreco')
    dirt_anaunit.SetTrackProducer(True,'mcreco')

dirt_anaunit._mgr.AddAlgo(cos_algo)
dirt_anaunit._mgr.AddAlgo(cosmicprimary_algo)
dirt_anaunit._mgr.AddAlgo(cosmicsecondary_algo)
dirt_anaunit._mgr.AddAlgo(cosmicorphanalgo)
dirt_anaunit._mgr.AddAlgo(primary_algo)
dirt_anaunit._mgr.AddAlgo(pid_algo)
dirt_anaunit._mgr.AddAlgo(my_algo)
dirt_anaunit._mgr.AddAna(nue_ana)
dirt_anaunit._mgr._profile_mode = True

dirt_anaunit.SetMinEDep(Ecut)
dirt_anaunit._mgr._mc_for_ana = True


# Add MC filter and analysis unit
# to the process to be run

my_proc.add_process(dirtfilter)
my_proc.add_process(dirt_anaunit)
my_proc.run()
# my_proc.run(0,500)

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

