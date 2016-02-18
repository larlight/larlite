from database import recoBase
from ROOT import evd, TVector3
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore
import math as mt

# Shower drawing is currently "experimental"


class shower(recoBase):

    """docstring for shower"""

    def __init__(self):
        super(shower, self).__init__()
        self._productName = 'shower'
        self._process = evd.DrawShower()
        self.init()

        # Defining the cluster colors:
        self._showerColors = [
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

    # def clearDrawnObjects(self,view_manager):
    #   pass

    # def getAutoRange(self,plane):
    #   pass

    def drawObjects(self, view_manager):

        # Showers can get messed up so only draw "good" showers
        # This means that if either projection is bad, don't draw that shower

        for view in view_manager.getViewPorts():
            # get the showers from the process:
            self._drawnObjects.append([])

            showers = self._process.getDataByPlane(view.plane())

            i_color = 0

            for i in xrange(len(showers)):
                shower = showers[i]

                if i_color > len(self._showerColors):
                    i_color = 0

                color = self._showerColors[i_color]

                # construct a polygon for this shower:
                points = []
                # Remember - everything is in cm, but the display is in
                # wire/time!
                geom = view_manager._geometry
                x = shower.startPoint().w / geom.wire2cm()
                y = shower.startPoint().t / geom.time2cm()
                points.append(QtCore.QPoint(x, y))
                # next connect the two points at the end of the shower to make
                # a cone
                #
                # We need the vector that's perpendicular to the axis, to make the cone.
                # Use 3D vectors to allow the cross product:
                zAxis = TVector3(0, 0, 1)
                showerAxis = TVector3(shower.endPoint().w - shower.startPoint().w,
                                      shower.endPoint().t -
                                      shower.startPoint().t,
                                      0.0)
                perpAxis = zAxis.Cross(showerAxis)

                length = showerAxis.Mag() * np.tan(shower.openingAngle()/2)
                perpAxis *= length / perpAxis.Mag()


                x1, y1 = shower.endPoint().w + perpAxis.X(), shower.endPoint().t + \
                    perpAxis.Y()
                x2, y2 = shower.endPoint().w - perpAxis.X(), shower.endPoint().t - \
                    perpAxis.Y()
                # x2, y2 = shower.endPoint().w, shower.endPoint().t
                # x1 = x1 + shower.length() * \
                #     mt.cos(shower.angleInPlane() - shower.openingAngle()/2)
                # y1 = y1 + shower.length() * \
                #     mt.sin(shower.angleInPlane() - shower.openingAngle()/2)
                # x2 = x2 + shower.length() * \
                #     mt.cos(shower.angleInPlane() + shower.openingAngle()/2)
                # y2 = y2 + shower.length() * \
                #     mt.sin(shower.angleInPlane() + shower.openingAngle()/2)

                # Scale everything to wire/time:
                x1 /= geom.wire2cm()
                y1 /= geom.time2cm()
                x2 /= geom.wire2cm()
                y2 /= geom.time2cm()

                points.append(QtCore.QPoint(x1, y1))
                points.append(QtCore.QPoint(x2, y2))


                thisPolyF = QtGui.QPolygonF(points)
                thisPoly = QtGui.QGraphicsPolygonItem(thisPolyF)

                thisPoly.setPen(pg.mkPen(None))
                thisPoly.setBrush(pg.mkColor(color))

                view._view.addItem(thisPoly)
                self._drawnObjects[view.plane()].append(thisPoly)

                # if view.plane() == 0:
                #   print "dedx: ", shower.dedx()

                i_color += 1


from database import recoBase3D

try:
    import pyqtgraph.opengl as gl
    import numpy as np

    class shower3D(recoBase3D):

        """docstring for shower3D"""

        def __init__(self):
            super(shower3D, self).__init__()
            self._productName = 'shower3D'
            self._process = evd.DrawShower3D()
            self.init()

            # Defining the cluster colors:
            self._showerColors = [
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

        # def clearDrawnObjects(self,view_manager):
        #   pass

        # def getAutoRange(self,plane):
        #   pass

        def drawObjects(self, view_manager):

            geom = view_manager._geometry
            view = view_manager.getView()

            showers = self._process.getData()
            i_color = 0

            for i in xrange(len(showers)):
                shower = showers[i]

                if i_color > len(self._showerColors):
                    i_color = 0

                color = self._showerColors[i_color]

                # We need to know the outer radius of the cylinder at the end
                # of the shower

                radius = shower.length() * np.tan(shower.opening_angle()/2)

                # Just be stupid and try to draw something:
                cylinderPoints = gl.MeshData.cylinder(
                    2, 50, radius=[0, radius], length=shower.length())
                cylinder = gl.GLMeshItem(meshdata=cylinderPoints,
                                         drawEdges=False,
                                         shader='shaded',
                                         color=color,
                                         glOptions='opaque')
                # We need to get this cyliner's axis to match the actual axis
                # we can rotate it around an axis.
                # Find the axis to rotate it around, which is the axis perpendicular to it's current axis
                # and the axis .
                # Since the axis the cylinder is made along is the z axis, we
                # can compute the cross product easily:
                crossx = - shower.direction().Y()
                crossy = shower.direction().X()
                crossz = 0

                # We need the angle to rotate it by, too:
                # Compute it with the dot product
                angle = np.arccos(
                    shower.direction().Z()/shower.direction().Mag())

                cylinder.rotate((180.0/np.pi)*angle, crossx, crossy, crossz)

                # Last, translate the shower to it's actual start point:
                #

                cylinder.translate(
                    shower.start_point().X(), shower.start_point().Y(), shower.start_point().Z())

                # print "Start point: ({x},{y},{z})".format(x=shower.start_point().X(),
                #                                           y=shower.start_point().Y(),
                #                                           z=shower.start_point().Z())

                # print "Direction  : ({x},{y},{z})".format(x=shower.direction().X(),
                #                                           y=shower.direction().Y(),
                #                                           z=shower.direction().Z())

                # cylinder.setGLOptions("additive")
                view.addItem(cylinder)

                self._drawnObjects.append(cylinder)

                i_color += 1


except Exception, e:
    pass
