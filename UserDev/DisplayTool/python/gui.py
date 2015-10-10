

import sys, signal
import argparse
import collections
from PyQt4 import QtGui, QtCore
import pyqtgraph as pg
import numpy as np

# Import the class that manages the view windows
from viewport import viewport

# Import the basic event management class
from event import event
from PyQt4 import QtCore

class view_manager(QtCore.QObject):
  """This class manages a collection of viewports"""
  def __init__(self, geometry):
    super(view_manager, self).__init__()
    self._nviews = 0
    self._drawerList = []
    self._cmapList = []
    self._geometry = geometry

    self._wireDrawer = pg.GraphicsLayoutWidget()
    self._wireDrawer.setBackground(None)
    self._wirePlot = self._wireDrawer.addPlot()
    # self._wirePlot.setPen((0,0,0))
    self._wirePlotItem = pg.PlotDataItem(pen=(0,0,0))
    # self._wirePlotItem.setBac
    self._wirePlot.addItem(self._wirePlotItem)
    self._wireDrawer.setMaximumHeight(200)
    self._wireDrawer.setMinimumHeight(100)

    self._selectedPlane = -1

    self._autoRange = False


  def addEvdDrawer(self,plane):
    self._drawerList.append(viewport(self._geometry, plane))
    self._drawerList[-1].connectWireDrawingFunction(self.drawWireOnPlot)
    self._nviews += 1
  
  def selectPlane(self,plane):
    self._selectedPlane = plane


  def restoreDefaults(self):
    for view in self._drawerList:
      view.restoreDefaults()

  def clearPoints(self):
    for view in self._drawerList:
      view.clearPoints()

  def getDrawListWidget(self):

    self._widgetList = []

    # loop through the list and add the drawing windows and their scale
    self._widget = QtGui.QWidget()
    self._layout = QtGui.QVBoxLayout()
    self._layout.setSpacing(0)
    self._layout.setMargin(0)
    self._layout.setContentsMargins(0,0,0,0)

    self._planeWidgets = []
    for view in self._drawerList:
      widget,layout = view.getWidget()
      self._planeWidgets.append(widget)
      self._layout.addWidget(widget,0)

    self._widget.setLayout(self._layout)

    return self._widget

  def refreshDrawListWidget(self):

    if self._selectedPlane == -1:
      for widget in self._planeWidgets:
        widget.setVisible(True)

    else:
      i = 0
      for widget in self._planeWidgets:
        if i == self._selectedPlane:
          widget.setVisible(True)
        else:
          widget.setVisible(False)
        i += 1


  def connectStatusBar(self,statusBar):
    for view in self._drawerList:
      view.connectStatusBar(statusBar)


  def linkViews(self):
    pass
  #   print "linking views"
  #   self._lockYRange = True
  #   self._drawerList[0]._view.sigYRangeChanged.connect(self.rangeChangeHandler)
  #   self._drawerList[1]._view.sigYRangeChanged.connect(self.rangeChangeHandler)
  #   self._drawerList[2]._view.sigYRangeChanged.connect(self.rangeChangeHandler)
  #   self._wirePlot.sigXRangeChanged.connect(self.rangeChangeHandler)




  # def rangeChangeHandler(self):
  #   range = self.sender().range
  #   if self._lockYRange:
  #     for view in self._drawerList:
  #       if view._view != self.sender():
  #         view._view.setRange
  #   print "range changed by ", self.sender()

  def setRangeToMax(self):
    for view in self._drawerList:
      view.setRangeToMax()

  def autoRange(self,event_manager):
    for view in self._drawerList:
      xRange,yRange = event_manager.getAutoRange(view.plane())
      view.autoRange(xRange,yRange)

  def lockAR(self, lockRatio):
    for view in self._drawerList:
      view.lockRatio(lockRatio)

  def makePath(self):
    for view in self._drawerList:
      path = view.makeIonizationPath()
      if path != None:
        self.drawWireOnPlot(path)
        return

  def drawWire(self,wireView):
    if wireView:
      self._layout.addWidget(self._wireDrawer)
      self._wireDrawer.setVisible(True)
    else:
      self._layout.removeWidget(self._wireDrawer)
      self._wireDrawer.setVisible(False)

  def useCM(self,useCM):
    for view in self._drawerList:
      view.useCM(useCM)


  def drawPlanes(self,event_manager):
    for i in xrange(len(self._drawerList)):
      if event_manager.hasWireData():
        self._drawerList[i].drawPlane(event_manager.getPlane(i))
      else:
        self._drawerList[i].drawBlank()

 
  def drawWireOnPlot(self, wireData):
    # Need to draw a wire on the wire view
    # Don't bother if the view isn't active:
    if not self._wireDrawer.isVisible():
      return
    else:
      # set the display to show the wire:
      self._wirePlotItem.setData(wireData)

  def getViewPorts(self):
    return self._drawerList


