
try:
    import pyqtgraph.opengl as gl
except:
    print "Error, must have open gl to use this viewer."
    exit(-1)

from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import numpy as np
import math


class viewport3D(gl.GLViewWidget):

  quitRequested = QtCore.pyqtSignal()
  keyPressSignal = QtCore.pyqtSignal(QtGui.QKeyEvent)

  def __init__(self, geometry):
    super(viewport3D, self).__init__()
    # add a view box, which is a widget that allows an image to be shown
    # add an image item which handles drawing (and refreshing) the image
    self.setBackgroundColor((50,50,50,255))
    self._geometry = geometry
    gy = gl.GLGridItem()
    gy.scale(0.1*self._geometry.length(),0.1*self._geometry.length(),0.1*self._geometry.length())
    gy.rotate(90, 1, 0, 0)
    gy.translate(0,-self._geometry.halfheight(), 0)
    self.addItem(gy)

    self.buildDetector()


    if self._geometry.name() == "uboone":
      self.buildPaddleBox1()
      self.buildPaddleBox2()


    self.pan(self._geometry.halfwidth(), 0, 0.5*self._geometry.length())

    # Set the camera to the correct starting position:
    pos = (self._geometry.halfwidth(),5*self._geometry.halfheight(),0.5*geometry.length())
    self.setCameraPos(pos=pos)

    # print self.cameraPosition()
    # self.orbit(0,180)
    # print self.cameraPosition()
    # self.orbit(225,0)
    # print self.cameraPosition()



    # self.pan(0,0,self._geometry.length())
    self.show()

  def setCenter(self, center):
    if len(center) != 3:
        return
    cVec = QtGui.QVector3D(center[0],center[1],center[2])
    self.opts['center'] = cVec
    self.update()


  def worldCenter(self):
    return self.opts['center']

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

  def getAzimuth(self):
    return self.opts['azimuth']


  def getElevation(self):
    return self.opts['elevation']

  def setCameraPos(self,pos):
    # calling set camera with pos doesn't actually do anything.  Convert spherical coordinates:
    if pos is not None:
        # Convert to relative coordinates to always leave the world center as the center point
        worldCenter = self.opts['center']
        # Check the type:
        if type(worldCenter) is QtGui.QVector3D:
            X = pos[0] - worldCenter.x()
            Y = pos[1] - worldCenter.y()
            Z = pos[2] - worldCenter.z()
        else:
            X = pos[0] - worldCenter[0]
            Y = pos[1] - worldCenter[1]
            Z = pos[2] - worldCenter[2]

        distance = X**2 + Y**2 + Z**2
        distance = math.sqrt(distance)
        if X != 0:
            azimuth = math.atan2(Y,X)
        else:
            azimuth = math.pi
            if Y < 0:
                azimuth = -1 * azimuth
        if distance != 0:
            elevation = math.asin(Z / distance)
        else:
            elevation = math.copysign(Z)
        azimuth *= 180./math.pi
        elevation *= 180./math.pi
        self.setCameraPosition(distance=distance,elevation=elevation,azimuth=azimuth)


  def keyPressEvent(self,e):
    if e.key() == QtCore.Qt.Key_C:
        # print "C was pressed"
        if e.modifiers() and QtCore.Qt.ControlModifier :
            self.quitRequested.emit()
            return
    elif e.modifiers():
        if QtCore.Qt.ShiftModifier :
            if e.key() == QtCore.Qt.Key_Up:
                # This is supposed to pan upwards in the view
                self.pan(0,20,0,True)
            if e.key() == QtCore.Qt.Key_Down:
                self.pan(0,-20,0,True)
            if e.key() == QtCore.Qt.Key_Left:
                self.pan(20,0,0,True)
            if e.key() == QtCore.Qt.Key_Right:
                self.pan(-20,0,0,True)
    else:
        super(viewport3D,self).keyPressEvent(e)
   

    # Pass this signal to the main gui, too
    self.keyPressSignal.emit(e)


  def drawLine(self,point1,point2):

    x = np.linspace(point1[0],point2[0],100)
    y = np.linspace(point1[1],point2[1],100)
    z = np.linspace(point1[2],point2[2],100)

    pts = np.vstack([x,y,z]).transpose()
    line = gl.GLLinePlotItem(pos=pts)
    self.addItem(line)

  # def mouseMoveEvent(self, pos):
    # print pos

