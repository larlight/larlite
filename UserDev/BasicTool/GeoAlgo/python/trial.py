from ROOT import geoalgo
alg=geoalgo.DistanceAlgo()
hline1  = geoalgo.HalfLine(3,3,3,-1,-2,-3)
hline2  = geoalgo.HalfLine(2,4,-1,1,0.5,4)
p1      = geoalgo.Vector(1,1,1)
p2      = geoalgo.Vector(2,2,2)
print alg.SqDist(hline1, hline2, p1, p2)

