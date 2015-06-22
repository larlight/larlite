import sys, os
from ROOT import gSystem
from ROOT import ertool
from ROOT import larlite as fmwk
from seltool.ccsingleeDef import GetCCSingleEInstance
from seltool.algoviewer import viewAll

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

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
my_algo.setVerbose(True)

# Create MC Filter
# This filter is if you want to look at CC1E events
MCfilter = fmwk.MC_CC1E_Filter();
#Set flip to FALSE if you are looking for efficiency, TRUE if you are looking for MID efficiency
MCfilter.flip(False)
#MCfilter.flip(True)

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
my_ana.SetDebug(True)
my_ana.SetECut(Ecut)

my_anaunit = fmwk.ExampleERSelection()
my_anaunit._mgr.AddAlgo(my_algo)
my_anaunit._mgr.AddAna(my_ana)
my_anaunit.SetMinEDep(Ecut)
my_anaunit._mgr._mc_for_ana = True
# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
#my_anaunit.SetShowerProducer(True,"mcreco");
#my_anaunit.SetShowerProducer(False,"davidreco");
#my_anaunit.SetShowerProducer(False,"newdefaultreco");
#my_anaunit.SetShowerProducer(False,"pandoraNuShower");
#my_anaunit.SetShowerProducer(False,"mergeall");
#my_anaunit.SetShowerProducer(False,"showerreco");
my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetTrackProducer(True,"mcreco");
#my_anaunit.SetTrackProducer(False,"stitchkalmanhit");
#my_anaunit.SetVtxProducer(True,"generator");
# ************************************************
my_proc.add_process(MCfilter)
my_proc.add_process(my_anaunit)


# Start event-by-event loop
counter = 0
while (my_proc.process_event(counter)):
    
    counter = counter + 1
    my_proc.process_event(counter)
    data_reco = my_anaunit.GetData()
    part_reco = my_anaunit.GetParticles()

    print "Event: ", counter
    
    # how many neutrinos?
    numNeutrinos = 0
    Nodes = part_reco.GetPrimaryNodes()
    for nodeID in Nodes:
        if (part_reco.GetParticle(nodeID).PdgCode() == 12):
            numNeutrinos += 1

    print numNeutrinos

    
    if (numNeutrinos == 1):
        # we found something...lets plot it
        data_mc   = my_anaunit.GetData(True)
        part_mc   = my_anaunit.GetParticles(True)
        print "Processing event {0}".format(counter) 
        # get objets and display
        viewAll(data_mc,part_mc,data_reco,part_reco)

        try:
            counter = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
        except SyntaxError:
            counter = counter

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

