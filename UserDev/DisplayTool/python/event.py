
from PyQt4.QtGui import QFileDialog
from PyQt4 import QtCore
from data import *
from larlite import larlite as fmwk
from ROOT import *
import os

# This class exists as the basic interface for controlling events
# Defines a lot of functions but the final implementation needs to be done
# by a specific mode of running
# This base class is meant to provide information interface
# This class is also the manager for file interface, and contains the general functions.
# It needs to be extended independently for daq or reco viewing
class event(object):
  """docstring for event"""
  def __init__(self):
    super(event, self).__init__()
    self._run = 0
    self._event = 0
    self._subrun = 0

    self._lastProcessed = -1

  def next(self):
    print "Called next event"

  def prev(self):
    print "Called prev event"

  def subrun(self):
    return self._subrun

  def run(self):
    return self._run

  def event(self):
    return self._event

  def setRun(self, r):
    self._run = r

  def setEvent(self,e):
    self._event = e

  def setSubRun(self, s):
    self._subrun = s



class manager(event):
  """docstring for manager"""
  def __init__(self, geom,file=None):
    super(manager, self).__init__()
    self._geom = geom
    
    # The manager needs to know about the gui and the view manager
    # It needs to be able to call a few of their methods to update things
    self._gui = None
    self._view_manager = None
    self._hasFile = False

  def connectGui(self,gui):
    self._gui = gui

  def connectViewManager(self,view_manager):
    self._view_manager = view_manager

  def next(self):
    print "Called next"
    # self._process.nextEvent()  


  def prev(self):
    print "Called prev"
    # self._process.prevEvent()   

  def goToEvent(self,event):
    print "Requested jump to event ", event

  def selectFile(self):
    filePath = str(QFileDialog.getOpenFileName())
    self.setInputFile(filePath)
    print "Selected file is ", filePath
    return filePath

  def setInputFile(self,file):
    pass

  def hasWireData(self):
    return False

  def getAutoRange(self,plane):
    xRange = [0,self._geom.wRange(plane)]
    yRange = [0,self._geom.tRange()]
    return xRange,yRange




