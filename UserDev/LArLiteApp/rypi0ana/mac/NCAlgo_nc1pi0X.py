import sys, os
from ROOT import larlite as fmwk
from seltool import ertool


if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

# Create algorithm

#====Pi0=====
algo_pi0 = ertool.ERAlgoNCPi0()
algo_pi0.setMinShrEnergy(50)
algo_pi0.setFidXmin(27)
algo_pi0.setFidXmax(27)
algo_pi0.setFidYmin(27)
algo_pi0.setFidYmax(27)
algo_pi0.setFidZmin(27)
algo_pi0.setFidZmax(27)
algo_pi0.setMinOpeningAngle(0.3)
algo_pi0.setPi0DirYMin(-0.9)
algo_pi0.setMaxEnergyAsy(0.8)


#==NC== Finder 
algo_ncpi0 = ertool.AlgoNCPi0Finder()
algo_ncpi0.SetVtxSphere_Radius(36)
algo_ncpi0.SetMaxTrackLength(1000000)

# Create MC filter
my_anaunit = fmwk.Pi0Reco()#ExampleERSelection()
my_anaunit.SetMinEDep(50.)# Sets min shower to be seen in an event
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)
my_proc.enable_event_alignment(False)


#Set up the configure params for the type of reco/mc
my_anaunit._mgr.ClearCfgFile()
my_anaunit._mgr.AddCfgFile('/home/ryan/LARLITE/trunk/UserDev/SelectionTool/ERTool/dat/ertool_default_mc.cfg')

# Set up ana for 1pi0 selection
my_ana = ertool.EREffCorrectAna()

#Here is a filter
pi0_topoMC = fmwk.MCTopo_BG_nc1pi0X();
pi0_topoMC.SetShowerEnergy(0)
pi0_topoMC.SetDetShowerEnergy(0.)
pi0_topoMC.SetFiducial(27.)


my_anaunit._mgr.AddAlgo(algo_pi0)
my_anaunit._mgr.AddAlgo(algo_ncpi0)
my_anaunit._mgr.AddAna(my_ana)
my_anaunit._mgr._training_mode =False


# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("bg_ncpi0x.root")



# here set E-cut for Helper & Ana modules
my_anaunit._mgr._mc_for_ana = True

# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetTrackProducer(True,"mcreco");
# ************************************************

my_proc.add_process(pi0_topoMC)
my_proc.add_process(my_anaunit)


#my_proc.run(0,100)
my_proc.run()


# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

