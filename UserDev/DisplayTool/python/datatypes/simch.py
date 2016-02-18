from database import recoBase
from ROOT import evd, TVector3
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore
import math as mt

from database import recoBase3D

try:
    import pyqtgraph.opengl as gl
    import numpy as np

    class simch3D(recoBase3D):

        """docstring for spacepoint3D"""

        def __init__(self):
            super(simch3D, self).__init__()
            self._productName = 'simch3D'
            self._process = evd.DrawSimch()
            self.init()

        def drawObjects(self, view_manager):

            geom = view_manager._geometry
            view = view_manager.getView()

            spts = self._process.getData()
            color = [128, 128, 128, 128]

            # Make a collection to add the points to:
            points = np.ndarray((spts.size(),3))

            for i in xrange(len(spts)):
                thisPoint = spts[i]

                points[i][0] = thisPoint.X()
                points[i][1] = thisPoint.Y()
                points[i][2] = thisPoint.Z()

            glPointsCollection = gl.GLScatterPlotItem(pos=points,size=5,color=color)

            view.addItem(glPointsCollection)

            self._drawnObjects.append(glPointsCollection)


except Exception, e:
    pass
