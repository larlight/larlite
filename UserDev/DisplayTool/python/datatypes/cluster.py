from data import recoBase
from PyQt4 import QtGui, QtCore
from connectedObjects import connectedBox, connectedCircle, boxCollection
from ROOT import evd


class clusterParams(QtCore.QObject):  # recoBase):

    """docstring for clusterParams"""

    mouseEnter = QtCore.pyqtSignal(QtGui.QGraphicsSceneHoverEvent)
    mouseExit = QtCore.pyqtSignal(QtGui.QGraphicsSceneHoverEvent)
    highlightChange = QtCore.pyqtSignal()

    def __init__(self, params, geom):
        super(clusterParams, self).__init__()
        self._listOfStartPoints = []
        self._params = params
        self._geom = geom
        self._isHighlighted = False

    # Some member params i'm not filling forrectly.
    def setParams(self, params):
        self._params = params

    def hoverEnter(self, e):
        for circle in self._listOfStartPoints:
            circle.setPen(pg.mkPen((0, 0, 0), width=1))
        # When the function is called from a box, the sender is none
        # When its passed from the params, the sender is something
        if self.sender() == None:
            self.mouseEnter.emit(e)

    def hoverExit(self, e):
        if self._isHighlighted:
            return
        for circle in self._listOfStartPoints:
            circle.setPen(pg.mkPen(None))
        # When the function is called from a box, the sender is none
        # When its passed from the params, the sender is something
        if self.sender() == None:
            self.mouseExit.emit(e)

    def toggleHighlight(self):
        self._isHighlighted = not self._isHighlighted
        # When the function is called from a box, the sender is none
        # When its passed from the params, the sender is something
        if self.sender() == None:
            self.highlightChange.emit()

    # Can connect clusterparams to other clusterparams or to box collections
    def connect(self, other):
        self.mouseEnter.connect(other.hoverEnter)
        self.mouseExit.connect(other.hoverExit)
        self.highlightChange.connect(other.toggleHighlight)

    def genToolTip(self):
        return self.toolTip()

    def toolTip(self):
        tip = "Hits: " + str(self._params.N_Hits) + "\n"
        tip += "Start: (" + "{:.2f}".format(self._params.start_point.w) + ", "
        tip += "{:.2f}".format(self._params.start_point.t) + ")\n"
        tip += ("Shower Start (" +
                "{:.2f}".format(self._params.showering_point.w) + ", ")
        tip += "{:.2f}".format(self._params.showering_point.t) + ")\n"
        tip += "End: (" + "{:.2f}".format(self._params.end_point.w) + ", "
        tip += "{:.2f}".format(self._params.end_point.t) + ")\n"
        if self._params.principal_dir[0] != 0:
            slope = self._params.principal_dir[1]/self._params.principal_dir[0]
            tip += ("Slope: " + "{:.2f}".format(slope) + "\n")
        else:
            tip += "Slope: inf\n"
        if self._params.start_dir[0] != 0:
            tip += "Start Slope: " + \
                "{:.2f}".format(
                    self._params.start_dir[1]/self._params.start_dir[0]) + "\n"
        else:
            tip += "Start Slope: inf\n"
        tip += "Angle: " + "{:.2f}".format(self._params.angle_2d) + "\n"
        tip += "Add more in data.py:clusterParams:toolTip!"
        return tip

    def draw(self, view):
        # This function is responsible for all clusterparams drawing
        red = (255, 0, 0)
        green = (0, 255, 0)
        black = (0, 0, 0)
        blue = (0, 0, 255)

        offset = self._geom.offset(
            self._params.plane_id.Plane) / self._geom.time2cm()

        # Draw the start and end points:
        sW = self._params.start_point.w / self._geom.wire2cm()
        sT = (self._params.start_point.t) / self._geom.time2cm() + offset
        eW = self._params.end_point.w / self._geom.wire2cm()
        eT = (self._params.end_point.t) / self._geom.time2cm() + offset
        showeringW = self._params.showering_point.w / self._geom.wire2cm()
        showeringT = (self._params.showering_point.t) / \
            self._geom.time2cm() + offset

        radBigW = 0.5 / self._geom.wire2cm()
        radBigT = (0.5) / self._geom.time2cm()
        radSmallW = 0.25 / self._geom.wire2cm()
        radSmallT = (0.25) / self._geom.time2cm()

        bigCircleStart = connectedCircle(
            sW-radBigW, sT-radBigT, 2*radBigW, 2*radBigT)
        if self._isHighlighted:
            bigCircleStart.setPen(pg.mkPen(black))
        else:
            bigCircleStart.setPen(pg.mkPen(None))

        bigCircleStart.setBrush(pg.mkColor(green))
        bigCircleStart.setOpacity(0.6)
        bigCircleStart.connectOwnerHoverEnter(self.hoverEnter)
        bigCircleStart.connectOwnerHoverExit(self.hoverExit)
        bigCircleStart.connectToggleHighlight(self.toggleHighlight)
        bigCircleStart.connectToolTip(self.genToolTip)

        smallCircleStart = QtGui.QGraphicsEllipseItem(
            sW-radSmallW, sT-radSmallT, 2*radSmallW, 2*radSmallT)
        smallCircleStart.setPen(pg.mkPen(None))
        smallCircleStart.setBrush(pg.mkColor(black))

        bigCircleEnd = connectedCircle(
            eW-radBigW, eT-radBigT, 2*radBigW, 2*radBigT)
        bigCircleEnd.setPen(pg.mkPen(None))
        bigCircleEnd.setBrush(pg.mkColor(red))
        bigCircleEnd.setOpacity(0.6)
        bigCircleEnd.connectOwnerHoverEnter(self.hoverEnter)
        bigCircleEnd.connectOwnerHoverExit(self.hoverExit)
        bigCircleEnd.connectToggleHighlight(self.toggleHighlight)
        bigCircleEnd.connectToolTip(self.genToolTip)

        smallCircleEnd = QtGui.QGraphicsEllipseItem(
            eW-radSmallW, eT-radSmallT, 2*radSmallW, 2*radSmallT)
        smallCircleEnd.setPen(pg.mkPen(None))
        smallCircleEnd.setBrush(pg.mkColor(black))

        radW = 0.5 / self._geom.wire2cm()
        radT = (0.5) / self._geom.time2cm()
        showeringPoint = connectedCircle(
            showeringW - radW, showeringT - radT, 2*radW, 2*radT)
        showeringPoint.setBrush(pg.mkColor(blue))
        showeringPoint.setOpacity(0.6)
        showeringPoint.connectOwnerHoverEnter(self.hoverEnter)
        showeringPoint.connectOwnerHoverExit(self.hoverExit)
        showeringPoint.connectToggleHighlight(self.toggleHighlight)
        showeringPoint.connectToolTip(self.genToolTip)

        self._listOfStartPoints.append(bigCircleStart)
        self._listOfStartPoints.append(smallCircleStart)
        self._listOfStartPoints.append(bigCircleEnd)
        self._listOfStartPoints.append(smallCircleEnd)
        self._listOfStartPoints.append(showeringPoint)

        view._view.addItem(bigCircleStart)
        view._view.addItem(smallCircleStart)
        view._view.addItem(bigCircleEnd)
        view._view.addItem(smallCircleEnd)
        view._view.addItem(showeringPoint)

        ########################################
        # 2D Polygon Drawing
        ########################################
        # Not super hard to do with the poly object
        self._thisPolyF = QtGui.QPolygonF()
        for p in xrange(self._params.PolyObject.Size()):
            point = self._params.PolyObject.Point(p)
            qpoint = QtCore.QPointF(
                point.first/self._geom.wire2cm(),
                point.second/self._geom.time2cm() + offset)
            self._thisPolyF.append(qpoint)

        self._thisPoly = QtGui.QGraphicsPolygonItem(self._thisPolyF)
        pen = self._thisPoly.pen()
        pen.setStyle(QtCore.Qt.DashLine)
        self._thisPoly.setPen(pen)
        view._view.addItem(self._thisPoly)

        ########################################
        # 2D Principal Axis Drawing
        ########################################
        # Draw the axis
        # The axis is just a line between two points.
        self._axisPolygon = QtGui.QPolygonF()
        self._axisPolygon.append(QtCore.QPointF(sW, sT))
        l = self._params.length
        if l < 1:
            l = (self._params.start_point.w - self._params.end_point.w)**2
            l += (self._params.start_point.t - self._params.end_point.t)**2
            l = math.sqrt(l)

        axisEndW = sW + l*self._params.principal_dir.at(0)/self._geom.wire2cm()
        axisEndT = sT + (l*self._params.principal_dir.at(1)) / \
            self._geom.time2cm()
        # Check to see if this line needs to be reversed:
        sign = (sW - axisEndW)*(sW - eW) + (sT - axisEndT)*(sT - eT)
        if sign < 0:
            axisEndW = sW - l * \
                self._params.principal_dir.at(0)/self._geom.wire2cm()
            axisEndT = sT - \
                (l*self._params.principal_dir.at(1))/self._geom.time2cm()
        self._axisPolygon.append(QtCore.QPointF(axisEndW, axisEndT))

        self._axisPath = QtGui.QPainterPath()
        self._axisPath.addPolygon(self._axisPolygon)
        self._polyGraphicsItem = QtGui.QGraphicsPathItem(self._axisPath)
        pen = self._polyGraphicsItem.pen()
        pen.setWidth(2)
        pen.setBrush(pg.mkColor((0, 0, 0, 125)))
        self._polyGraphicsItem.setPen(pen)
        view._view.addItem(self._polyGraphicsItem)

        ########################################
        # 2D Start Axis Drawing
        ########################################
        # Draw an line to show the start direction of the shower
        self._startAxis = QtGui.QPolygonF()
        self._startAxis.append(QtCore.QPointF(sW, sT))
        startDirEndW = sW + l*self._params.start_dir.at(0)/self._geom.wire2cm()
        startDirEndT = sT + \
            (l*self._params.start_dir.at(1))/self._geom.time2cm()

        # print sign
        sign = (sW - startDirEndW)*(sW - eW) + (sT - startDirEndT)*(sT - eT)
        if sign < 0:
            startDirEndW = sW - l * \
                self._params.start_dir.at(0)/self._geom.wire2cm()
            startDirEndT = sT - \
                (l*self._params.start_dir.at(1))/self._geom.time2cm()

        self._startAxis.append(QtCore.QPointF(startDirEndW, startDirEndT))
        self._startAxisPath = QtGui.QPainterPath()
        self._startAxisPath.addPolygon(self._startAxis)
        self._startAxisPolyItem = QtGui.QGraphicsPathItem(self._startAxisPath)
        pen = self._startAxisPolyItem.pen()
        pen.setWidth(1)
        pen.setStyle(QtCore.Qt.DashLine)
        pen.setBrush(pg.mkColor((0, 0, 0, 200)))
        self._startAxisPolyItem.setPen(pen)
        view._view.addItem(self._startAxisPolyItem)

    def clear(self, view):
        # This function clears all of this cluster params
        #
        for item in self._listOfStartPoints:
            view._view.removeItem(item)
        self._listOfStartPoints = []
        view._view.removeItem(self._thisPoly)
        view._view.removeItem(self._polyGraphicsItem)
        view._view.removeItem(self._startAxisPolyItem)


