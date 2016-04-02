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
                r.setBrush(pg.mkColor(0,255,255))
                # r.setBrush((0,0,0,opacity))
                self._drawnObjects[thisPlane].append(r)
                view._view.addItem(r)


from database import recoBase3D

try:
    import pyqtgraph.opengl as gl
    import numpy as np

    class vertex3D(recoBase3D):

        """docstring for vertex3D"""

        def __init__(self):
            super(vertex3D, self).__init__()
            self._productName = 'vertex3D'
            self._process = evd.DrawVertex3D()
            self.init()

        def drawObjects(self, view_manager):

            geom = view_manager._geometry
            view = view_manager.getView()

            vertexes = self._process.getData()

            for i in xrange(len(vertexes)):
                vertex = vertexes[i]

                # Stupid thing right now:
                # make a 3 lines around the vertex

                xline = np.ndarray((2, 3))
                yline = np.ndarray((2, 3))
                zline = np.ndarray((2, 3))

                length = 5

                for line in xline, yline, zline:
                    for point in line:
                        point[0] = vertex.X()
                        point[1] = vertex.Y()
                        point[2] = vertex.Z()

                xline[0][0] += length/2
                xline[1][0] -= length/2
                yline[0][1] += length/2
                yline[1][1] -= length/2
                zline[0][2] += length/2
                zline[1][2] -= length/2

                # Make the 3 lines for the vertex:

                # pts = np.vstack([x, y, z]).transpose()
                # pen = pg.mkPen((255, 0, 0), width=2)
                xglline = gl.GLLinePlotItem(pos=xline, width=3,
                                            color=(0, 0, 255, 255))
                yglline = gl.GLLinePlotItem(pos=yline, width=3,
                                            color=(0, 0, 255, 255))
                zglline = gl.GLLinePlotItem(pos=zline, width=3,
                                            color=(0, 0, 255, 255))
                view.addItem(xglline)
                view.addItem(yglline)
                view.addItem(zglline)
                self._drawnObjects.append(xglline)
                self._drawnObjects.append(yglline)
                self._drawnObjects.append(zglline)

except Exception, e:
    pass
