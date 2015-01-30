from ROOT import geoalgo, larpy
from matplotlib.collections import PolyCollection
#from matplotlib.colors import colorConverter
from mpl_toolkits.mplot3d import Axes3D
from drawcone import  rot
import matplotlib.pyplot as plt
import numpy as np
import random

class GeoViewer(object):

    _type_vec  = geoalgo.Vector()
    _type_box  = geoalgo.AABox()
    _type_trk  = geoalgo.Trajectory()
    _type_seg  = geoalgo.LineSegment()
    _type_lin  = geoalgo.HalfLine()
    _type_con  = geoalgo.Cone()
    _holder    = geoalgo.GeoObjCollection()
    _converter = larpy.PyGeoObj()

    def __init__(self):
        self._fig = plt.figure(#num=None,
                               figsize=(10,5),
                               dpi=120,
                               facecolor="w",
                               edgecolor='k')
        self._fig.patch.set_alpha(0.5)
        #self._fig.set_size_inches(10,5)
        #self._fig.set_size_inches(10.5,5.5,forward=True)
        #self._fig.subplots_adjust(left=0.01,
        #                          right=0.42,
        #                          top=0.55,
        #                          bottom=0.01)
        #plt.get_current_fig_manager().frame.Maximize(True)
        self._ax  = self._fig.gca(projection='3d')
        #self._ax.set_aspect("equal")
        self._range_min = [ 1e12, 1e12, 1e12]
        self._range_max = [-1e12,-1e12,-1e12]

    def rand_color(self):
        c = np.random.rand(3,1)
        Gray = c[0] * 0.3 + c[1] * 0.59 + c[2] * 0.11
        return c

    def GetObjCollection(self):
        return self._holder

    def add(self,arg,label='',c=''):
        self._holder.Add(arg,label,c)

    def _add_label(self,args):
        #data = self._converter.Convert(arg)
        self._ax.text(*args, fontsize=10, color='black')

    def _add_point(self,arg,c=''):
        data = self._converter.Convert(arg)
        if not c: c = self.rand_color()
        
        # update boundaries
        for i in xrange(3):
            if data[i] < self._range_min[i]: self._range_min[i] = data[i]
            if data[i] > self._range_max[i]: self._range_max[i] = data[i]

        self._ax.scatter(*data,color=c,marker='*',s=100)

    def _add_trajectory(self,arg,c=''):
        if not c: c = self.rand_color()
        data = self._converter.Convert(arg) 
        
        # update boundaries
        for i in xrange(3):
            for j in xrange(len(data[i])):
                if data[i][j] < self._range_min[i]: self._range_min[i] = data[i][j]
                if data[i][j] > self._range_max[i]: self._range_max[i] = data[i][j]

        self._ax.plot(data[0],data[1],data[2],color=c)
        self._ax.scatter(data[0][0],data[1][0],data[2][0],s=50,color=c,marker='x')
        self._ax.scatter(data[0][-1],data[1][-1],data[2][-1],s=50,color=c,marker='>')
        self._ax.scatter(data[0][1:-2],data[1][1:-2],data[2][1:-2],color=c,marker='o')

    def _add_linesegment(self,arg,c=''):
        if not c: c = self.rand_color()
        data = self._converter.Convert(arg)

        s = arg.Start()
        e = arg.End()

        # update boundaries
        for i in xrange(3):
            for j in xrange(len(data[i])):
                if data[i][j] < self._range_min[i]: self._range_min[i] = data[i][j]
                if data[i][j] > self._range_max[i]: self._range_max[i] = data[i][j]

        self._ax.plot(data[0],data[1],data[2],color=c)
        self._ax.scatter(s[0],s[1],s[2],s=50, color=c,marker='x')
        self._ax.scatter(e[0],e[1],e[2],s=50, color=c,marker='o')

    def _add_halfline(self,arg,c=''):
        if not c: c = self.rand_color()
        
        s = arg.Start()
        d = arg.Dir()

        u = np.linspace(0,2*np.pi,50)
        r0 = 10 # maximum radius of cone
        h0 = 50 # height of cone
        num_levels = 30


        # Take direction and find Euler Angles for cone rotation
        phi = np.arccos(d[0]/np.sqrt(d[0]*d[0]+d[1]*d[1]))
        if ( d[1] < 0 ):
            phi *= -1
        theta  = np.arccos(d[2]/np.sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]))

        x = np.hstack([r0*(1-h)*np.cos(u) for h in np.linspace(0,1,num_levels)])
        y = np.hstack([r0*(1-h)*np.sin(u) for h in np.linspace(0,1,num_levels)])
        z = np.hstack([np.ones(len(u))*h0*(1-h) for h in np.linspace(0,1,num_levels)])
        XYZ = np.vstack([x,y,z])

        xp,yp,zp = rot(XYZ,theta,phi) 

        for i in xrange(len(xp)):
            xp[i] += s[0]
            yp[i] += s[1]
            zp[i] += s[2]

        # update boundaries
        for j in xrange(len(xp)):
                if xp[j] < self._range_min[0]: self._range_min[0] = xp[j]
                if xp[j] > self._range_max[0]: self._range_max[0] = xp[j]
        for j in xrange(len(yp)):
                if yp[j] < self._range_min[1]: self._range_min[1] = yp[j]
                if yp[j] > self._range_max[1]: self._range_max[1] = yp[j]
        for j in xrange(len(zp)):
                if zp[j] < self._range_min[2]: self._range_min[2] = zp[j]
                if zp[j] > self._range_max[2]: self._range_max[2] = zp[j]


        self._ax.plot(xp,yp,zp,color=c)



    def _add_cone(self,arg,c=''):

        if not c: c = self.rand_color()
        
        s = arg.Start()
        d = arg.Dir()

        u = np.linspace(0,2*np.pi,50)
        r0 = arg.Radius() # maximum radius of cone
        h0 = arg.Length() # height of cone
        num_levels = 30


        # Take direction and find Euler Angles for cone rotation
        phi = np.arccos(d[0]/np.sqrt(d[0]*d[0]+d[1]*d[1]))
        if ( d[1] < 0 ):
            phi *= -1
        theta  = np.arccos(d[2]/np.sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]))

        x = np.hstack([r0*(1-h)*np.cos(u) for h in np.linspace(0,1,num_levels)])
        y = np.hstack([r0*(1-h)*np.sin(u) for h in np.linspace(0,1,num_levels)])
        z = np.hstack([np.ones(len(u))*h0*(1-h) for h in np.linspace(0,1,num_levels)])
        XYZ = np.vstack([x,y,z])

        xp,yp,zp = rot(XYZ,theta,phi) 

        for i in xrange(len(xp)):
            xp[i] += s[0]
            yp[i] += s[1]
            zp[i] += s[2]

        # update boundaries
        for j in xrange(len(xp)):
                if xp[j] < self._range_min[0]: self._range_min[0] = xp[j]
                if xp[j] > self._range_max[0]: self._range_max[0] = xp[j]
        for j in xrange(len(yp)):
                if yp[j] < self._range_min[1]: self._range_min[1] = yp[j]
                if yp[j] > self._range_max[1]: self._range_max[1] = yp[j]
        for j in xrange(len(zp)):
                if zp[j] < self._range_min[2]: self._range_min[2] = zp[j]
                if zp[j] > self._range_max[2]: self._range_max[2] = zp[j]

        self._ax.scatter(s[0],s[1],s[2],s=50,color=c,marker='x')
        self._ax.plot(xp,yp,zp,color=c)

    def _add_box(self,arg, c=''):
        if not c: c = self.rand_color()
        data = self._converter.Convert(arg);
        for x in data:
            self._ax.plot3D(*x,color=c)

        # Update max/min
        pt_min = arg.Min()
        pt_max = arg.Max()
        for x in xrange(pt_min.size()):
            if pt_min[x] < self._range_min[x]: self._range_min[x] = pt_min[x]
            if pt_min[x] > self._range_max[x]: self._range_max[x] = pt_min[x]        

        for x in xrange(pt_max.size()):
            if pt_max[x] < self._range_min[x]: self._range_min[x] = pt_max[x]
            if pt_max[x] > self._range_max[x]: self._range_max[x] = pt_max[x]        

            
    def clear(self):

        #plt.cla()
        self._fig.clear()
        self._ax  = self._fig.gca(projection='3d')
        self._fig.show()
        self._range_min = [ 1e12, 1e12, 1e12]
        self._range_max = [-1e12,-1e12,-1e12]
        self._holder.Clear()

    def show(self):

        # Process GeoObj
        points = self._holder.Point()
        np = points.size()

        # Draw axis

        for x in xrange(len(self._holder.Point())):
            #print x[0],x[1],x[2]
            self._add_point(self._holder.Point()[x], self._holder.PointColor()[x])
        for x in xrange(len(self._holder.LineSegment())):
            self._add_linesegment(self._holder.LineSegment()[x], self._holder.LineSegmentColor()[x])
        for x in xrange(len(self._holder.HalfLine())):
            self._add_halfline(self._holder.HalfLine()[x], self._holder.HalfLineColor()[x])
        for x in xrange(len(self._holder.AABox())):
            self._add_box(self._holder.AABox()[x], self._holder.AABoxColor()[x])
        for x in xrange(len(self._holder.Trajectory())):
            self._add_trajectory(self._holder.Trajectory()[x], self._holder.TrajectoryColor()[x])
        for x in xrange(len(self._holder.Cone())):
            self._add_cone(self._holder.Cone()[x], self._holder.ConeColor()[x])
        for x in self._converter.Convert(self._holder.Labels()):
            self._add_label(x)

        if not self._range_min[0] == 1e12:
            '''
            for x in xrange(3):

                if self._range_min[x] < 0: 
                    self._range_min[x] *=1.2
                else:
                    self._range_min[x] *=0.8

                if self._range_max[x] < 0:
                    self._range_max[x] *=0.8
                else:
                    self._range_max[x] *=1.2
            '''
            #plt.xlim([self._range_min[0],self._range_max[0]])
            #plt.ylim([self._range_min[1],self._range_max[1]])
            
        self._ax.set_xlim(self._range_min[0],self._range_max[0])
        self._ax.set_ylim(self._range_min[1],self._range_max[1])
        self._ax.set_zlim(self._range_min[2],self._range_max[2])


        #self._ax.plot([0,0],[0,0],[0,30],color='red')
        #self._ax.plot([0,0],[0,30],[0,0],color='red')
        #self._ax.plot([0,30],[0,0],[0,0],color='red')

        self._ax.set_xlabel('X [cm]')
        self._ax.set_ylabel('Y [cm]')
        self._ax.set_zlabel('Z [cm]')

        print self._fig
        self._fig.canvas
        self._fig.canvas.draw()#plt.show()
        #self._fig.show()
        #help(self._fig.canvas)
        #help(self._fig)
