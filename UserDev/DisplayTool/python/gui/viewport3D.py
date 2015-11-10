
try:
    import pyqtgraph.opengl as gl
except:
    print "Error, must have open gl to use this viewer."
    exit(-1)

from PyQt4 import QtGui, QtCore
import pyqtgraph as pg
import numpy as np
import math


class viewport3D(gl.GLViewWidget):

  keyPressSignal = QtCore.pyqtSignal(QtGui.QKeyEvent)

  def __init__(self, geometry):
    super(viewport3D, self).__init__()
    # add a view box, which is a widget that allows an image to be shown
    # add an image item which handles drawing (and refreshing) the image
    # self._item._setPen((0,0,0))
    self.opts['distance'] = 1000
    # g = gl.GLGridItem()
    # g.scale(300, 300, 1000)
    # self.addItem(g)

    self._geometry = geometry
    self.buildDetector()


    if self._geometry.name() == "uboone":
      self.buildPaddleBox1()
      self.buildPaddleBox2()

    # Set the camera to the correct starting position:
    pos = (4*geometry.halfwidth(),2*geometry.halfheight(),-2*geometry.length())
    self.setCameraPosition(pos=pos, distance=None, elevation=None, azimuth=None)

    # print self.cameraPosition()
    # self.orbit(0,180)
    # print self.cameraPosition()
    # self.orbit(225,0)
    # print self.cameraPosition()


    # self.pan(0,0,self._geometry.length())
    self.show()

  def keyPressEvent(self,e):
    self.keyPressSignal.emit(e)


  def buildDetector(self):

    # Get the coordinates of the detector
    halfwidth = self._geometry.halfwidth()
    halfheight = self._geometry.halfheight()
    length = self._geometry.length()

    self.drawBox(0,-halfheight,0,2*halfwidth,halfheight,length)


  def buildPaddleBox1(self):
    self.drawBox(-71.795, 398.351, 531.45, -23.695, 398.451, 579.45)

  def buildPaddleBox2(self):
    self.drawBox(-19.6948, 320.05, 533.25, 28.3052, 320.15, 581.25)

  def drawBox(self, xmin, ymin, zmin, xmax, ymax, zmax):

    # Draw lines from each corner to the
    # 3 corners it connects with
    self.drawLine((xmin,ymin,zmin),(xmin,ymax,zmin))
    self.drawLine((xmin,ymin,zmin),(xmax,ymin,zmin))
    self.drawLine((xmax,ymin,zmin),(xmax,ymax,zmin))
    self.drawLine((xmin,ymax,zmin),(xmax,ymax,zmin))

    self.drawLine((xmin,ymin,zmax),(xmin,ymax,zmax))
    self.drawLine((xmin,ymin,zmax),(xmax,ymin,zmax))
    self.drawLine((xmax,ymin,zmax),(xmax,ymax,zmax))
    self.drawLine((xmin,ymax,zmax),(xmax,ymax,zmax))

    self.drawLine((xmin,ymin,zmin),(xmin,ymin,zmax))
    self.drawLine((xmax,ymax,zmin),(xmax,ymax,zmax))
    self.drawLine((xmin,ymax,zmin),(xmin,ymax,zmax))
    self.drawLine((xmax,ymin,zmin),(xmax,ymin,zmax))

    return

  def drawLine(self,point1,point2):

    x = np.linspace(point1[0],point2[0],100)
    y = np.linspace(point1[1],point2[1],100)
    z = np.linspace(point1[2],point2[2],100)

    pts = np.vstack([x,y,z]).transpose()
    line = gl.GLLinePlotItem(pos=pts)
    self.addItem(line)

  # def mouseMoveEvent(self, pos):
    # print pos

