import ROOT
from ROOT import ertool
from ROOT import geoalgo
from basictool.geoviewer import GeoViewer
import matplotlib.pyplot as plt
import random as rnd
#
# Make my fake data input
#


# Make a fake track and shower
# shower1:
shrDir   = geoalgo.Vector(0.3,1.0,0.5)# direction
shrStart = geoalgo.Vector(14,-10,30) # start point
shower1   = ertool.Shower(shrStart,shrDir,20,5)
shower1._energy = 100
shower1._dedx   = 2.2
# shower2:
shrDir   = geoalgo.Vector(-0.33,1.0,0.5)# direction
shrStart = geoalgo.Vector(10,-11,30) # start point
shower2   = ertool.Shower(shrStart,shrDir,20,5)
shower2._energy = 100
shower2._dedx   = 2.2

# load viewer
plt.ion()
viewer = GeoViewer()
viewer.add(shower1,'shower1','b')
viewer.add(shower2,'shower2','b')
#viewer.add(track,'track','r')
viewer.show()
try:
    counter = input('Hit ENTER when you are ready to close the viewer')
except:
    print "Continuing to analys..."

# figure out the impact parameter between the two:
# use AlgoFindRelationship
algo = ertool.AlgoFindRelationship()
vertex = geoalgo.Vector(3)
IP = algo.FindClosestApproach(shower1,shower2,vertex)

# segments connecting vertex to start points
seg1 = geoalgo.LineSegment(vertex,shower1.Start())
seg2 = geoalgo.LineSegment(vertex,shower2.Start())

print "The closest approach between the two showers is: "
print "Impact parameter: %f cm"%IP
print "Vertex: [%.02f,%.02f,%.02f]"%(vertex[0],vertex[1],vertex[2])

#viewer = GeoViewer()
viewer.add(seg1,'','r')
viewer.add(seg2,'','r')
viewer.add(vertex,'vertex','k')
viewer.show()
try:
    counter = input('Hit ENTER when you are ready to close the viewer')
except:
    print "done with this example..."



