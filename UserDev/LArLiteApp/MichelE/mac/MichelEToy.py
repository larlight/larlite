import ROOT
from ROOT import ertool
from ROOT import geoalgo
from basictool.geoviewer import GeoViewer
import matplotlib.pyplot as plt
#
# Make my fake data input
#


# Make a fake track and shower
# shower:
shrDir   = geoalgo.Vector(0.3,1.0,0.5)# direction
shrStart = geoalgo.Vector(10,-10,30) # start point
shower   = ertool.Shower(shrStart,shrDir,20,5)
shower._energy = 100
shower._dedx   = 2.2

# track:
track = ertool.Track()
for n in xrange(11):
    track.push_back(geoalgo.Vector(-45+(10+45)*(n/10.),15-(10+15)*(n/10.),4+(30-4)*(n/10.)))
track._energy = 435;

plt.ion()
viewer = GeoViewer()
viewer.add(shower,'shower','b')
viewer.add(track,'track','r')
viewer.show()
try:
    counter = input('Hit ENTER when you are ready to close the viewer')
except:
    print "Continuing to analys..."


# Fake ID
myid=ROOT.std.pair("size_t","string")()

#
# Configure ERTool
#

# My ERTool manager
mgr=ertool.Manager()
# Add algorithm
algo = ertool.AlgoMichelE()
algo.setVerbose(True)
mgr.AddAlgo(algo)
# Add my algorithm's config file
mgr.AddCfgFile('../../SelectionTool/ERTool/dat/ertool_default.cfg')

#
# Run it
#
mgr.Initialize()

# Fake "event" 1
mgr.ClearData()
mgr.Add( shower , myid )
mgr.Add( track , myid)
mgr.Process()