class gui(QtGui.QWidget):

  def __init__(self, geometry,manager):
    super(gui, self).__init__()

    # initUI should not do ANY data handling, it should only get the interface loaded
    self._geometry = geometry
    self._event_manager = manager
    self._view_manager = view_manager(geometry)
    self._event_manager.connectGui(self)
    self._event_manager.connectViewManager(self._view_manager)
    # self.setStyleSheet("background-color:rgb(230,230,230);")

  def closeEvent(self, event):
    self.quit()  

  def quit(self):
    # if self._running:
      # self.stopRun()
    QtCore.QCoreApplication.instance().quit()


  def update(self):
    # set the text boxes correctly:
    self._larliteEventEntry.setText(str(self._event_manager.internalEvent()))

    eventLabel = "Ev: " + str(self._event_manager.event())
    self._eventLabel.setText(eventLabel)
    runLabel = "Run: " + str(self._event_manager.run())
    self._runLabel.setText(runLabel)
    subrunLabel = "Subrun: " + str(self._event_manager.subrun())
    self._subrunLabel.setText(subrunLabel)
    
    self._view_manager.drawPlanes(self._event_manager)
    self.autoRangeWorker()

  # This function prepares the buttons such as prev, next, etc and returns a layout
  def getEventControlButtons(self):

    # This is a box to allow users to enter an event (larlite numbering)
    self._goToLabel = QtGui.QLabel("Go to: ")
    self._larliteEventEntry = QtGui.QLineEdit()
    self._larliteEventEntry.setToolTip("Enter an event to skip to that event (larlite numbering")
    self._larliteEventEntry.returnPressed.connect(self.goToEventWorker)
    # These labels display current events
    self._runLabel = QtGui.QLabel("Run: 0")
    self._eventLabel = QtGui.QLabel("Ev.: 0")
    self._subrunLabel = QtGui.QLabel("Subrun: 0")

    # Jump to the next event
    self._nextButton = QtGui.QPushButton("Next")
    # self._nextButton.setStyleSheet("background-color: red")
    self._nextButton.clicked.connect(self._event_manager.next)
    self._nextButton.setToolTip("Move to the next event.")
    # Go to the previous event
    self._prevButton = QtGui.QPushButton("Previous")
    self._prevButton.clicked.connect(self._event_manager.prev)
    self._prevButton.setToolTip("Move to the previous event.")
    # Select a file to use
    self._fileSelectButton = QtGui.QPushButton("Select File")
    self._fileSelectButton.clicked.connect(self._event_manager.selectFile)
    
    # pack the buttons into a box
    self._eventControlBox = QtGui.QVBoxLayout()

    # Make a horiztontal box for the event entry and label:
    self._eventGrid = QtGui.QHBoxLayout()
    self._eventGrid.addWidget(self._goToLabel)
    self._eventGrid.addWidget(self._larliteEventEntry)
    # Another horizontal box for the run/subrun
    self._runSubRunGrid = QtGui.QHBoxLayout()
    self._runSubRunGrid.addWidget(self._eventLabel)
    self._runSubRunGrid.addWidget(self._runLabel)
    # Pack it all together
    self._eventControlBox.addLayout(self._eventGrid)
    self._eventControlBox.addLayout(self._runSubRunGrid)
    self._eventControlBox.addWidget(self._subrunLabel)
    self._eventControlBox.addWidget(self._nextButton)
    self._eventControlBox.addWidget(self._prevButton)
    self._eventControlBox.addWidget(self._fileSelectButton)

    return self._eventControlBox
  

  # this function helps pass the entry of the line edit item to the event control
  def goToEventWorker(self):
    try:
      event = int(self._larliteEventEntry.text())
    except:
      print "Error, must enter an integer"
      self._larliteEventEntry.setText(str(self._event_manager.event()))
      return
    self._event_manager.goToEvent(event)

  # This function prepares the range controlling options and returns a layout
  def getDrawingControlButtons(self):

    # Button to set range to max
    self._maxRangeButton = QtGui.QPushButton("Max Range")
    self._maxRangeButton.setToolTip("Set the range of the viewers to show the whole event")
    self._maxRangeButton.clicked.connect(self._view_manager.setRangeToMax)
    # Check box to active autorange
    self._autoRangeBox = QtGui.QCheckBox("Auto Range")
    self._autoRangeBox.setToolTip("Set the range of the viewers to the regions of interest")
    self._autoRangeBox.setTristate(False)
    self._autoRangeBox.stateChanged.connect(self.autoRangeWorker)  

    self._lockAspectRatio = QtGui.QCheckBox("Lock A.R.")
    self._lockAspectRatio.setToolTip("Lock the aspect ratio to 1:1")
    self._lockAspectRatio.stateChanged.connect(self.lockARWorker)

    # check box to toggle the wire drawing
    self._drawWireOption = QtGui.QCheckBox("Wire Drawing")
    self._drawWireOption.setToolTip("Draw the wires when clicked on")
    self._drawWireOption.stateChanged.connect(self.drawWireWorker)
    self._drawRawOption = QtGui.QCheckBox("Draw Raw")
    self._drawRawOption.setToolTip("Draw the raw wire signals in 2D")
    self._drawRawOption.setTristate(False)

    # add a box to restore the drawing defaults:
    self._restoreDefaults = QtGui.QPushButton("Restore Defaults")
    self._restoreDefaults.setToolTip("Restore the drawing defaults of the views.")
    self._restoreDefaults.clicked.connect(self.restoreDefaultsWorker)

    self._unitDisplayOption = QtGui.QCheckBox("Use cm")
    self._unitDisplayOption.setToolTip("Display the units in cm (checked = true)")
    self._unitDisplayOption.setTristate(False)
    self._unitDisplayOption.stateChanged.connect(self.useCMWorker)


    self._clearPointsButton = QtGui.QPushButton("ClearPoints")
    self._clearPointsButton.setToolTip("Clear all of the drawn points from the views")
    self._clearPointsButton.clicked.connect(self.clearPointsWorker)

    self._makePathButton = QtGui.QPushButton("Eval. Points")
    self._makePathButton.setToolTip("Compute the ADCs along the path defined by the points")
    self._makePathButton.clicked.connect(self.drawIonizationWorker)


    # Pack the stuff into a layout

    self._drawingControlBox = QtGui.QVBoxLayout()
    self._drawingControlBox.addWidget(self._restoreDefaults)
    self._drawingControlBox.addWidget(self._maxRangeButton)
    self._drawingControlBox.addWidget(self._clearPointsButton)
    self._drawingControlBox.addWidget(self._makePathButton)
    self._drawingControlBox.addWidget(self._autoRangeBox)
    self._drawingControlBox.addWidget(self._lockAspectRatio)
    self._drawingControlBox.addWidget(self._drawWireOption)
    self._drawingControlBox.addWidget(self._unitDisplayOption)

    return self._drawingControlBox

  def autoRangeWorker(self):
    if self._autoRangeBox.isChecked():
      self._view_manager.autoRange(self._event_manager)
    else:
      self._view_manager.setRangeToMax()

  def viewSelectWorker(self):

    i = 0
    for i in xrange(self._geometry.nViews()):
      if self.sender() == self._viewButtonArray[i]:
        self._view_manager.selectPlane(i)
        self._view_manager.refreshDrawListWidget()
        return
      else:
        i += 1

    # if there wasn't a match, then it must be the ALL button:
    if self.sender() != None:
      self._view_manager.selectPlane(-1)
      self._view_manager.refreshDrawListWidget()
      return

  def clearPointsWorker(self):
    self._view_manager.clearPoints()
    pass

  def drawIonizationWorker(self):
    self._view_manager.makePath()
    pass

  def lockARWorker(self):
    if self._lockAspectRatio.isChecked():
      self._view_manager.lockAR(True)
    else:
      self._view_manager.lockAR(False)

  def drawWireWorker(self):
    if self._drawWireOption.isChecked():
      self._view_manager.drawWire(True)
    else:
      self._view_manager.drawWire(False)    

  def useCMWorker(self):
    if self._unitDisplayOption.isChecked():
      self._view_manager.useCM(True)
    else:
      self._view_manager.useCM(False)    

  def restoreDefaultsWorker(self):
    self._view_manager.restoreDefaults()
    self._view_manager.setRangeToMax()
    
  # This function prepares the quit buttons layout and returns it
  def getQuitLayout(self):
    self._quitButton = QtGui.QPushButton("Quit")
    self._quitButton.setToolTip("Close the viewer.")
    self._quitButton.clicked.connect(self.quit)
    return self._quitButton

  # This function combines the control button layouts, range layouts, and quit button
  def getWestLayout(self):

    event_control = self.getEventControlButtons()
    draw_control = self.getDrawingControlButtons()


    # Add the quit button?
    quit_control = self.getQuitLayout()
    
    self._westLayout = QtGui.QVBoxLayout()
    self._westLayout.addLayout(event_control)
    self._westLayout.addStretch(1)
    self._westLayout.addLayout(draw_control)
    self._westLayout.addStretch(1)

    # Add a section to allow users to just view one window instead of two/three
    self._viewButtonGroup = QtGui.QButtonGroup()
    # Draw all planes:
    self._allViewsButton = QtGui.QRadioButton("All")
    self._allViewsButton.clicked.connect(self.viewSelectWorker)
    self._viewButtonGroup.addButton(self._allViewsButton)

    # Put the buttons in a layout
    self._viewChoiceLayout = QtGui.QVBoxLayout()

    # Make a label for this stuff:
    self._viewChoiceLabel = QtGui.QLabel("View Options")
    self._viewChoiceLayout.addWidget(self._viewChoiceLabel)
    self._viewChoiceLayout.addWidget(self._allViewsButton)

    i = 0
    self._viewButtonArray = []
    for plane in xrange(self._geometry.nViews()):
      button = QtGui.QRadioButton("Plane" + str(i))
      i += 1
      self._viewButtonGroup.addButton(button)
      button.clicked.connect(self.viewSelectWorker)
      self._viewButtonArray.append(button)
      self._viewChoiceLayout.addWidget(button)

    self._westLayout.addLayout(self._viewChoiceLayout)

    self._westLayout.addStretch(1)

    self._westLayout.addWidget(quit_control)
    self._westWidget = QtGui.QWidget()
    self._westWidget.setLayout(self._westLayout)
    self._westWidget.setMaximumWidth(150)
    self._westWidget.setMinimumWidth(100)
    return self._westWidget


  def getSouthLayout(self):
    # This layout contains the status bar and the capture screen buttons

    # The screen capture button:
    self._screenCaptureButton = QtGui.QPushButton("Capture Screen")
    self._screenCaptureButton.setToolTip("Capture the entire screen to file")
    self._screenCaptureButton.clicked.connect(self.screenCapture)
    self._southWidget = QtGui.QWidget()
    self._southLayout = QtGui.QHBoxLayout()
    # Add a status bar
    self._statusBar = QtGui.QStatusBar()
    self._statusBar.showMessage("Test message")
    self._southLayout.addWidget(self._statusBar)
    # self._southLayout.addStretch(1)
    self._southLayout.addWidget(self._screenCaptureButton)
    self._southWidget.setLayout(self._southLayout)

    return self._southWidget

  def getEastLayout(self):
    # This function just makes a dummy eastern layout to use.
    label = QtGui.QLabel("Dummy")
    self._eastWidget = QtGui.QWidget()
    self._eastLayout = QtGui.QVBoxLayout()
    self._eastLayout.addWidget(label)
    self._eastLayout.addStretch(1)
    self._eastWidget.setLayout(self._eastLayout)
    self._eastWidget.setMaximumWidth(200)
    self._eastWidget.setMinimumWidth(100)
    return self._eastWidget

  def refreshEastLayout(self):
    east = getEastLayout()
    self._eastLayout.setVisible(False)
    self._eastLayout.setVisible(True)

  def refreshCenterView(self):

    # for child in self.centerWidget.children():
    #   print type(child)
    #   if type(child) == QtGui.QVBoxLayout:
    #     layout = child

    # print layout.children()
    # print layout

    widget = self._view_manager.getDrawListWidget()
    # for child in widget.children():
    #   print child

    # print widget
    # print layout

    # print layout.children()

    # for i in reversed(range(self.centerWidget.layout.count())): 
        # layout.itemAt(i).widget().setParent(None)

    self.centerWidget.setVisible(False)   
    self.centerWidget.setVisible(True)   

  def initUI(self):


    # Get all of the widgets:
    self.eastWidget  = self.getEastLayout()
    self.westWidget  = self.getWestLayout()
    self.southLayout = self.getSouthLayout()

    # Area to hold data:
    nviews = self._geometry.nViews()
    # nviews = self._baseData._nviews
    for i in range(0, nviews):
      # These boxes hold the wire/time views:
      self._view_manager.addEvdDrawer(i)

    self._view_manager.linkViews()

    self._view_manager.connectStatusBar(self._statusBar)

    self.centerWidget = self._view_manager.getDrawListWidget()

    # Put the layout together


    self.master = QtGui.QVBoxLayout()
    self.slave = QtGui.QHBoxLayout()
    self.slave.addWidget(self.westWidget)
    self.slave.addWidget(self.centerWidget)
    self.slave.addWidget(self.eastWidget)
    self.master.addLayout(self.slave)
    self.master.addWidget(self.southLayout)

    self.setLayout(self.master)    

    # ask the view manager to draw the planes:
    self._view_manager.drawPlanes(self._event_manager)

    self.setGeometry(500, 300, 1200, 800)
    self.setWindowTitle('Event Display')    
    self.setFocus()
    self.show()

  def keyPressEvent(self,e):
    if e.key() == QtCore.Qt.Key_N:
      self._event_manager.next()
      return
    if e.key() == QtCore.Qt.Key_P:
      self._event_manager.prev()
      return
    if e.key() == QtCore.Qt.Key_C:
      # print "C was pressed"
      if e.modifiers() and QtCore.Qt.ControlModifier :
        self.quit()
        return

    # if e.key() == QtCore.Qt.Key_C:
  #     self._dataListsAndLabels['Clusters'].setFocus()
    # if e.key() == QtCore.Qt.Key_H:
  #     self._dataListsAndLabels['Hits'].setFocus()

    if e.key() == QtCore.Qt.Key_R:
      self.setRangeToMax()
      return

    super(gui, self).keyPressEvent(e)

  def screenCapture(self):
    print "Screen Capture!"
    dialog = QtGui.QFileDialog()
    r = self._event_manager.run()
    e = self._event_manager.event()
    s = self._event_manager.subrun()
    name = "evd_" + self._geometry.name() + "_R" + str(r)
    name = name + "_S" + str(s)
    name = name + "_E" + str(e) + ".png"
    f = dialog.getSaveFileName(self,"Save File",name,
        "PNG (*.png);;JPG (*.jpg);;All Files (*)")

    print f
    # print filt
    # Print
    pixmapImage = QtGui.QPixmap.grabWidget(self)
    pixmapImage.save(f,"PNG")


