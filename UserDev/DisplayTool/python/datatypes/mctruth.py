from database import recoBase
from pyqtgraph.Qt import QtGui, QtCore
from ROOT import evd, larutil
import pyqtgraph as pg

class mctruth(recoBase):

    def __init__(self):
        super(mctruth, self).__init__()
        self._productName = 'mctruth'
        self._process = evd.DrawMCTruth()
        self.init()

    def getLabel(self):
        return "this is a stupid test label."

    def drawObjects(self, view_manager):
        geom = view_manager._geometry

        # Just draw the vertex to start:
        info = self._process.getData()
        vertex = info.vertex()

        for view in view_manager.getViewPorts():
            self._drawnObjects.append([])


            point = larutil.GeometryHelper.GetME().Point_3Dto2D(vertex,view.plane())

            r = QtGui.QGraphicsRectItem(point.w, point.t, 3,10)
            self._drawnObjects[view.plane()].append(r)
            view._view.addItem(r)
            # #   # get the showers from the process:
            # tracks = self._process.getDataByPlane(view.plane())
            # offset = geom.offset(view.plane()) / geom.time2cm()

            # for i in xrange(len(tracks)):
            #     track = tracks[i]
            #     # construct a polygon for this track:
            #     points = []
            #     # Remeber - everything is in cm, but the display is in
            #     # wire/time!
            #     for pair in track.track():
            #         x = pair.first / geom.wire2cm()
            #         y = pair.second / geom.time2cm() + offset
            #         points.append(QtCore.QPointF(x, y))

            #     # self._drawnObjects[view.plane()].append(thisPoly)

            #     thisPoly = polyLine(points)
            #     pen = pg.mkPen((0,0,0), width=2)
            #     thisPoly.setPen(pen)
            #     # polyLine.draw(view._view)

            #     view._view.addItem(thisPoly)

            #     self._drawnObjects[view.plane()].append(thisPoly)

