from ROOT import geoalgo, larpy
from matplotlib.collections import PolyCollection
#from matplotlib.colors import colorConverter
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import random

class GeoViewer:

    _type_vec  = geoalgo.Vector()
    _type_box  = geoalgo.AABox()
    _type_trk  = geoalgo.Trajectory()
    _type_seg  = geoalgo.LineSegment()
    _holder    = geoalgo.GeoObjCollection()
    _converter = larpy.PyGeoObj()

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
        return c

    def add(self,arg):
        self._holder.Add(arg)

    def _add_point(self,arg,c=''):
        data = self._converter.Convert(arg)
        if not c: c = self.rand_color()
        self._ax.scatter(*data,color=c,marker='o')

    def _add_label(self,args):
        #data = self._converter.Convert(arg)
        self._ax.text(*args, fontsize=10, alpha=0.5)

    def _add_trajectory(self,arg):
        c = self.rand_color()
        data = self._converter.Convert(arg) 
        for i in xrange(3):
            for j in xrange(len(data[i])):
                if data[i][j] < self._range_min[i]: self._range_min[i] = data[i][j]
        self._ax.plot(data[0],data[1],data[2],color=c)
        self._ax.scatter(data[0],data[1],data[2],color=c,marker='o')

    def _add_linesegment(self,arg):
        c = self.rand_color()
        data=_converter.Convert(arg)
        s = arg.Start()
        e = arg.End()

        for i in xrange(3):
            for j in xrange(len(data[i])):
                if data[i][j] < self._range_min[i]: self._range_min[i] = data[i][j]

        self._ax.plot(data[0],data[1],data[2],color=c)
        self._ax.scatter(data[0],data[1],data[2],color=c,marker='o')

    def _add_box(self,arg):
        c = self.rand_color()
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

    def show(self):

        # Process GeoObj
        for x in self._holder.Point():
            self._add_point(x)
        for x in self._holder.LineSegment():
            self._add_linesegment(x)
        for x in self._holder.AABox():
            self._add_box(x)
        for x in self._holder.Trajectory():
            self._add_trajectory(x)
        for x in self._converter.Convert(self._holder.Labels()):
            self._add_label(x)

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
        
