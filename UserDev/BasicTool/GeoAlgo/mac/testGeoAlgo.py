#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT, sys
from ROOT import *
from ROOT import gSystem
#import plotting tools
import matplotlib.pyplot as plt
import matplotlib.lines as lines
from matplotlib.path import Path
import matplotlib.patches as patches
import numpy as np

#define functions to plot point, line, half-line, segment and box
#plot POINT
def plotPoint(p,col):

    xy.plot(p[0],p[1], 'o', color=col)
    zy.plot(p[2],p[1], 'o', color=col)

#plot SEGMENT
def plotSegment(seg,col):

    lineseg = np.array([ [seg.Start()[0], seg.Start()[1], seg.Start()[2]],
                         [seg.End()[0],   seg.End()[1],   seg.End()[2]] ])

    xy.plot( lineseg[:,0], lineseg[:,1], 'o-', color=col)
    zy.plot( lineseg[:,2], lineseg[:,1], 'o-', color=col)

#plot HALF-LINE
def plotHalfLine(hline):

    linesegX = np.array( [hline.Start()[0], hline.Start()[0]+200*hline.Dir()[0]] )
    linesegY = np.array( [hline.Start()[1], hline.Start()[1]+200*hline.Dir()[1]] )
    linesegZ = np.array( [hline.Start()[2], hline.Start()[2]+200*hline.Dir()[2]] )

    xy.plot( linesegX, linesegY, 'o-', color='g')
    zy.plot( linesegZ, linesegY, 'o-', color='g')

#plot TRAJECTORY
def plotTrajectory(traj):

    Xpoints = np.array([])
    Ypoints = np.array([])
    Zpoints = np.array([])

    for x in range(traj.size()):

        Xpoints = np.append(Xpoints, traj[x][0])
        Ypoints = np.append(Ypoints, traj[x][1])
        Zpoints = np.append(Zpoints, traj[x][2])

    xy.plot( Xpoints, Ypoints, 'o-', color='k')
    zy.plot( Zpoints, Ypoints, 'o-', color='k')

#plot AABox
def plotAABox(box):

    #pointsXY = np.array( [[box.Min()[0], box.Max()[0]],
    #                      [box.Min()[1], box.Max()[1]] ])
    #pointsZY = np.array( [[box.Min()[2], box.Max()[2]],
    #                      [box.Min()[1], box.Max()[1]] ])

    pointsXY = np.array( [[box.Min()[0], box.Min()[1]],
                          [box.Min()[0], box.Max()[1]],
                          [box.Max()[0], box.Max()[1]],
                          [box.Max()[0], box.Min()[1]]])

    pointsZY = np.array( [[box.Min()[2], box.Min()[1]],
                          [box.Min()[2], box.Max()[1]],
                          [box.Max()[2], box.Max()[1]],
                          [box.Max()[2], box.Min()[1]]])


    rectXY = patches.Polygon(pointsXY, edgecolor='m', fill=False)
    rectZY = patches.Polygon(pointsZY, edgecolor='m', fill=False)

    xy.add_patch(rectXY)
    zy.add_patch(rectZY)
    


gSystem.Load("libLArLite_Base")
gSystem.Load("libLArLite_Analysis")
gSystem.Load("libLArLite_LArUtil")
gSystem.Load("libBasicTool_GeoAlgo")

# Now import ana_processor & your class. For this example, ana_base.
from ROOT import larlite as fmwk
from ROOT import geoalgo


# create canvas for plotting
# split screen into X-Y projection and Z-Y projection

fig, (xy, zy) = plt.subplots(ncols=2,figsize=(10,5))

xy.set_xlim(-5,5)
xy.set_ylim(-5,5)
zy.set_xlim(-5,5)
zy.set_ylim(-5,5)
xy.set_title("XY Projection",fontsize=18)
xy.set_xlabel("X Coordinate",fontsize=18)
xy.set_ylabel("Y Coordinate",fontsize=18)
zy.set_title("ZY Projection",fontsize=18)
zy.set_xlabel("Z Coordinate",fontsize=18)
zy.set_ylabel("Y Coordinate",fontsize=18)


t = ROOT.vector(ROOT.vector('double'))()
p = ROOT.vector('double')()
p.push_back(-4)
p.push_back(-5)
p.push_back(-6)
t.push_back(p)
p = ROOT.vector('double')()
p.push_back(-2)
p.push_back(-4)
p.push_back(-6)
t.push_back(p)
p = ROOT.vector('double')()
p.push_back(0)
p.push_back(-1)
p.push_back(-3)
t.push_back(p)

#distance algo
dAlgo = geoalgo.DistanceAlgo()

pt = geoalgo.Vector(3.3,-2.2,0)
seg = geoalgo.LineSegment(0,0,0,1,1,0)
plotPoint(pt,'r')
plotSegment(seg,'b')
cl = dAlgo.ClosestPt(pt,seg)
plotPoint(cl,'k')
print "Distance to Closest Point: {0}".format(cl.Dist(pt))
print "Algo Min Dist: {0}".format(dAlgo.SqDist(pt,seg))

plt.show()

# done!
