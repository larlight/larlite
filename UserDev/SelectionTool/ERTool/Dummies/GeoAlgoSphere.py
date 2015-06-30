import ROOT
from ROOT import ertool
from ROOT import geoalgo
from basictool.geoviewer import GeoViewer
import matplotlib.pyplot as plt
import random as rnd
import time

plt.ion()

# track:
viewer2 = GeoViewer()
geoAlgo = geoalgo.GeoAlgo()
for x in xrange(1):
    track = ertool.Track()
    #viewer2._fig.clear()
    for n in xrange(6):
        time.sleep(0.01)
        point = geoalgo.Vector(rnd.random(),rnd.random(),rnd.random())
        viewer2.add(point,'pt','k')
        track.push_back(point)
    print "making a new sphere!"
    sphere = geoAlgo.boundingSphere(track)
    print "sphere center is: [%.02f,%.02f,%.02f]"%(sphere.Center()[0],sphere.Center()[1],sphere.Center()[2])
    print "sphere radius is: %f"%sphere.Radius()
viewer2.add(sphere,'','r')
viewer2.show()
try:
    counter = input('Hit ENTER when you are ready to close the viewer')
except:
    print "done with this example..."
