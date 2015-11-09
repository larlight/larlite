import ROOT
from ROOT import ertool
from ROOT import geoalgo
from basictool.geoviewer import GeoViewer
import matplotlib.pyplot as plt
import random as rnd
import time
import datetime

plt.ion()

tottime = 0

# track:
viewer2 = GeoViewer()
viewer2._use_box = False
geoAlgo = geoalgo.GeoAlgo()
ntries = 1000
npoints = 800
#for x in xrange(ntries):
track = ertool.Track()
#viewer2._fig.clear()
#for n in xrange(npoints):
#time.sleep(0.01)
point = geoalgo.Vector(rnd.random(),rnd.random(),rnd.random())
n = 0
viewer2.add(point,'pt %i'%n,'k')
track.push_back(point)
viewer2.add(point,'pt %i'%n,'k')
track.push_back(point)
viewer2.add(point,'pt %i'%n,'k')
track.push_back(point)
viewer2.add(point,'pt %i'%n,'k')
track.push_back(point)
t = time.time()
sphere = geoAlgo.boundingSphere(track)
tdelta = time.time()-t
tottime += tdelta
#print "sphere center is: [%.02f,%.02f,%.02f]"%(sphere.Center()[0],sphere.Center()[1],sphere.Center()[2])
#print "sphere radius is: %f"%sphere.Radius()
print "Time per sphere with %i points is: %.02f [us]"%(npoints,1000000*(tottime/float(ntries)))
viewer2.add(sphere,'','r')
viewer2.show()
try:
    counter = input('Hit ENTER when you are ready to close the viewer')
except:
    print "done with this example..."
