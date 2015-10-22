from data import recoBase
from PyQt4 import QtGui
from ROOT import evd
import pyqtgraph as pg


class polyLine(QtGui.QGraphicsPathItem):

    def __init__(self, points, pen=None):
        super(polyLine, self).__init__()
        self._points = points

        # Initialize a path:
        path = QtGui.QPainterPath()
        if pen is None:
            pen = QtGui.QPen(QtCore.Qt.black)
        self.setPen(pen)

        # Fill the path:
        path.moveTo(points[0])
        for i in xrange(len(points)-1):
            path.lineTo(points[i+1])
        self.setPath(path)


class track(recoBase):

    def __init__(self):
        super(track, self).__init__()
        self._productName = 'track'
        self._process = evd.DrawTrack()
        self.init()

    def drawObjects(self, view_manager):
        geom = view_manager._geometry

        for view in view_manager.getViewPorts():
            #   # get the showers from the process:
            self._drawnObjects.append([])
            tracks = self._process.getDataByPlane(view.plane())
            offset = geom.offset(view.plane()) / geom.time2cm()

            for track in tracks:

                # construct a polygon for this track:
                points = []
                # Remeber - everything is in cm, but the display is in
                # wire/time!
                for pair in track.track():
                    x = pair.first / geom.wire2cm()
                    y = pair.second / geom.time2cm() + offset
                    points.append(QtCore.QPointF(x, y))

                # self._drawnObjects[view.plane()].append(thisPoly)

                thisPoly = polyLine(points)
                pen = pg.mkPen((0,0,0), width=2)
                thisPoly.setPen(pen)
                # polyLine.draw(view._view)

                view._view.addItem(thisPoly)

                self._drawnObjects[view.plane()].append(thisPoly)
