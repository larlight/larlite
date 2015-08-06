
from PyQt4 import QtGui, QtCore
from larlite import larlite as fmwk
from ROOT import *
import numpy as np
import pyqtgraph as pg


# These classes provide the basic interfaces for drawing objects
# It's meant to be inherited from for specific instances
# There is a class for drawing the following things:
#   - Raw wire data
#   - hits
#   - clusters
#   - nothing else ... yet

# To add a class, what you have to do is write a module to process it (larlite ana_processor)
# Put your class in RecoViewer or wherever.  I use the namespace evd for all of these classes
# so if you use something else, be sure to import it above from ROOT

# Your class needs to include getter methods for the data you need, and a setProducer function
# which sets the producer of the data type.  Then, write a python class that inherits from 
# recoBase below.  Your class must override the function drawObjects to display it's data
# on the views.  Your class is responsible for maintaining the items being drawn, and must 
# clear them with the clearDrawnObjects function.  Store drawn objects in _drawnObjects.

# Lastly, extend the drawableItems class with one line:
# self._drawableClasses.update({'dataProduct':yourPythonClassName})
# This gives the viewer the connection between the class you made and the product you want to draw

# Beyond that, inside your larlite or python class you can do pretty much whatever you want
# Run merging, matching, ertool, who cares.




# This is the class that maintains the list of drawable items.
# If your class isn't here, it can't be drawn
import collections
class drawableItems(object):
  """This class exists to enumerate the drawableItems"""
  # If you make a new drawing class, add it here
  def __init__(self):
    super(drawableItems,self).__init__()
    # items are stored as pointers to the classes (not instances)
    self._drawableClasses = collections.OrderedDict()
    self._drawableClasses.update({'hit':hit})
    self._drawableClasses.update({'cluster':cluster})
    self._drawableClasses.update({'shower':shower})
    self._drawableClasses.update({'track':track})

  def getListOfItems(self):
    return self._drawableClasses.keys()

  def getDict(self):
    return self._drawableClasses


class dataBase(object):
  """basic class from which data objects inherit"""
  def __init__(self):
    super(dataBase,self).__init__()
    self._productName = "null"
    self._producerName = "null"
    self._process = None
    
    # This is the (clunky) converter to native python
    self._c2p = evd.Converter()

  def productName(self):
    return self._productName

  def producerName(self):
    return self._producerName

  def setProducer(self,name):
    self._producerName = name


# Reco base, all reco objects must inherit from this
class recoBase(dataBase):
  """docstring for recoBase"""
  def __init__(self):
    super(recoBase, self).__init__()
    self._drawnObjects = []
    self._process = None

  def init(self):
    self._process.initialize()


  def clearDrawnObjects(self,view_manager):
    for view in view_manager.getViewPorts():
      thisPlane = view.plane()
      for hit in self._drawnObjects[thisPlane]:
        view._view.removeItem(hit)

    # clear the list:
    self._drawnObjects = [] 

  # override set producer
  def setProducer(self, producer):
    self._producerName = producer
    self._process.setProducer(str(producer))

  def getDrawnObjects(self):
    return self._drawnObjects

  def drawObjects(self,view_manager):
    pass

  def getAutoRange(self,plane):
    return None, None






class wire(dataBase):
  """docstring for wire"""
  def __init__(self):
    super(wire, self).__init__()
    self._process = None
    
    # This is the (clunky) converter to native python
    self._c2p = evd.Converter()

  def get_img(self):
    d = []
    for i in range(0,self._nviews):
      d.append(np.array(self._c2p.Convert(self._process.getDataByPlane(i))) )
      # print "got a plane, here is a sample: ", d[i][0][0]
    return d

  def getPlane(self,plane):

    b = self._process.getNumpyByPlane(plane)
    print type(b)
    return b
    
  def getWire(self, plane, wire):
    return np.array(self._c2p.Convert(self._process.getWireData(plane,wire)))

class recoWire(wire):
  def __init__(self):
    super(recoWire,self).__init__()
    self._process = evd.DrawRaw()
    self._process.initialize()



