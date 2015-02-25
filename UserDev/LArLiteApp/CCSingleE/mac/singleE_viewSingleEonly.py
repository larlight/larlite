import sys, os
from ROOT import gSystem
from ROOT import ertool
ertool.Manager()
from ROOT import larlite as fmwk
fmwk.geo.PlaneID

from algoviewer import viewAll

ertool.Manager()

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Create algorithm
my_algo = ertool.AlgoSingleE()
my_algo.useRadLength(True)
my_algo.setVerbose(True)
my_algo.setVtxToTrkStartDist(1)
my_algo.setVtxToTrkDist(1)
my_algo.setVtxToShrStartDist(50)
my_algo.setMaxIP(1)
#my_algo.setVerbose(True)

# Create Filter
MCfilter = fmwk.MC_CC1E_Filter();
MCfilter.flip(False)

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("singleE_selection.root")


# Possible filter to select true events
#my_proc.enable_filter(True)
#pdgsel = fmwk.PDGSelection()
#pdgsel.Select(11,pdgsel.kGENERATOR,1)
#my_proc.add_process(pdgsel)

my_ana = ertool.ERAnaSingleE()
my_ana.SetDebug(True)

my_anaunit = fmwk.ExampleERSelection()
my_anaunit.SetMinEDep(10)
my_anaunit._mgr.SetAlgo(my_algo)
my_anaunit._mgr.SetAna(my_ana)
my_anaunit._mgr._mc_for_ana = True
# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
#my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetTrackProducer(True,"mcreco");
#my_anaunit.SetVtxProducer(True,"generator");
#my_anaunit.SetShowerProducer(False,"mergeall");
my_anaunit.SetShowerProducer(False,"showerreco");
#my_anaunit.SetTrackProducer(False,"stitchkalmanhit");
# ************************************************
my_proc.add_process(MCfilter)
my_proc.add_process(my_anaunit)


# Start event-by-event loop
counter = 0
while (counter < 20000):
    
    counter = counter + 1
    my_proc.process_event(counter)

    data_reco = my_anaunit.GetData()
    part_reco = my_anaunit.GetParticles()

    if (part_reco.size() > 0):
        # we found something...lets plot it
        data_mc   = my_anaunit.GetData(True)
        part_mc   = my_anaunit.GetParticles(True)
        print "Processing event {0}".format(counter) 
        # get objets and display
        viewAll(data_mc,part_mc,data_reco,part_reco)

        for x in xrange(part_mc.size()):
            print part_mc[x].Diagram()
            
        try:
            counter = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
        except SyntaxError:
            counter = counter + 1

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

