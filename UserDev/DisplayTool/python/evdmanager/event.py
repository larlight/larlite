
from pyqtgraph.Qt import QtGui
# from data import *
# import ROOT
# from larlite import larlite as fmwk
# from ROOT import *
# import os

# This class exists as the basic interface for controlling events
# Defines a lot of functions but the final implementation needs to be done
# by a specific mode of running
# This base class is meant to provide information interface
# This class is also the manager for file interface, and contains the general functions.
# It needs to be extended independently for daq or reco viewing
class event(object):
  """docstring for event"""
  def __init__(self):
#    15-Nov-2016 WGS: The following line causes problems with PySide,
#    and it's not clear to me it's necessary in a base class. 
#    super(event, self).__init__()
    self._run = 0
    self._event = 0
    self._subrun = 0

    self._keyTable = dict()

    self._lastProcessed = -1

  def next(self):
    print("Called next event")

  def prev(self):
    print("Called prev event")

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
    print("Called next")
    # self._process.nextEvent()  


  def prev(self):
    print("Called prev")
    # self._process.prevEvent()   

  def goToEvent(self,event):
    print("Requested jump to event ", event)

  def selectFile(self):
    filePath = str(QtGui.QFileDialog.getOpenFileName())
    self.setInputFile(filePath)
    print("Selected file is ", filePath)
    return filePath

  def setInputFile(self,file):
    pass

  def hasWireData(self):
    return False

  def getAutoRange(self,plane):
    xRange = [0,self._geom.wRange(plane)]
    yRange = [0,self._geom.tRange()]
    return xRange,yRange

  def drawHitsOnWire(self,plane,wire):
    pass

