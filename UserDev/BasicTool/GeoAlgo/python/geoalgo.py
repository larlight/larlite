from ROOT import geoalgo, mpl
from matplotlib.collections import PolyCollection
from matplotlib.colors import colorConverter
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import random

#from itertools import product, combinations

class GeoViewer:

    _type_vec=geoalgo.Vector()
    _type_box=geoalgo.AABox()
    _type_trk=geoalgo.Trajectory()
    _type_seg=geoalgo.LineSegment()
    _helper = mpl.GeoAlgoMPLBackEnd()

    def __init__(self):

        self._fig = plt.figure(#num=None,
                               figsize=(8,6),
                               dpi=120,
                               facecolor="w",
                               edgecolor='k')
        self._fig.patch.set_alpha(0.5)
        self._fig.set_size_inches(18,10)
        self._fig.set_size_inches(18.5,10.5,forward=True)
        self._fig.subplots_adjust(left=0.01,
                                  right=0.42,
                                  top=0.55,
                                  bottom=0.01)
        #plt.get_current_fig_manager().frame.Maximize(True)
        self._ax  = self._fig.gca(projection='3d')
        #self._ax.set_aspect("equal")
        self._range_min = [ 1e12, 1e12, 1e12]
        self._range_max = [-1e12,-1e12,-1e12]

    def rand_color(self):
        c = np.random.rand(3,1)
        Gray = c[0] * 0.3 + c[1] * 0.59 + c[2] * 0.11
        print Gray
        return c

    def add(self,arg):

        if   type(arg) == type(self._type_vec): self.add_point(arg)
        elif type(arg) == type(self._type_trk): self.add_trajectory(arg)
        elif type(arg) == type(self._type_seg): self.add_linesegment(arg)
        elif type(arg) == type(self._type_box): self.add_box(arg)

    def add_point(self,arg):
        c = self.rand_color()
        if arg.size() == 3:
            self._ax.scatter([0],data[1],data[2],color=c,marker='o')
        elif arg.size() == 2:
            self._ax.scatter([0],data[1],0,color=c,marker='o')            

    def add_trajectory(self,arg):
        data=[[],[],[]]
        c = self.rand_color()
        for x in xrange(arg.size()):
            pt=arg[x]
            for y in xrange(pt.size()):
                data[y].append(pt[y])
                if pt[y] < self._range_min[y]: self._range_min[y] = pt[y]
                if pt[y] > self._range_max[y]: self._range_max[y] = pt[y]
        self._ax.plot(data[0],data[1],data[2],color=c)
        self._ax.scatter(data[0],data[1],data[2],color=c,marker='o')

        label = 'Traj\n(%.2g,%.2g,%.2g)\n(%.2g,%.2g,%.2g)' % \
                (data[0][0],data[1][0],data[2][0],
                 data[0][-1],data[2][-1],data[2][-1])
        self._ax.text(data[0][0] - (data[0][-1] - data[0][-1])*0.1,
                      data[1][0] - (data[1][-1] - data[1][-1])*0.1,
                      data[2][0] - (data[2][-1] - data[2][-1])*0.1,
                      label,
                      fontsize=10,
                      #color=c)
                      alpha=0.5)

    def add_linesegment(self,arg):
        data=[[],[],[]]
        s = arg.Start()
        e = arg.End()
        c = self.rand_color()
        for x in xrange(s.size()):
            data[x].append(s[x])
            data[x].append(e[x])

            if s[x] < self._range_min[x]: self._range_min[x] = s[x]
            if s[x] > self._range_max[x]: self._range_max[x] = s[x]
            if e[x] < self._range_min[x]: self._range_min[x] = e[x]
            if e[x] > self._range_max[x]: self._range_max[x] = e[x]

        self._ax.plot(data[0],data[1],data[2],color=c)
        self._ax.scatter(data[0],data[1],data[2],color=c,marker='o')

        label = 'LineSeg\n(%.2g,%.2g,%.2g)\n(%.2g,%.2g,%.2g)' % (s[0],s[1],s[2],
                                                                       e[0],e[1],e[2])
        self._ax.text(s[0] - (e[0] - s[0])*0.1,
                      s[1] - (e[1] - s[1])*0.1,
                      s[2] - (e[2] - s[2])*0.1,
                      label,
                      fontsize=10,
                      #color=c)
                      alpha=0.5)

    def add_box(self,arg):
        pt_min = arg.Min()
        pt_max = arg.Max()

        x_min,y_min,z_min = (pt_min[0],pt_min[1],pt_min[2])
        x_max,y_max,z_max = (pt_max[0],pt_max[1],pt_max[2])
        
        c = self.rand_color()
        #xy => z
        vtxs = []
        vtxs.append([(x_min,y_min), (x_max,y_min), (x_max,y_max), (x_min,y_max), (x_min,y_min)])
        vtxs.append([(x_min,y_min), (x_max,y_min), (x_max,y_max), (x_min,y_max), (x_min,y_min)])
        poly = PolyCollection(vtxs, facecolors = [c,c])
        poly.set_alpha(0.7)
        self._ax.add_collection3d(poly, zs=[z_min,z_max], zdir='z')

        #yz => x
        vtxs = []
        vtxs.append([(y_min,z_min), (y_max,z_min), (y_max,z_max), (y_min,z_max), (y_min,z_min)])
        vtxs.append([(y_min,z_min), (y_max,z_min), (y_max,z_max), (y_min,z_max), (y_min,z_min)])
        poly = PolyCollection(vtxs, facecolors = [c,c])
        poly.set_alpha(0.7)
        self._ax.add_collection3d(poly, zs=[x_min,x_max], zdir='x')

        #xy => z
        vtxs = []
        vtxs.append([(x_min,z_min), (x_max,z_min), (x_max,z_max), (x_min,z_max), (x_min,z_min)])
        vtxs.append([(x_min,z_min), (x_max,z_min), (x_max,z_max), (x_min,z_max), (x_min,z_min)])
        poly = PolyCollection(vtxs, facecolors = [c,c])
        poly.set_alpha(0.7)
        self._ax.add_collection3d(poly, zs=[z_min,z_max], zdir='y')

        label = 'AABox\n(%.2g,%.2g,%.2g)\n(%.2g,%.2g,%.2g)' % (x_min,y_min,z_min,
                                                                     x_max,y_max,z_max)
        self._ax.text((x_max - x_min)/2. + x_min,
                      (y_max - y_min)/2. + y_min,
                      (z_max - z_min)/2. + z_min,
                      label,
                      fontsize=10,
                      #color=c)
                      alpha=0.5)

        # Update max/min
        for x in xrange(pt_min.size()):
            if pt_min[x] < self._range_min[x]: self._range_min[x] = pt_min[x]
            if pt_min[x] > self._range_max[x]: self._range_max[x] = pt_min[x]        

        for x in xrange(pt_max.size()):
            if pt_max[x] < self._range_min[x]: self._range_min[x] = pt_max[x]
            if pt_max[x] > self._range_max[x]: self._range_max[x] = pt_max[x]        

    def show(self):

        if not self._range_min[0] == 1e12:

            for x in xrange(3):

                if self._range_min[x] < 0: 
                    self._range_min[x] *=1.2
                else:
                    self._range_min[x] *=0.8

                if self._range_max[x] < 0:
                    self._range_max[x] *=0.8
                else:
                    self._range_max[x] *=1.2

            #plt.xlim([self._range_min[0],self._range_max[0]])
            #plt.ylim([self._range_min[1],self._range_max[1]])
            
        self._ax.set_xlim(self._range_min[0],self._range_max[0])
        self._ax.set_ylim(self._range_min[1],self._range_max[1])
        self._ax.set_zlim(self._range_min[2],self._range_max[2])

        self._ax.set_xlabel('X [cm]')
        self._ax.set_ylabel('Y [cm]')
        self._ax.set_zlabel('Z [cm]')
        
        plt.show()
        