class cluster(recoBase):

    """docstring for cluster"""

    def __init__(self):
        super(cluster, self).__init__()
        self._productName = 'cluster'
        self._process = evd.DrawCluster()
        self.init()

        self.setParamsDrawing(False)

        self._listOfClusters = []
        self._listOfCParams = []

        # Defining the cluster colors:
        self._clusterColors = [
            (0, 147, 147),  # dark teal
            (0, 0, 252),   # bright blue
            (156, 0, 156),  # purple
            (255, 0, 255),  # pink
            (255, 0, 0),  # red
            (175, 0, 0),  # red/brown
            (252, 127, 0),  # orange
            (102, 51, 0),  # brown
            (127, 127, 127),  # dark gray
            (210, 210, 210),  # gray
            (100, 253, 0)  # bright green
        ]

    def setParamsDrawing(self, paramsBool):
        self._drawParams = paramsBool

    # this is the function that actually draws the cluster.
    def drawObjects(self, view_manager):


        for view in view_manager.getViewPorts():
            colorIndex = 0
            # get the plane
            thisPlane = view.plane()

            # extend the list of clusters
            self._listOfClusters.append([])

            self._listOfCParams.append([])

            clusters = self._process.getDataByPlane(thisPlane)

            for cluster in clusters:

                # Now make the cluster
                cluster_box_coll = boxCollection()
                cluster_box_coll.setColor(self._clusterColors[colorIndex])
                cluster_box_coll.setPlane(thisPlane)

                # Keep track of the cluster for drawing management
                self._listOfClusters[thisPlane].append(cluster_box_coll)
                self._listOfCParams[thisPlane].append(None)

                # draw the hits in this cluster:
                cluster_box_coll.drawHits(view, cluster)

                colorIndex += 1
                if colorIndex >= len(self._clusterColors):
                    colorIndex = 0

                if self._drawParams:
                    if cluster.params().N_Hits > 10:

                        cParams = clusterParams(
                            cluster.params(), view_manager._geometry)
                        self._listOfCParams[thisPlane][-1] = cParams
                        cluster_box_coll.attachParams(cParams)
                        self._listOfCParams[thisPlane][-1].draw(view)

                        # Connect the cluster to the params:
                        self._listOfClusters[
                            thisPlane][-1].connect(self._listOfCParams[thisPlane][-1])

                        # Connect the params to the cluster:
                        self._listOfCParams[
                            thisPlane][-1].connect(self._listOfClusters[thisPlane][-1])

    def clearDrawnObjects(self, view_manager):
        i_plane = 0
        # erase the clusters
        for plane in self._listOfClusters:
            view = view_manager.getViewPorts()[i_plane]
            i_plane += 1
            for cluster in plane:
                cluster.clearHits(view)

        # Erase the cparams
        i_plane = 0
        for plane in self._listOfCParams:
            view = view_manager.getViewPorts()[i_plane]
            i_plane += 1
            for cparams in plane:
                if cparams != None:
                    cparams.clear(view)

        self._listOfClusters = []
        self._listOfCParams = []

    def getAutoRange(self, plane):
        w = self._process.getWireRange(plane)
        t = self._process.getTimeRange(plane)
        return [w.first, w.second], [t.first, t.second]

