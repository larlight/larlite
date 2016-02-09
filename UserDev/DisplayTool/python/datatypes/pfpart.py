from database import recoBase
from ROOT import evd, TVector3
import pyqtgraph as pg
import pyqtgraph.opengl as gl
from pyqtgraph.Qt import QtGui, QtCore
import math as mt
from ROOT import larutil

from database import recoBase3D

try:
    import pyqtgraph.opengl as gl
    import numpy as np

    class pfpart3D(recoBase3D):

        """docstring for pfpart3D"""

        def __init__(self):
            super(pfpart3D, self).__init__()
            self._productName = 'pfpart3D'
            self._process = evd.DrawPFParticle3D()
            self.init()
            # Defining the cluster colors:
            self._pfpartColors = [
                (0, 147, 147, 100),  # dark teal
                (0, 0, 252, 100),   # bright blue
                (156, 0, 156, 100),  # purple
                (255, 0, 255, 100),  # pink
                (255, 0, 0, 100),  # red
                (175, 0, 0, 100),  # red/brown
                (252, 127, 0, 100),  # orange
                (102, 51, 0, 100),  # brown
                # (127, 127, 127, 100),  # dark gray
                # (210, 210, 210, 100),  # gray
                (100, 253, 0, 100)  # bright green
            ]

            self.setParamsDrawing(False)

        def setParamsDrawing(self, paramsBool):
            self._drawParams = paramsBool

        def drawObjects(self, view_manager):

            geom = view_manager._geometry
            view = view_manager.getView()

            parts = self._process.getData()
            i_color = 0

            # Each flash is drawn as an oval in Y/Z/X
            for i in xrange(len(parts)):
                thisPart = parts[i]

                # Get the points from the particles and draw them, color coded
                spacepoints = thisPart.points()
                points = np.ndarray((spacepoints.size(), 3))

                for i in xrange(spacepoints.size()):
                    thisPoint = spacepoints[i]

                    points[i][0] = thisPoint.X()
                    points[i][1] = thisPoint.Y()
                    points[i][2] = thisPoint.Z()

                glPointsCollection \
                    = gl.GLScatterPlotItem(pos=points,
                                           size=5,
                                           color=self._pfpartColors[i_color])

                view.addItem(glPointsCollection)

                self._drawnObjects.append(glPointsCollection)

                if self._drawParams:

                    # Let's find out what's in the params:
                    thisPart.params().Report()

                    # Draw the axis of the 
                    pass

                i_color += 1
                if i_color >= len(self._pfpartColors):
                    i_color = 0

except Exception, e:
    pass