class hit(recoBase):
  """docstring for hit"""
  def __init__(self):
    super(hit, self).__init__()
    self._productName = 'hit'
    self._process = evd.DrawHit()
    self._brush = (0,0,0)
    self.init()

  # this is the function that actually draws the hits.
  def drawObjects(self,view_manager):
  
    for view in view_manager.getViewPorts():
      self._drawnObjects.append([])
      thisPlane = view.plane()
      # First get the hit information:
      wireList       = self._c2p.Convert(self._process.getWireByPlane(thisPlane) )
      timeStartList  = self._c2p.Convert(self._process.getHitStartByPlane(thisPlane) )
      timeEndList    = self._c2p.Convert(self._process.getHitEndByPlane(thisPlane) )

      for i in xrange(len(wireList)):
        # Draws a rectangle at (x,y,xlength, ylength)
        r = QtGui.QGraphicsRectItem(wireList[i], timeStartList[i], 1, timeEndList[i]-timeStartList[i])

        # # New Way
        # r.setPen(pg.mkPen(brush,width=2))
        # # r.setBrush(pg.mkColor((255,255,255)))

        # Old Way:
        r.setPen(pg.mkPen(None))
        r.setBrush(pg.mkColor(self._brush))
        self._drawnObjects[thisPlane].append(r)
        view._view.addItem(r)


class circleThing(QtGui.QGraphicsEllipseItem):

  def __init__(self, *args, **kwargs):
    super(circleThing, self).__init__(*args)
#    self.setAcceptHoverEvents(True)
#    self._isHighlighted = False



# This class wraps the hit object to allow them to all function together
class connectedBox(QtGui.QGraphicsRectItem):
  """docstring for connectedBox"""
  def __init__(self, *args, **kwargs):
    super(connectedBox, self).__init__(*args)
    self.setAcceptHoverEvents(True)
    self._isHighlighted = False
      

  def hoverEnterEvent(self, e):
    self.setToolTip(self._ownerToolTip())
    self._ownerHoverEnter(e)

  def hoverLeaveEvent(self,e):
    self._ownerHoverExit(e) 

  def mouseDoubleClickEvent(self,e):
    self._toggleHighlight()

  def connectOwnerHoverEnter(self, ownerHoverEnter):
    self._ownerHoverEnter = ownerHoverEnter

  def connectOwnerHoverExit(self, ownerHoverExit):
    self._ownerHoverExit = ownerHoverExit

  def connectToggleHighlight(self, ownerTH):
    self._toggleHighlight = ownerTH

  def connectToolTip(self, ownerToolTip):
    self._ownerToolTip = ownerToolTip

class connectedCircle(QtGui.QGraphicsEllipseItem):
  """docstring for connectedCircle"""
  def __init__(self, *args, **kwargs):
    super(connectedCircle, self).__init__(*args,**kwargs)
    self.setAcceptHoverEvents(True)
    self._isHighlighted = False

  def hoverEnterEvent(self, e):
    self.setToolTip(self._ownerToolTip())
    self._ownerHoverEnter(e)

  def hoverLeaveEvent(self,e):
    self._ownerHoverExit(e) 

  def mouseDoubleClickEvent(self,e):
    self._toggleHighlight()

  def connectOwnerHoverEnter(self, ownerHoverEnter):
    self._ownerHoverEnter = ownerHoverEnter

  def connectOwnerHoverExit(self, ownerHoverExit):
    self._ownerHoverExit = ownerHoverExit

  def connectToggleHighlight(self, ownerTH):
    self._toggleHighlight = ownerTH

  def connectToolTip(self, ownerToolTip):
    self._ownerToolTip = ownerToolTip  


