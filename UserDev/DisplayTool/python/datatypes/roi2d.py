from database import recoBase
from ROOT import evd, TVector3
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore
import math as mt

# roi drawing is currently "experimental"


class roi2d(recoBase):

    """docstring for roi2d"""

    def __init__(self):
        super(roi2d, self).__init__()
        self._productName = 'roi2d'
        self._process = evd.DrawPiZeroROI()
        self.init()

        # Defining the cluster colors:
        self._roi2dColors = [
            (0, 147, 147, 100),  # dark teal
            (0, 0, 252, 100),   # bright blue
            (156, 0, 156, 100),  # purple
            (255, 0, 255, 100),  # pink
            (255, 0, 0, 100),  # red
            (175, 0, 0, 100),  # red/brown
            (252, 127, 0, 100),  # orange
            (102, 51, 0, 100),  # brown
            (127, 127, 127, 100),  # dark gray
            (210, 210, 210, 100),  # gray
            (100, 253, 0, 100)  # bright green
        ]

    def drawObjects(self, view_manager):

        for view in view_manager.getViewPorts():
            # get the showers from the process:
            self._drawnObjects.append([])

            roi2ds = self._process.getDataByPlane(view.plane())

            i_color = 0

            for i in xrange(len(roi2ds)):
                roi2d = roi2ds[i]

                if i_color > len(self._roi2dColors):
                    i_color = 0

                color = self._roi2dColors[i_color]

                # construct a polygon for this roi2d:
                points = []
                # Remember - everything is in cm, but the display is in
                # wire/time!
                geom = view_manager._geometry
                x1 = roi2d.maxmaxPoint().w
                y1 = roi2d.maxmaxPoint().t
                x2 = roi2d.minminPoint().w
                y2 = roi2d.minminPoint().t

                points.append(QtCore.QPoint(x1, y1))
                points.append(QtCore.QPoint(x2, y2))

                thisPolyF = QtGui.QPolygonF(points)
                thisPoly = QtGui.QGraphicsPolygonItem(thisPolyF)

                thisPoly.setPen(pg.mkPen(None))
                thisPoly.setBrush(pg.mkColor(color))

                view._view.addItem(thisPoly)
                self._drawnObjects[view.plane()].append(thisPoly)

                i_color += 1

except Exception, e:
    pass
