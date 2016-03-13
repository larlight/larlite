from database import recoBase
from ROOT import evd, TVector3
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore
import math as mt

from database import recoBase3D

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
            points = np.ndarray((spts.size(),3))

            for i in xrange(len(spts)):
                thisPoint = spts[i]

                points[i][0] = thisPoint.X()
                points[i][1] = thisPoint.Y()
                points[i][2] = thisPoint.Z()

            glPointsCollection = gl.GLScatterPlotItem(pos=points,size=5)

            view.addItem(glPointsCollection)

            self._drawnObjects.append(glPointsCollection)


except Exception, e:
    pass