class larlite_manager(manager,QtCore.QObject):
  fileChanged = QtCore.pyqtSignal()
  eventChanged = QtCore.pyqtSignal()
  clusterParamsChanged = QtCore.pyqtSignal(bool)

  """docstring for lariat_manager"""
  def __init__(self, geom, file=None):
    super(larlite_manager, self).__init__(geom,file)
    manager.__init__(self,geom,file)
    QtCore.QObject.__init__(self)
    # For the larlite manager, need both the ana_processor and 
    # the storage manager
    self._process = fmwk.ana_processor()
    self._mgr = fmwk.storage_manager()
    self._drawableItems = drawableItems()
    self._keyTable = dict()
    self._drawnClasses = dict()

    self.setInputFile(file)

    # Toggle whether or not to draw wires:
    self._drawWires = False
    self._drawParams = False
    self._wireDrawer = None

    # Lariat has special meanings to event/spill/run
    self._spill = 0

  def pingFile(self,file):
    """this function opens the file and determines what is available to draw"""
    # This function opens the file to see
    # what data products are available
    
    # Open the file
    f = TFile(file)
    # Use the larlite_id_tree to find out how many entries are in the file:
    self._n_entries = f.larlite_id_tree.GetEntries()
    # prepare a dictionary of data products
    lookUpTable = dict()
    # Loop over the keys (list of trees)
    for key in f.GetListOfKeys():
      # keys are dataproduct_producer_tree
      thisKeyList = key.GetName().split('_')
      # gets three items in thisKeyList, which is a list
      # [dataProduct, producer, 'tree'] (don't care about 'tree')
      # check if the data product is in the dict:
      if thisKeyList[0] in lookUpTable:
        # extend the list:
        lookUpTable[thisKeyList[0]] += (thisKeyList[1], )
      else:
        lookUpTable.update( {thisKeyList[0] : (thisKeyList[1],)})

    self._keyTable = lookUpTable

  def setInputFile(self,file):
    # First, check that the file exists:
    try:
      if not os.path.exists(file):
        print "ERROR: requested file does not exist."
        return
    except Exception, e:
      return
    # Next, verify it is a root file:
    if not file.endswith(".root"):
      print "ERROR: must supply a root file."
      return
    # Finally, ping the file to see what is available to draw
    self.pingFile(file)
    if len(self._keyTable) > 0:
      self._hasFile = True
      # add this file to the storage manager here
      self._mgr.reset()
      self._mgr.add_in_filename(file)
      self._mgr.set_io_mode(fmwk.storage_manager.kREAD)
      self._mgr.open()
      # setup the processor in the same way
      self._process.reset()
      self._process.add_input_file(file)
      self._process.set_io_mode(fmwk.storage_manager.kREAD)

      self._lastProcessed = -1
      self.goToEvent(0)
      self.fileChanged.emit()


  # This function will return all producers for the given product
  def getProducers(self,product):
    try:
      return self._keyTable[product]
    except:
      return None

  # This function returns the list of products that can be drawn:
  def getDrawableProducts(self):
    return self._drawableItems.getListOfItems()


  # override the run,event,subrun functions:
  def run(self):
    if not self._mgr.is_open():
      return 0
    return self._mgr.run_id()

  def event(self):
    if not self._mgr.is_open():
      return 0

    return self._mgr.event_id()

  def subrun(self):
    if not self._mgr.is_open():
      return 0

    return self._mgr.subrun_id()

  # override the functions from manager as needed here
  def next(self):
    # print "Called next"
    # Check that this isn't the last event:
    if self._event < self._n_entries - 1:
      self.goToEvent(self._event + 1)
    else:
      print "On the last event, can't go to next."

  def prev(self):
    if self._event != 0:
      self.goToEvent(self._event - 1) 
    else:
      print "On the first event, can't go to previous."

  # this function is meant for the first request to draw an object or
  # when the producer changes
  def redrawProduct(self,product,producer,view_manager):
    # print "Received request to redraw ", product, " by ",producer
    # First, determine if there is a drawing process for this product:
    if producer == None:
      if product in self._drawnClasses:
        self._drawnClasses[product].clearDrawnObjects(self._view_manager)
        self._drawnClasses.pop(product)
      return
    if product in self._drawnClasses:
      self._drawnClasses[product].setProducer(producer)
      self.processEvent(True)
      self._drawnClasses[product].clearDrawnObjects(self._view_manager)
      self._drawnClasses[product].drawObjects(self._view_manager)
      return


    # Now, draw the new product
    if product in self._drawableItems.getListOfItems():
      # drawable items contains a reference to the class, so instantiate it
      drawingClass = self._drawableItems.getDict()[product]()
      # Special case for clusters, connect it to the signal:
      if product == 'cluster':
        self.clusterParamsChanged.connect(drawingClass.setParamsDrawing)
        drawingClass.setParamsDrawing(self._drawParams)

      drawingClass.setProducer(producer)
      self._process.add_process(drawingClass._process)
      self._drawnClasses.update({product : drawingClass})
      # Need to process the event
      self.processEvent(True)
      drawingClass.drawObjects(self._view_manager)

  def processEvent(self,force = False):
    if len(self._drawnClasses) == 0 and not self._drawWires:
      self._mgr.go_to(self._event)
      return
    if self._lastProcessed != self._event or force:
      self._process.process_event(self._event)
      self._mgr.go_to(self._event)
      self._lastProcessed = self._event

  def goToEvent(self,event,force = False):
    self.setEvent(event)
    self.processEvent()
    self.clearAll()
    if self._view_manager != None:
      self._view_manager.drawPlanes(self)
    self.drawFresh()
    self.eventChanged.emit()

  def clearAll(self):
    for recoProduct in self._drawnClasses:
      self._drawnClasses[recoProduct].clearDrawnObjects(self._view_manager)


  def drawFresh(self):
    # # wires are special:
    # if self._drawWires:
    #   self._view_manager.drawPlanes(self)
    self.clearAll()
    # Draw objects in a specific order defined by drawableItems
    order = self._drawableItems.getListOfItems()
    for item in order:
      if item in self._drawnClasses:
        self._drawnClasses[item].drawObjects(self._view_manager)

  def getAutoRange(self,plane):
    # This gets the max bounds
    xRangeMax,yRangeMax = super(larlite_manager,self).getAutoRange(plane)
    xRange = xRangeMax
    yRange = yRangeMax
    for process in self._drawnClasses:
      x,y = self._drawnClasses[process].getAutoRange(plane)
      # Check against all four of the parameters:
      if x != None:
        if x[0] > xRange[0]:
          xRange[0] = x[0]
        if x[1] < xRange[1]:
          xRange[1] = x[1]
      if y != None:
        if y[0] > yRange[0]:
          yRange[0] = y[0]
        if y[1] < yRange[1]:
          yRange[1] = y[1]

    # Pad the ranges by 1 cm to accommodate 
    padding = 1
    xRange[0] = min(xRangeMax[0],xRange[0] - padding/self._geom.wire2cm())
    xRange[1] = max(xRangeMax[1],xRange[1] + padding/self._geom.wire2cm())
    yRange[0] = min(yRangeMax[0],yRange[0] - padding/self._geom.time2cm())
    yRange[1] = max(yRangeMax[1],yRange[1] + padding/self._geom.time2cm())
    return xRange,yRange

  # handle all the wire stuff:
  def toggleWires(self, wiresBool):
    # Now, either add the drawing process or remove it:
    if 'wire' not in self._keyTable:
      print "No wire data available to draw"
      return
    self._drawWires = wiresBool

    if self._drawWires:
      self._wireDrawer = recoWire()
      self._process.add_process(self._wireDrawer._process)
      self.processEvent(True)
    else:
      self._wireDrawer = None   

  def toggleParams(self, paramsBool):
    self._drawParams = paramsBool
    self.clusterParamsChanged.emit(paramsBool)
    if 'cluster' in self._drawnClasses:
      self.drawFresh()

  def getPlane(self,plane):
    if self._drawWires:
      return self._wireDrawer.getPlane(plane)


  def hasWireData(self):
    if self._drawWires:
      return True
    else:
      return False    