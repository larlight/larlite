from .database import recoBase
from ROOT import evd
from pyqtgraph.Qt import QtGui
import pyqtgraph as pg


class hit(recoBase):

    """docstring for hit"""

    def __init__(self):
        super(hit, self).__init__()
        self._productName = 'hit'
        self._process = evd.DrawHit()
        self._brush = (0, 0, 0)
        self.init()

    # this is the function that actually draws the hits.
    def drawObjects(self, view_manager):

        for view in view_manager.getViewPorts():
            thisPlane = view.plane()
            self._drawnObjects.append([])
            # First get the hit information:
            hits = self._process.getDataByPlane(thisPlane)

            for i in range(len(hits)):
                hit = hits[i]
                # Draws a rectangle at (x,y,xlength, ylength)
                r = QtGui.QGraphicsRectItem(
                    hit.wire(), hit.time(), 1, hit.rms())

                opacity = hit.charge() / self._process.maxCharge(thisPlane)
                # opacity = exp( 1 + hit.charge() / self._process.maxCharge(thisPlane))/exp(2);
                # # New Way
                # r.setPen(pg.mkPen(brush,width=2))
                # # r.setBrush(pg.mkColor((255,255,255)))

                # Old Way:
                r.setPen(pg.mkPen(None))
                r.setBrush(pg.mkColor(opacity))
                # r.setBrush((0,0,0,opacity))
                self._drawnObjects[thisPlane].append(r)
                view._view.addItem(r)

    def getHitsOnWire(self, plane, wire):
        return self._process.getHitsOnWirePlane(wire,plane)
        