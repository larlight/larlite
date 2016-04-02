from database import recoBase
from ROOT import evd, TVector3, larutil
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore
import math as mt

from database import recoBase3D

class spacepoint(recoBase):

    """docstring for spacepoint"""

    def __init__(self):
        super(spacepoint, self).__init__()
        self._productName = 'spacepoint'
        self._process = evd.DrawSpacepoint()
        self.init()

    def drawObjects(self, view_manager):

        geom = view_manager._geometry


        geoHelper = larutil.GeometryHelper.GetME()
     
        for view in view_manager.getViewPorts():

            thisPlane = view.plane()
            self._drawnObjects.append([])
            spts = self._process.getDataByPlane(thisPlane)


            radBigW = 0.2 / view_manager._geometry.wire2cm()
            radBigT = (0.2) / view_manager._geometry.time2cm()
            offset = view_manager._geometry.offset(
                thisPlane) / view_manager._geometry.time2cm()

            for i in xrange(len(spts)):
                thisPoint = spts[i]

                # Need to scale back into wire time coordinates:

                
                sW = thisPoint.w / view_manager._geometry.wire2cm()
                sT = thisPoint.t / view_manager._geometry.time2cm() + offset
                r = QtGui.QGraphicsEllipseItem(
                    sW -radBigW, sT-radBigT, 2*radBigW, 2*radBigT)

                r.setPen(pg.mkPen(255,0,255))
                # r.setBrush(pg.mkColor(255,0,255))
                # r.setBrush((0,0,0,opacity))
                self._drawnObjects[thisPlane].append(r)
                view._view.addItem(r)




try:
    import pyqtgraph.opengl as gl
    import numpy as np

    class spacepoint3D(recoBase3D):

        """docstring for spacepoint3D"""

        def __init__(self):
            super(spacepoint3D, self).__init__()
            self._productName = 'spacepoint3D'
            self._process = evd.DrawSpacepoint3D()
            self.init()

        def drawObjects(self, view_manager):

            geom = view_manager._geometry
            view = view_manager.getView()

            spts = self._process.getData()
            i_color = 0

            # Make a collection to add the points to:
            points = np.ndarray((spts.size(), 3))

            for i in xrange(len(spts)):
                thisPoint = spts[i]

                points[i][0] = thisPoint.X()
                points[i][1] = thisPoint.Y()
                points[i][2] = thisPoint.Z()

            glPointsCollection = gl.GLScatterPlotItem(pos=points, size=5)

            view.addItem(glPointsCollection)

            self._drawnObjects.append(glPointsCollection)


except Exception, e:
    pass
