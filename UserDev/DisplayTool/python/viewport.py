
from PyQt4 import QtGui, QtCore
import pyqtgraph as pg
import numpy as np


class viewport(pg.GraphicsLayoutWidget):
  def __init__(self, geometry,plane=-1):
    super(viewport, self).__init__()
    # add a view box, which is a widget that allows an image to be shown
    self._view = self.addViewBox(border=(0,0,0))
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

    # Set up the blank data:
    self._blankData = np.ones((self._geometry.wRange(self._plane),self._geometry.tRange()))

    self.setBackground('w')


    # each drawer contains its own color gradient and levels
    # this class can return a widget containing the right layout for everything
    # Define some color collections:
    self._blankMapCollection    = {'ticks': [(0, (255, 255, 255, 255)), 
                                             (1, (255, 255, 255, 255))], 
                                             'mode': 'rgb'}
    self._daqColorMapCollection = {'ticks': [(0.0,  (30,  30, 255, 255)),
                                             (0.15, (30,  30, 255, 255)), 
                                             (0.6,  (0,  255, 255, 255)), 
                                             (0.8,  (0,  255, 0,   255)), 
                                             (1,    (255,  0, 0,   255))], 
                                             'mode': 'rgb'}
    self._colorMapCollection    = {'ticks': [(0, (30, 30, 255, 255)),
                                             (0.33333, (0, 255, 255, 255)), 
                                             (0.66666, (255,255,100,255)), 
                                             (1, (255, 0, 0, 255))], 
                                             'mode': 'rgb'}

    self._cmap = pg.GradientWidget(orientation='right')
    self._cmap.sigGradientChanged.connect(self.refreshGradient)
    self._cmap.resize(1,1)

    self._blankMap = pg.GradientWidget()
    self._blankMap.restoreState(self._blankMapCollection)

    self._cmap.restoreState(self._colorMapCollection)
    self._item.setLookupTable(self._cmap.getLookupTable(255))

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

  def restoreDefaults(self):
    self._lowerLevel.setText(str(self._geometry.getLevels(self._plane)[0]))
    self._upperLevel.setText(str(self._geometry.getLevels(self._plane)[1]))

    self._cmap.restoreState(self._colorMapCollection)
    # self._item.setLookupTable(self._cmap.getLookupTable(255))

  def getWidget(self):

    colors = QtGui.QVBoxLayout()
    colors.addWidget(self._upperLevel)
    colors.addWidget(self._cmap)
    colors.addWidget(self._lowerLevel)
    total = QtGui.QHBoxLayout()
    total.addWidget(self)
    total.addLayout(colors)
    widget = QtGui.QWidget()
    widget.setLayout(total)
    return widget

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

  def mouseMoved(self, pos):
    self.q = self._item.mapFromScene(pos)
    message= QtCore.QString()
    if self._cmSpace:
      message.append("X: ")
      message.append("{0:.1f}".format(self.q.x()*self._geometry.wire2cm()))
    else:
      message.append("W: ")
      message.append(str(int(self.q.x())))
    if self._cmSpace:
      message.append(", Y: ")
      message.append("{0:.1f}".format(self.q.y()*self._geometry.time2cm()))
    else:
      message.append(", T: ")
      message.append(str(int(self.q.y())))
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
    wire = int( self._item.mapFromScene(event.pos()).x())
    # get the data from the plot:
    data = self._item.image
    wireData = data[wire]
    self._wdf(wireData)
    # print "Plane: " + str(self._plane) + ", Wire: " + str(wire)
    # return self.plane,self.wire

  def connectWireDrawingFunction(self,func):
    self._wdf = func

  def connectStatusBar(self, _statusBar):
    self._statusBar = _statusBar

  def setRangeToMax(self):
    xR = (0,self._geometry.wRange(self._plane))
    yR = (0,self._geometry.tRange())
    self._view.setRange(xRange=xR,yRange=yR, padding=0.002)

  def autoRange(self):
    pass

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
    self._item.setImage(self._blankData)
    self._item.setLookupTable(self._blankMap.getLookupTable(255))
    self._cmap.setVisible(False)
    self._upperLevel.setVisible(False)
    self._lowerLevel.setVisible(False)
