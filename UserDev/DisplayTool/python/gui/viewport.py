
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import numpy as np
import math


class viewport(pg.GraphicsLayoutWidget):

  drawHitsRequested = QtCore.Signal(int, int)

  def __init__(self, geometry,plane=-1):
    super(viewport, self).__init__(border=None)
    # add a view box, which is a widget that allows an image to be shown
    self._view = self.addViewBox(border=None)
    # add an image item which handles drawing (and refreshing) the image
    self._item = pg.ImageItem(useOpenGL=True)
    # self._item._setPen((0,0,0))
    self._view.addItem(self._item)
    # connect the scene to click events, used to get wires
    self.scene().sigMouseClicked.connect(self.mouseClicked)
    # connect the views to mouse move events, used to update the info box at the bottom
    self.scene().sigMouseMoved.connect(self.mouseMoved)
    self._plane = plane
    self._cmSpace = False
    self._geometry = geometry

    self._dataPoints = []
    self._drawnPoints = []
    self._polygon = QtGui.QPolygonF()
    self._path = QtGui.QPainterPath()
    self._path.addPolygon(self._polygon)
    self._polyGraphicsItem = QtGui.QGraphicsPathItem(self._path)
    self._view.addItem(self._polyGraphicsItem)

    # Connect scale changes to handle the scale bar correctly
    self._view.sigYRangeChanged.connect(self.scaleHandler)
    self._view.sigXRangeChanged.connect(self.scaleHandler)
    self._xBar = None
    self._xBarText = None
    # self._yBar = None
    # self._yBarText = None
    self.useScaleBar = False

    # Set up the blank data:
    # self._blankData = np.ones((self._geometry.wRange(self._plane),self._geometry.tRange()))
    self.setBackground('w')

    self._useLogo = False
    self._logo = None

    # each drawer contains its own color gradient and levels
    # this class can return a widget containing the right layout for everything
    # Define some color collections:


    self._colorMap = self._geometry.colorMap(self._plane)


    self._cmap = pg.GradientWidget(orientation='right')
    self._cmap.restoreState(self._colorMap)
    self._cmap.sigGradientChanged.connect(self.refreshGradient)
    self._cmap.resize(1,1)

    # These boxes control the levels.
    self._upperLevel = QtGui.QLineEdit()
    self._lowerLevel = QtGui.QLineEdit()

    self._upperLevel.returnPressed.connect(self.levelChanged)
    self._lowerLevel.returnPressed.connect(self.levelChanged)

    self._lowerLevel.setText(str(self._geometry.getLevels(self._plane)[0]))
    self._upperLevel.setText(str(self._geometry.getLevels(self._plane)[1]))


    # Fix the maximum width of the widgets:
    self._upperLevel.setMaximumWidth(35)
    self._cmap.setMaximumWidth(25)
    self._lowerLevel.setMaximumWidth(35)

    colors = QtGui.QVBoxLayout()
    colors.addWidget(self._upperLevel)
    colors.addWidget(self._cmap)
    colors.addWidget(self._lowerLevel)
    self._totalLayout = QtGui.QHBoxLayout()
    self._totalLayout.addWidget(self)
    self._totalLayout.addLayout(colors)
    self._widget = QtGui.QWidget()
    self._widget.setLayout(self._totalLayout)

  def toggleScale(self,scaleBool):
    # If there is a scale, remove it:
    if self._xBar in self._view.addedItems:
      self._view.removeItem(self._xBar)
      self._view.removeItem(self._xBarText)
    # if self._yBar in self._view.addedItems:
    #   self._view.removeItem(self._yBar)
    #   self._view.removeItem(self._yBarText)
    self.useScaleBar = scaleBool
    self.refreshScaleBar()

  def toggleLogo(self,logoBool):

    if self._logo in self.scene().items():
        self.scene().removeItem(self._logo)

    self._useLogo = logoBool
    self.refreshLogo()

  def refreshLogo(self):
    if not self._useLogo:
      return

    self._logo = QtGui.QGraphicsPixmapItem(QtGui.QPixmap(self._geometry.logo()))
    self.scene().addItem(self._logo)
    self._logo.setX(self._geometry.logoPos()[0])
    self._logo.setY(self._geometry.logoPos()[1])
    self._logo.setScale(self._geometry.logoScale())


    # self._logo.scale(1.0*(width/rectW),-1*(height/rectH))
    # self._view.addItem(self._logo)

    # self._logo.setY(0.7)


  def restoreDefaults(self):
    self._lowerLevel.setText(str(self._geometry.getLevels(self._plane)[0]))
    self._upperLevel.setText(str(self._geometry.getLevels(self._plane)[1]))

    self._cmap.restoreState(self._colorMap)

  def mouseDrag(self):
    print "mouse was dragged"

  def getWidget(self):

    return self._widget,self._totalLayout

  def levelChanged(self):
    # First, get the current values of the levels:
    lowerLevel = int(self._lowerLevel.text())
    upperLevel = int(self._upperLevel.text())

    # set the levels as requested:
    levels = (lowerLevel, upperLevel)
    # next, set the levels in the geometry:
    # self._geometry._levels[self._plane] = (lowerLevel,upperLevel)
    # last, update the levels in the image:
    self._item.setLevels(levels)

  def refreshGradient(self):
    self._item.setLookupTable(self._cmap.getLookupTable(255))

  def useCM(self,useCMBool):
    self._cmSpace = useCMBool
    self.refreshScaleBar()

  def mouseMoved(self, pos):
    self.q = self._item.mapFromScene(pos)
    self._lastPos = self.q
    if (pg.Qt.QT_LIB == 'PyQt4'):
      message= QtCore.QString()
    else:
      message= str()
    if self._cmSpace:
      if type(message) != str:
        message.append("X: ")
        message.append("{0:.1f}".format(self.q.x()*self._geometry.wire2cm()))
      else:
        message += "X: "
        message += "{0:.1f}".format(self.q.x()*self._geometry.wire2cm())
    else:
      if type(message) != str:
        message.append("W: ")
        message.append(str(int(self.q.x())))
      else:
        message += "W: "
        message += str(int(self.q.x()))
    if self._cmSpace:
      if type(message) != str:
        message.append(", Y: ")
        message.append("{0:.1f}".format(self.q.y()*self._geometry.time2cm() - self._geometry.offset(self._plane)))
      else:
        message += ", Y: "
        message += "{0:.1f}".format(self.q.y()*self._geometry.time2cm() - self._geometry.offset(self._plane))
    else:
      if type(message) != str:
        message.append(", T: ")
        message.append(str(int(self.q.y())))
      else:
        message += ", T: "
        message += str(int(self.q.y()))

    # print message
    if self.q.x() > 0 and self.q.x() < self._geometry.wRange(self._plane):
      if self.q.y() > 0 and self.q.y() < self._geometry.tRange():
        self._statusBar.showMessage(message)

  def mouseClicked(self, event):
    # print self
    # print event
    # print event.pos()
    # Get the Mouse position and print it:
    # print "Image position:", self.q.x()
    # use this method to try drawing rectangles
    # self.drawRect()
    # pdi.plot()
    # For this function, a click should get the wire that is
    # being hovered over and draw it at the bottom
    if event.modifiers() == QtCore.Qt.ShiftModifier:
      if event.pos() is not  None:
        self.processPoint(self._lastPos)

    # 
    wire = int( self._lastPos.x())
    if self._item.image is not None:
      # get the data from the plot:
      data = self._item.image
      self._wireData = data[wire]
      self._wdf(self._wireData)
      # print "Plane: " + str(self._plane) + ", Wire: " + str(wire)
      # return self.plane,self.wire

    # Make a request to draw the hits from this wire:
    self.drawHitsRequested.emit(self._plane,wire)


  def connectWireDrawingFunction(self,func):
    self._wdf = func

  def connectStatusBar(self, _statusBar):
    self._statusBar = _statusBar

  def setRangeToMax(self):
    xR = (0,self._geometry.wRange(self._plane))
    yR = (0,self._geometry.tRange())
    self._view.setRange(xRange=xR,yRange=yR, padding=0.002)

  def autoRange(self,xR,yR):
    self._view.setRange(xRange=xR,yRange=yR, padding=0.002)
    pass


  def scaleHandler(self):
    # print self.sender()
    if self.useScaleBar:
      self.refreshScaleBar()


  def refreshScaleBar(self):
    if not self.useScaleBar:
      return
    # First, get the range in x and y:
    # [[xmin, xmax], [ymin, ymax]]
    dims = self._view.viewRange()

    # The view bars get drawn on 10% of the lengths
    # if ratio lock is set, only draw X

    # Draw the X bar:
    xMin = dims[0][0]
    xMax = dims[0][1]
    yMin = dims[1][0]
    yMax = dims[1][1]
    width = 0.1*(xMax - xMin)
    height = 0.01*(yMax - yMin)
    xLoc = xMin + 0.1*(xMax - xMin)
    yLoc = yMin + 0.1*(yMax - yMin)


    if self._xBar in self._view.addedItems:
      self._view.removeItem(self._xBar)
      self._view.removeItem(self._xBarText)

    self._xBar = QtGui.QGraphicsRectItem(xLoc,yLoc,width,height)
    self._xBar.setBrush(pg.mkColor(255,255,255))
    self._view.addItem(self._xBar)

    xString = ""
    if self._cmSpace:
      xString = "{0:.0f}".format(round(width*self._geometry.wire2cm()))
      xString = xString + " cm"
    else:
      xString = "{0:.0f}".format(round(width))
      xString = xString + " wires"


    # Add the text:
    self._xBarText = QtGui.QGraphicsSimpleTextItem(xString)
    self._xBarText.setBrush(pg.mkColor(255,255,255))
    xScale = 0.015* width
    yScale = - 0.5* height
    self._xBarText.setPos(xLoc,yLoc)
    self._xBarText.scale(xScale,yScale)
    self._xBarText.font().setPixelSize(15)
    self._view.addItem(self._xBarText)

    # # Now do the y Bar
    # width = 0.01*(xMax - xMin)
    # height = 0.1*(yMax - yMin)
    # xLoc = xMin + 0.1*(xMax - xMin)
    # yLoc = yMin + 0.1*(yMax - yMin)
    # if self._yBar in self._view.addedItems:
    #   self._view.removeItem(self._yBar)
    #   self._view.removeItem(self._yBarText)

    # self._yBar = QtGui.QGraphicsRectItem(xLoc,yLoc,width,height)
    # self._yBar.setBrush(pg.mkColor(0,0,0))
    # self._view.addItem(self._yBar)

    # # Add the text:
    # self._yBarText = QtGui.QGraphicsSimpleTextItem(xString)
    # xScale = 0.015* width
    # yScale = - 0.5* height
    # self._yBarText.setPos(xLoc,yLoc)
    # self._yBarText.scale(xScale,yScale)
    # self._yBarText.setRotation(90)
    # self._yBarText.font().setPixelSize(15)
    # self._view.addItem(self._yBarText)

  def plane(self):
    return self._plane

  def lockRatio(self, lockAR ):
    ratio = self._geometry.aspectRatio()
    if lockAR:
      self._view.setAspectLocked(True, ratio=self._geometry.aspectRatio())
    else:
      self._view.setAspectLocked(False)

  def drawPlane(self, image):
    self._item.setImage(image,autoLevels=False)
    self._item.setLookupTable(self._cmap.getLookupTable(255))
    self._cmap.setVisible(True)
    self._upperLevel.setVisible(True)
    self._lowerLevel.setVisible(True)
    self._item.setVisible(False)
    self._item.setVisible(True)
    # Make sure the levels are actually set:
    self.levelChanged()

  def drawBlank(self):
    self._item.clear()
    self._cmap.setVisible(False)
    self._upperLevel.setVisible(False)
    self._lowerLevel.setVisible(False)


  def clearPoints(self):
    for point in self._drawnPoints:
      self._view.removeItem(point)

      self._drawnPoints = []
      self._dataPoints = []
      self._polygon.clear()
      self._path = QtGui.QPainterPath()
      self._polyGraphicsItem.setPath(self._path)

  def makeIonizationPath(self):


    if len(self._dataPoints) < 2:
      return None

    if self._item.image is None:
      return None

    data = self._item.image
    totalpath = np.empty(0)

    for p in xrange(len(self._dataPoints) - 1):
      start = int(round(self._dataPoints[p].x())), int(round(self._dataPoints[p].y()))
      end =  int(round(self._dataPoints[p+1].x())), int(round(self._dataPoints[p+1].y()))
      line = self.get_line(start,end)
      path = np.zeros([len(line)])
      for i in xrange(len(line)):
        pt = line[i]
        path[i] = data[pt[0]][pt[1]]
      # print line
      totalpath = np.concatenate((totalpath,path))

    return totalpath




  def processPoint(self,_in_point):


    # Check if this point is close to another point (less than some dist)
    i = 0
    for point in self._drawnPoints:
      if point.contains(_in_point):
        self._dataPoints.pop(i)
        self._polygon.remove(i)
        self._view.removeItem(self._drawnPoints[i])
        self._drawnPoints.pop(i)
        # Refresh the path:
        self._path = QtGui.QPainterPath()
        self._path.addPolygon(self._polygon)
        self._polyGraphicsItem.setPath(self._path)
        return
      i +=1

    # Point wasn't popped, so add it to the list
    self._dataPoints.append(_in_point)
    r = QtGui.QGraphicsEllipseItem(_in_point.x()-1, _in_point.y()-10, 2,20)
    r.setBrush(pg.mkColor((0,0,0)))
    self._view.addItem(r)
    self._drawnPoints.append(r)

    # Refresh the polygon and then update the path
    self._polygon.append(_in_point)

    # self._polyGraphicsItem.setPolygon(self._polygon)
    self._path = QtGui.QPainterPath()
    self._path.addPolygon(self._polygon)
    self._polyGraphicsItem.setPath(self._path)
    

  # Lovingly stolen from wikipedia, this is not my algorithm
  def get_line(self, start, end):
    """Bresenham's Line Algorithm
    Produces a list of tuples from start and end
 
    >>> points1 = get_line((0, 0), (3, 4))
    >>> points2 = get_line((3, 4), (0, 0))
    >>> assert(set(points1) == set(points2))
    >>> print points1
    [(0, 0), (1, 1), (1, 2), (2, 3), (3, 4)]
    >>> print points2
    [(3, 4), (2, 3), (1, 2), (1, 1), (0, 0)]
    """
    # Setup initial conditions
    x1, y1 = start
    x2, y2 = end
    dx = x2 - x1
    dy = y2 - y1
 
    # Determine how steep the line is
    is_steep = abs(dy) > abs(dx)
 
    # Rotate line
    if is_steep:
        x1, y1 = y1, x1
        x2, y2 = y2, x2
 
    # Swap start and end points if necessary and store swap state
    swapped = False
    if x1 > x2:
        x1, x2 = x2, x1
        y1, y2 = y2, y1
        swapped = True
 
    # Recalculate differentials
    dx = x2 - x1
    dy = y2 - y1
 
    # Calculate error
    error = int(dx / 2.0)
    ystep = 1 if y1 < y2 else -1
 
    # Iterate over bounding box generating points between start and end
    y = y1
    points = []
    for x in range(x1, x2 + 1):
        coord = (y, x) if is_steep else (x, y)
        points.append(coord)
        error -= abs(dy)
        if error < 0:
            y += ystep
            error += dx
 
    # Reverse the list if the coordinates were swapped
    if swapped:
        points.reverse()
    return points
