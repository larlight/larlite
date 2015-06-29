import sys
import ROOT
from ROOT import ertool
from seltool.algoviewer import view,getViewer

#
# Make my fake data input
#

# My toy data maker instance
data_maker=ertool.ToyDataMaker()
# Make 2 fake tracks
my_shower1=data_maker.MakeShower(100,1,100, 40,40,40, 11)
my_shower2=data_maker.MakeShower(100,0,100, 20,20,20, 11)
# Fake ID
myid=ROOT.std.pair("size_t","string")()

#
# Configure ERTool
#

# My ERTool manager
mgr=ertool.Manager()
# Add algorithm
mgr.AddAlgo(ertool.ERAlgoToy())
# Add my algorithm's config file
mgr.AddCfgFile('dummy.cfg')

#
# Run it
#
mgr.Initialize()

# Fake "event" 1
mgr.ClearData()
mgr.Add(my_shower1,myid)
mgr.Add(my_shower2,myid)
mgr.Process()

#
# View event
#

# Instantiate viewer
viewer = getViewer("Reco")
# Call view function to draw on the viewer
view( viewer, mgr.EventData(), mgr.ParticleGraph() )
# Wait for a user's "hit-enter"
sys.stdin.readline()
# Finalize
mgr.Finalize()
