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
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.patches as patches
import numpy as np

#function to print point
def printPoint(p):
    
    print "[{0}, {1}, {2}]".format(p[0],p[1],p[2])

#define functions to plot point, line, half-line, segment and box
#plot POINT
def plotPoint(p,col):

    ax.scatter(p[0],p[1],p[2], col)

#plot SEGMENT
def plotSegment(seg,col):

    lineX = np.array( [seg.Start()[0], seg.End()[0]] )
    lineY = np.array( [seg.Start()[1], seg.End()[1]] )
    lineZ = np.array( [seg.Start()[2], seg.End()[2]] )
    
    ax.plot(lineX,lineY,lineZ,'o-',color=col)

#plot HALF-LINE
def plotHalfLine(hline,col):

    linesegX = np.array( [hline.Start()[0], hline.Start()[0]+200*hline.Dir()[0]] )
    linesegY = np.array( [hline.Start()[1], hline.Start()[1]+200*hline.Dir()[1]] )
    linesegZ = np.array( [hline.Start()[2], hline.Start()[2]+200*hline.Dir()[2]] )

    ax.plot(linesegX,linesegY,linesegZ,'o-',color=col)

#plot TRAJECTORY
def plotTrajectory(traj,col):

    Xpoints = np.array([])
    Ypoints = np.array([])
    Zpoints = np.array([])

    for x in range(traj.size()):

        Xpoints = np.append(Xpoints, traj[x][0])
        Ypoints = np.append(Ypoints, traj[x][1])
        Zpoints = np.append(Zpoints, traj[x][2])

    ax.plot( Xpoints,Ypoints,Zpoints,'o-',color=col)

gSystem.Load("libLArLite_Base")
gSystem.Load("libLArLite_Analysis")
gSystem.Load("libLArLite_LArUtil")
gSystem.Load("libBasicTool_GeoAlgo")

# Now import ana_processor & your class. For this example, ana_base.
from ROOT import larlite as fmwk
from ROOT import geoalgo


# create canvas for plotting
# split screen into X-Y projection and Z-Y projection

fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')
ax.set_xlim(-1,1)
ax.set_ylim(-1,1)
ax.set_zlim(-1,1)
ax.set_xlabel("X axis",fontsize=20)
ax.set_ylabel("Y axis",fontsize=20)
ax.set_zlabel("Z axis",fontsize=20)

t = ROOT.vector(ROOT.vector('double'))()
p = ROOT.vector('double')()
p.push_back(-.4)
p.push_back(-.5)
p.push_back(-.6)
t.push_back(p)
p = ROOT.vector('double')()
p.push_back(-.4)
p.push_back(-.5)
p.push_back(-.5)
t.push_back(p)
p = ROOT.vector('double')()
p.push_back(0)
p.push_back(-.1)
p.push_back(-.3)
t.push_back(p)


#start using functions from IntersectAlgo
IntAlg  = geoalgo.IntersectAlgo()
DistAlg = geoalgo.DistanceAlgo()

#get HalfLine & Box intersection
#pInt = IntAlg.Intersection(box,hline)
track = geoalgo.Trajectory(t);
hline1 = geoalgo.HalfLine(6,6,6,-1,0,2)
seg1 = geoalgo.LineSegment(-1,-0.2,-1,1,-0.5,0.3)
seg2 = geoalgo.LineSegment(0,1,0,0,-1,-1)
hline2 = geoalgo.HalfLine(2,4,-1,0,1,4)
p1 = geoalgo.Vector(-.4,-.5,-.55)
p2 = geoalgo.Vector(2,2,2)
minDist = DistAlg.SqDist(track, p1)
print minDist
#plotHalfLine(hline1)
#plotHalfLine(hline2)
plotTrajectory(track,'b')
plotPoint(p1,'b')
print "Point 1:"
print "Point 2:"

plt.show()

# done!
