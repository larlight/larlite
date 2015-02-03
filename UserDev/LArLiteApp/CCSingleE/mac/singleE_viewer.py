import sys, os
from ROOT import gSystem
from ROOT import ertool
ertool.Manager()
from ROOT import larlite as fmwk
fmwk.geo.PlaneID

from basictool import GeoViewer, geoalgo
from seltool import ERViewer
import matplotlib.pyplot as plt

ertool.Manager()

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

#viewer
plt.ion()
display_reco = ERViewer()
display_mc   = ERViewer()
display_reco.show()
display_mc.show()

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Create algorithm
my_algo = ertool.AlgoSingleE()
my_algo.useEMPart(False)
my_algo.setVerbose(True)
my_algo.setVtxToTrkStartDist(3)
my_algo.setVtxToTrkDist(5)
my_algo.setVtxToShrStartDist(50)
my_algo.setMaxIP(3)
#my_algo.setVerbose(True)

# Create Filter
MCfilter = fmwk.MC_CC1E_Filter();


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


my_anaunit = fmwk.ExampleERSelection()
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
while (counter < 1000):
    try:
        counter = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        counter = counter + 1
    my_proc.process_event(counter)

    # get objets and display
    display_reco.clear()
    display_mc.clear()
    data_reco = my_anaunit.GetData()
    part_reco = my_anaunit.GetParticles()
    display_reco.add(part_reco, data_reco, False)
    data_mc   = my_anaunit.GetData(True)
    part_mc   = my_anaunit.GetParticles(True)
    display_mc.add(part_mc, data_mc, False)

    display_reco.show()
    display_mc.show()

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

