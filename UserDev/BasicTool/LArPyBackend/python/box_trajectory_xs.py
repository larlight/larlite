from larpy import GeoViewer
from ROOT import geoalgo

k=GeoViewer()
xs_alg=geoalgo.GeoAlgo()
pt=geoalgo.Vector(3)
trj=geoalgo.Trajectory()

for x in xrange(12):
    for y in xrange(pt.size()):
        if x<5 or not y:
            pt[y] = x-2
        elif y>=1:
            pt[y] = 5
    print 'Adding trajectory point:',x,' ... ',pt[0],pt[1],pt[2]
    trj += pt

box=geoalgo.AABox(0,0,0,7,7,7)

k.add(box,'Test Box')
k.add(trj,'Test Trajectory')

xs_pts = xs_alg.Intersection(trj,box)

for x in xrange(xs_pts.size()):
    k.add(xs_pts[x],'XS Point')

k.show()
