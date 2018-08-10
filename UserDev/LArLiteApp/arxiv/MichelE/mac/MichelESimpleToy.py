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
    track.push_back(geoalgo.Vector(-45.1+(10+45)*(n/10.),15.1-(10+15)*(n/10.),4.1+(30-4)*(n/10.)))
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


# are these shower and track correlated? is the shower a michel?
dist = shower.Start().Dist(track.back())
print "distance between shower start and track end is: %f"%dist


