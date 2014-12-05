from geoalgo import *

k=GeoViewer()

pt=geoalgo.Vector(3)
trj=geoalgo.Trajectory()

for x in xrange(10):
    for y in xrange(pt.size()):
        if x<5 or not y:
            pt[y] = x
        elif y>=1:
            pt[y] = 5
    print 'Adding trajectory point:',x,' ... ',pt[0],pt[1],pt[2]
    trj += pt

box=geoalgo.AABox(0,0,0,7,7,7)

k.add(box)
k.add(trj)
k.show()
