from database import recoBase
from ROOT import evd
from pyqtgraph.Qt import QtGui
import pyqtgraph as pg
from ROOT import larutil


class vertex(recoBase):

    """docstring for vertex"""

    def __init__(self):
        super(vertex, self).__init__()
        self._productName = 'vertex'
        self._process = evd.DrawVertex()
        self._brush = (0, 0, 0)
        self.init()

    # this is the function that actually draws the hits.
    def drawObjects(self, view_manager):

        for view in view_manager.getViewPorts():
            thisPlane = view.plane()
            self._drawnObjects.append([])
            # First get the vertex information:
            vertexes = self._process.getDataByPlane(thisPlane)

            for i in xrange(len(vertexes)):
                point = vertexes[i]
                # Draws a circle at (x,y,radius = 0.5cm)
                radBigW = 0.5 / view_manager._geometry.wire2cm()
                radBigT = (0.5) / view_manager._geometry.time2cm()

                offset = view_manager._geometry.offset(
                    thisPlane) / view_manager._geometry.time2cm()


                sW = point.w / view_manager._geometry.wire2cm()
                sT = point.t / view_manager._geometry.time2cm() + offset



                r = QtGui.QGraphicsEllipseItem(
                    sW-radBigW, sT-radBigT, 2*radBigW, 2*radBigT)

                # r = QtGui.QGraphicsRectItem(
                #     point.wire(), point.time(), 1, point.strength())

                # opacity = point.charge() / self._process.maxCharge(thisPlane)
                opacity = 1
                # opacity = exp( 1 + hit.charge() / self._process.maxCharge(thisPlane))/exp(2);
                # # New Way
                # r.setPen(pg.mkPen(brush,width=2))
                # # r.setBrush(pg.mkColor((255,255,255)))

                # Old Way:
                r.setPen(pg.mkPen(None))
                r.setBrush(pg.mkColor(0))
                # r.setBrush((0,0,0,opacity))
                self._drawnObjects[thisPlane].append(r)
                view._view.addItem(r)
