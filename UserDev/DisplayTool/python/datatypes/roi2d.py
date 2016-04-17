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
                vtx = []
                # Remember - everything is in cm, but the display is in
                # wire/time!
                geom = view_manager._geometry
                x1 = roi2d.maxmaxPoint().w
                y1 = roi2d.maxmaxPoint().t
                x2 = roi2d.minminPoint().w
                y2 = roi2d.minminPoint().t
                vx = (roi2d.vtxPoint().w)
                vy = (roi2d.vtxPoint().t)
                
                points.append(QtCore.QPoint(x1, y1))
                points.append(QtCore.QPoint(x1, y2))
                points.append(QtCore.QPoint(x2, y2))
                points.append(QtCore.QPoint(x2, y1))

                thisPolyF = QtGui.QPolygonF(points)
                thisPoly = QtGui.QGraphicsPolygonItem(thisPolyF)

                thisPoly.setPen(pg.mkPen(pg.mkColor((255, 255, 0, 100)), width = 4))
                thisPoly.setBrush(pg.mkBrush(None))

                radBigW = 0.5 / view_manager._geometry.wire2cm()
                radBigT = (0.5) / view_manager._geometry.time2cm()
                
                otherPoly = QtGui.QGraphicsEllipseItem(vx-radBigW, vy-radBigT, 20*radBigW, 20*radBigT)
                #otherPoly = QtGui.QGraphicsEllipseItem( vy-radBigT, vx-radBigW,2*radBigW,2*radBigT)

                print "(vx,vy): {},{}".format(vx,vy)
                
                otherPoly.setPen(pg.mkPen(pg.mkColor((255, 0, 100, 100)), width = 1))
                otherPoly.setBrush(pg.mkBrush(pg.mkColor((255, 0, 100, 100))))                

                view._view.addItem(thisPoly)
                view._view.addItem(otherPoly)

                self._drawnObjects[view.plane()].append(thisPoly)
                self._drawnObjects[view.plane()].append(otherPoly)

                i_color += 1
