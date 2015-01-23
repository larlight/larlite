from basictool import GeoViewer
from ROOT import geoalgo
import matplotlib.pyplot as plt
import random

k=GeoViewer()

pt=geoalgo.Vector(3)
trj=geoalgo.Trajectory()

for x in xrange(10):
    for y in xrange(pt.size()):
        if x<5 or not y:
            pt[y] = x
        elif y>=1:
            pt[y] = 5
    #print 'Adding trajectory point:',x,' ... ',pt[0],pt[1],pt[2]
    trj += pt

box=geoalgo.AABox(0,0,0,7,7,7)



# set interactive mode on
plt.ion()

for x in xrange(10):
    line = geoalgo.LineSegment(random.random(),random.random(),random.random(),random.random(),random.random(),random.random())
    k.add(line,"shower","green")
    k.show()
    counter = input('Hit Enter for next event')
    k.clear()

print "done."
