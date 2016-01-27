from database import recoBase
from pyqtgraph.Qt import QtGui, QtCore
from ROOT import evd
import pyqtgraph as pg


# class polyLine(QtGui.QGraphicsPathItem):

#     def __init__(self, points, pen=None):
#         super(polyLine, self).__init__()
#         self._points = points

#         # Initialize a path:
#         path = QtGui.QPainterPath()
#         if pen is None:
#             pen = QtGui.QPen(QtCore.Qt.black)
#         self.setPen(pen)

#         # Fill the path:
#         path.moveTo(points[0])
#         for i in xrange(len(points)-1):
#             path.lineTo(points[i+1])
#         self.setPath(path)


# class track(recoBase):

#     def __init__(self):
#         super(track, self).__init__()
#         self._productName = 'track'
#         self._process = evd.DrawTrack()
#         self.init()

#     def drawObjects(self, view_manager):
#         geom = view_manager._geometry

#         for view in view_manager.getViewPorts():
#             #   # get the showers from the process:
#             self._drawnObjects.append([])
#             tracks = self._process.getDataByPlane(view.plane())
#             offset = geom.offset(view.plane()) / geom.time2cm()

#             for i in xrange(len(tracks)):
#                 track = tracks[i]
#                 # construct a polygon for this track:
#                 points = []
#                 # Remeber - everything is in cm, but the display is in
#                 # wire/time!
#                 for pair in track.track():
#                     x = pair.first / geom.wire2cm()
#                     y = pair.second / geom.time2cm() + offset
#                     points.append(QtCore.QPointF(x, y))

#                 # self._drawnObjects[view.plane()].append(thisPoly)

#                 thisPoly = polyLine(points)
#                 pen = pg.mkPen((0,0,0), width=2)
#                 thisPoly.setPen(pen)
#                 # polyLine.draw(view._view)

#                 view._view.addItem(thisPoly)

#                 self._drawnObjects[view.plane()].append(thisPoly)


from database import recoBase3D

try:
    import pyqtgraph.opengl as gl
    import numpy as np
    class mctrack3D(recoBase3D):

        def __init__(self):
            super(mctrack3D, self).__init__()
            self._productName = 'mctrack3D'
            self._process = evd.DrawMCTrack3D()
            self.init()
            self._mesh = gl.MeshData()

        def drawObjects(self, view_manager):
            geom = view_manager._geometry
            view = view_manager.getView()


            tracks = self._process.getData()


            for track in tracks:

                # construct a line for this track:
                points = track.track()
                x = np.zeros(points.size())
                y = np.zeros(points.size())
                z = np.zeros(points.size())
                # x = numpy.ndarray()
                # x = numpy.ndarray()
                i = 0
                for point in points:
                    x[i] = point.X()
                    y[i] = point.Y()
                    z[i] = point.Z()
                    i+= 1

                pts = np.vstack([x,y,z]).transpose()
                pen = pg.mkPen((255,0,0), width=2)
                line = gl.GLLinePlotItem(pos=pts,color=(255,255,0,255))
                view.addItem(line)
                self._drawnObjects.append(line)

    
    # # Just be stupid and try to draw something:
    # cylinderPoints = gl.MeshData.cylinder(2,50,radius=[0,1],length=1)
    # cylinder = gl.GLMeshItem(meshdata=cylinderPoints,drawEdges=False,shader='shaded', glOptions='opaque')
    # cylinder.scale(10,10,10)
    # # cylinder.setGLOptions("additive")
    # self.addItem(cylinder)


except Exception, e:
    pass