class boxCollection(QtCore.QObject):
  # This class wraps a collection of hits and connects them together
  # it can draw and delete itself when provided with view_manage
  # 
  # Provide some signals to communicate with cluster params
  mouseEnter  = QtCore.pyqtSignal(QtGui.QGraphicsSceneHoverEvent)
  mouseExit   = QtCore.pyqtSignal(QtGui.QGraphicsSceneHoverEvent)
  highlightChange = QtCore.pyqtSignal()

  def __init__(self):
    super(boxCollection, self).__init__()
    self._color = (0,0,0)
    self._plane = -1
    self._listOfHits = []
    self._listOfStarts = []
    self._isHighlighted = False
    self._params = None

  def setColor(self,color):
    self._color = color

  def setPlane(self,plane):
    self._plane = plane

  def attachParams(self,params):
    self._params = params

  def genToolTip(self):
    if self._params == None:
      nhits = len(self._listOfHits)
      tip = "Hits: " + str(nhits)  
      return tip
    else:
      return self._params.toolTip()

  def hoverEnter(self, e):
    for hit in self._listOfHits:
      hit.setPen(pg.mkPen((0,0,0),width=1))
    # When the function is called from a box, the sender is none
    # When its passed from the params, the sender is something
    if self.sender() == None:
      self.mouseEnter.emit(e)

  def hoverExit(self, e):
    if self._isHighlighted:
      return
    for hit in self._listOfHits:
      hit.setPen(pg.mkPen(None))
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


  def drawHits(self,view,wireList,timeStartList,timeEndList):
    for i in xrange(len(wireList)):
      # Draws a rectangle at (x,y,xlength, ylength)
      r = connectedBox(wireList[i], timeStartList[i], 1, timeEndList[i]-timeStartList[i])
      r.setPen(pg.mkPen(None))
      r.setBrush(pg.mkColor(self._color))
      self._listOfHits.append(r)
      view._view.addItem(r)

      # Connect the hit's actions with the clusters functions
      r.connectOwnerHoverEnter(self.hoverEnter)
      r.connectOwnerHoverExit(self.hoverExit)
      r.connectToggleHighlight(self.toggleHighlight)
      r.connectToolTip(self.genToolTip)

  def clearHits(self,view):
    for hit in self._listOfHits:
      view._view.removeItem(hit)
    self._listOfHits = []


class clusterParams(QtCore.QObject): #recoBase):
  """docstring for clusterParams"""
  
  mouseEnter  = QtCore.pyqtSignal(QtGui.QGraphicsSceneHoverEvent)
  mouseExit   = QtCore.pyqtSignal(QtGui.QGraphicsSceneHoverEvent)
  highlightChange = QtCore.pyqtSignal()

  def __init__(self,params,geom):
    super(clusterParams, self).__init__()
    self._listOfStartPoints = []
    self._params = params
    self._geom = geom
    self._isHighlighted = False

  #Some member params i'm not filling forrectly.
  def setParams(self,params):
    self._params = params



  def hoverEnter(self, e):
    for circle in self._listOfStartPoints:
      circle.setPen(pg.mkPen((0,0,0),width=1))
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

  def genToolTip(self):
    return self.toolTip()

  def toolTip(self):
    tip = "Hits: \t" + str(self._params.N_Hits) + "\n"
    tip += "Start:\t(" + "{:.2f}".format(self._params.start_point.w) + ", "
    tip += "{:.2f}".format(self._params.start_point.t) + ")\n"
    tip += "End:  \t(" + "{:.2f}".format(self._params.end_point.w) + ", "
    tip += "{:.2f}".format(self._params.end_point.t) + ")\n"
    tip += "Add more in data.py:clusterParams:toolTip!"
    return tip

  def draw(self, view):
    # This function is responsible for all clusterparams drawing
    red   = (255,0  ,0) 
    green = (0  ,255,0)  
    black = (0  ,0  ,0)
    # Draw the start and end points:
    sW = self._params.start_point.w / self._geom.wire2cm()
    sT = self._params.start_point.t / self._geom.time2cm()
    eW = self._params.end_point.w / self._geom.wire2cm()
    eT = self._params.end_point.t / self._geom.time2cm()


    bigCircleStart = connectedCircle(sW-1,sT-20,2,40)
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


    smallCircleStart = QtGui.QGraphicsEllipseItem(sW-0.2,sT-5,0.4,10)
    smallCircleStart.setPen(pg.mkPen(None))
    smallCircleStart.setBrush(pg.mkColor(black))

    bigCircleEnd = connectedCircle(eW-1,eT-20,2,40)
    bigCircleEnd.setPen(pg.mkPen(None))
    bigCircleEnd.setBrush(pg.mkColor(red))
    bigCircleEnd.setOpacity(0.6)
    bigCircleEnd.connectOwnerHoverEnter(self.hoverEnter)
    bigCircleEnd.connectOwnerHoverExit(self.hoverExit)
    bigCircleEnd.connectToggleHighlight(self.toggleHighlight)
    bigCircleEnd.connectToolTip(self.genToolTip)


    smallCircleEnd = QtGui.QGraphicsEllipseItem(eW-0.2,eT-5,0.4,10)
    smallCircleEnd.setPen(pg.mkPen(None))
    smallCircleEnd.setBrush(pg.mkColor(black))

    self._listOfStartPoints.append(bigCircleStart)
    self._listOfStartPoints.append(smallCircleStart)
    self._listOfStartPoints.append(bigCircleEnd)
    self._listOfStartPoints.append(smallCircleEnd)

    view._view.addItem(bigCircleStart)
    view._view.addItem(smallCircleStart)
    view._view.addItem(bigCircleEnd)
    view._view.addItem(smallCircleEnd)



  def clear(self,view):
    # This function clears all of this cluster params
    # 
    for item in self._listOfStartPoints:
      view._view.removeItem(item)
    self._listOfStartPoints = []




