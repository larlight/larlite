from database import recoBase
from ROOT import evd, TVector3
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore
import math as mt


from database import recoBase3D

try:
    import pyqtgraph.opengl as gl
    import numpy as np

    class seed3D(recoBase3D):

        """docstring for seed3D"""

        def __init__(self):
            super(seed3D, self).__init__()
            self._productName = 'seed3D'
            self._process = evd.DrawSeed3D()
            self.init()

        def drawObjects(self, view_manager):

            geom = view_manager._geometry
            view = view_manager.getView()

            seeds = self._process.getData()

            # Each flash is drawn as an oval in Y/Z/X
            for i in xrange(len(seeds)):
                thisSeed = seeds[i]

                # Each seed just needs two points, the start and end point

                x = np.zeros(2)
                y = np.zeros(2)
                z = np.zeros(2)

                x[0] = thisSeed.point().X()
                x[1] = thisSeed.point().X() \
                    + thisSeed.direction().X()*thisSeed.length()

                y[0] = thisSeed.point().Y()
                y[1] = thisSeed.point().Y() \
                    + thisSeed.direction().Y()*thisSeed.length()

                z[0] = thisSeed.point().Z()
                z[1] = thisSeed.point().Z() \
                    + thisSeed.direction().Z()*thisSeed.length()

                pts = np.vstack([x, y, z]).transpose()
                pen = pg.mkPen((255, 0, 0), width=2)
                line = gl.GLLinePlotItem(pos=pts, color=(255, 255, 0, 255))
                view.addItem(line)
                self._drawnObjects.append(line)



except Exception, e:
    pass
