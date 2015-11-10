from database import recoBase
from ROOT import evd
import pyqtgraph as pg
from PyQt4 import QtCore, QtGui
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
        print ""

        for view in view_manager.getViewPorts():
            # get the showers from the process:
            self._drawnObjects.append([])

            showers = self._process.getDataByPlane(view.plane())

            i_color = 0

            for shower in showers:

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
                x1, y1 = shower.startPoint().w, shower.startPoint().t
                x2, y2 = shower.startPoint().w, shower.startPoint().t
                x1 = x1 + shower.length() * \
                    mt.cos(shower.angleInPlane() - shower.openingAngle()/2)
                y1 = y1 + shower.length() * \
                    mt.sin(shower.angleInPlane() - shower.openingAngle()/2)
                x2 = x2 + shower.length() * \
                    mt.cos(shower.angleInPlane() + shower.openingAngle()/2)
                y2 = y2 + shower.length() * \
                    mt.sin(shower.angleInPlane() + shower.openingAngle()/2)

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