class cluster(recoBase):
  """docstring for cluster"""
  def __init__(self):
    super(cluster, self).__init__()
    self._productName = 'cluster'
    self._process = evd.DrawCluster()
    self.init()

    self.setParamsDrawing(False)

    self._listOfClusters = []
    self._listOfCParams  = []


    # Defining the cluster colors:
    self._clusterColors = [ 
                            (0,147,147),  # dark teal
                            (0,0,252),   # bright blue
                            (156,0,156), # purple
                            (255,0,255), # pink
                            (255,0,0),  #red
                            (175,0,0),  #red/brown
                            (252,127,0), # orange
                            (102,51,0), # brown
                            (127,127,127),  # dark gray
                            (210,210,210),  # gray
                            (100,253,0) # bright green
                          ]   

  def setParamsDrawing(self, paramsBool):
    self._drawParams = paramsBool

  # this is the function that actually draws the cluster.
  def drawObjects(self,view_manager):


    totalClus = 0 
    for view in view_manager.getViewPorts():
      colorIndex = 0
      # get the plane
      thisPlane = view.plane()

      # extend the list of clusters
      self._listOfClusters.append([])
      
      self._listOfCParams.append([])

      params_v = self._process.getParamsByPlane(thisPlane)

      # loop over the clusters in this plane:
      for i_cluster in xrange(self._process.getNClustersByPlane(thisPlane)):

        totalClus += 1 

        wireList       = self._c2p.Convert(self._process.getWireByPlaneAndCluster(thisPlane,i_cluster))
        timeStartList  = self._c2p.Convert(self._process.getHitStartByPlaneAndCluster(thisPlane,i_cluster))
        timeEndList    = self._c2p.Convert(self._process.getHitEndByPlaneAndCluster(thisPlane,i_cluster))


        # Now make the cluster
        cluster = boxCollection()
        cluster.setColor(self._clusterColors[colorIndex])
        cluster.setPlane(thisPlane)

        # draw the hits in this cluster:
        cluster.drawHits(view,wireList,timeStartList,timeEndList)

        self._listOfClusters[thisPlane].append(cluster)
        self._listOfCParams[thisPlane].append(None)


        colorIndex += 1
        if colorIndex >= len(self._clusterColors):
          colorIndex = 0

        if self._drawParams:
          if int(i_cluster) < int(len(params_v)) and params_v[i_cluster].N_Hits > 10:

            cParams = clusterParams(params_v[i_cluster],view_manager._geometry) 
            self._listOfCParams[thisPlane][-1] = cParams
            cluster.attachParams(cParams)
            self._listOfCParams[thisPlane][-1].draw(view)

            # Connect the params to the cluster:
            self._listOfCParams[thisPlane][-1].mouseEnter.connect(self._listOfClusters[thisPlane][-1].hoverEnter)
            self._listOfCParams[thisPlane][-1].mouseExit.connect(self._listOfClusters[thisPlane][-1].hoverExit)
            self._listOfCParams[thisPlane][-1].highlightChange.connect(self._listOfClusters[thisPlane][-1].toggleHighlight)
            self._listOfClusters[thisPlane][-1].mouseEnter.connect(self._listOfCParams[thisPlane][-1].hoverEnter)
            self._listOfClusters[thisPlane][-1].mouseExit.connect(self._listOfCParams[thisPlane][-1].hoverExit)
            self._listOfClusters[thisPlane][-1].highlightChange.connect(self._listOfCParams[thisPlane][-1].toggleHighlight)	

  def clearDrawnObjects(self,view_manager):
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
    self._listOfCParams  = []


  def getAutoRange(self,plane):
    wires = self._process.GetWireRange(plane)
    times = self._process.GetTimeRange(plane)
    return [wires[0],wires[1]],[times[0],times[1]]



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
                            (0,147,147, 100),  # dark teal
                            (0,0,252, 100),   # bright blue
                            (156,0,156, 100), # purple
                            (255,0,255, 100), # pink
                            (255,0,0, 100),  #red
                            (175,0,0, 100),  #red/brown
                            (252,127,0, 100), # orange
                            (102,51,0, 100), # brown
                            (127,127,127, 100),  # dark gray
                            (210,210,210, 100),  # gray
                            (100,253,0, 100) # bright green
                          ]   

  # def clearDrawnObjects(self,view_manager):
  #   pass

  # def getAutoRange(self,plane):
  #   pass

  def drawObjects(self,view_manager):
    
    # Showers can get messed up so only draw "good" showers
    # This means that if either projection is bad, don't draw that shower
    print ""

    for view in view_manager.getViewPorts():
      # get the showers from the process:
      self._drawnObjects.append([])

      showers = self._process.getShowersByPlane(view.plane())

      i_color = 0

      for shower in showers:

        if i_color > len(self._showerColors):
          i_color = 0

        color = self._showerColors[i_color]

        # construct a polygon for this shower:
        points = []
        # Remeber - everything is in cm, but the display is in wire/time!
        geom = view_manager._geometry
        x = shower.startPoint().X() / geom.wire2cm()
        y = shower.startPoint().Y() / geom.time2cm()
        points.append(QtCore.QPoint(x,y))
        # next connect the two points at the end of the shower to make a cone
        x1, y1 = shower.startPoint().X(), shower.startPoint().Y()
        x2, y2 = shower.startPoint().X(), shower.startPoint().Y()
        x1 = x1 + shower.length() * cos(shower.angleInPlane() - shower.openingAngle()/2)
        y1 = y1 + shower.length() * sin(shower.angleInPlane() - shower.openingAngle()/2)
        x2 = x2 + shower.length() * cos(shower.angleInPlane() + shower.openingAngle()/2)
        y2 = y2 + shower.length() * sin(shower.angleInPlane() + shower.openingAngle()/2)

        # Scale everything to wire/time:
        x1 /= geom.wire2cm()
        y1 /= geom.time2cm()
        x2 /= geom.wire2cm()
        y2 /= geom.time2cm()

        points.append(QtCore.QPoint(x1,y1))
        points.append(QtCore.QPoint(x2,y2))

        thisPolyF = QtGui.QPolygonF(points)
        thisPoly = QtGui.QGraphicsPolygonItem(thisPolyF)
        
        thisPoly.setPen(pg.mkPen(None))
        thisPoly.setBrush(pg.mkColor(color))

        view._view.addItem(thisPoly)
        self._drawnObjects[view.plane()].append(thisPoly)

        # if view.plane() == 0:
        #   print "dedx: ", shower.dedx()


        i_color += 1



class polyLine(QtGui.QGraphicsPathItem):

  def __init__(self, points, pen = None):
    super(polyLine,self).__init__()
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



  def drawObjects(self,view_manager):
    
    for view in view_manager.getViewPorts():
    #   # get the showers from the process:
      self._drawnObjects.append([])
      tracks = self._process.getTracksByPlane(view.plane())
      for track in tracks:

        # construct a polygon for this track:
        points = []
        # Remeber - everything is in cm, but the display is in wire/time!
        for pair in track:
          x = pair.first
          y = pair.second
          points.append(QtCore.QPointF(x,y))

        # self._drawnObjects[view.plane()].append(thisPoly)

        thisPoly = polyLine(points)
        # polyLine.draw(view._view)

        view._view.addItem(thisPoly)


        self._drawnObjects[view.plane()].append(thisPoly)
