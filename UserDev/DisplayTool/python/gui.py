

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


class view_manager(object):
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
    self._wireDrawer.setMaximumHeight(100)



  def addEvdDrawer(self,plane):
    self._drawerList.append(viewport(self._geometry, plane))
    self._drawerList[-1].connectWireDrawingFunction(self.drawWireOnPlot)
    self._nviews += 1
  


  def restoreDefaults(self):
    for view in self._drawerList:
      view.restoreDefaults()

  def getDrawListWidget(self):

    # loop through the list and add the drawing windows and their scale
    self._widget = QtGui.QWidget()
    self._layout = QtGui.QVBoxLayout()

    for view in self._drawerList:
      self._layout.addWidget(view.getWidget())

    self._widget.setLayout(self._layout)
    return self._widget

  def connectStatusBar(self,statusBar):
    for view in self._drawerList:
      view.connectStatusBar(statusBar)


  def setRangeToMax(self):
    for view in self._drawerList:
      view.setRangeToMax()

  def autoRange(self):
    for view in self._drawerList:
      view.autoRange()

  def lockAR(self, lockRatio):
    for view in self._drawerList:
      view.lockRatio(lockRatio)

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

    # Max sure the widgets are maxed:
    self.setRangeToMax()
 
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


  def closeEvent(self, event):
    self.quit()  

  def quit(self):
    # if self._running:
      # self.stopRun()
    QtCore.QCoreApplication.instance().quit()


  def update(self):
    # set the text boxes correctly:
    eventLabel = "Ev: "
    self._eventLabel.setText(eventLabel)
    self._eventEntry.setText(str(self._event_manager.event()))
    runLabel = "Run: " + str(self._event_manager.run())
    self._runLabel.setText(runLabel)
    subrunLabel = "Subrun: " + str(self._event_manager.subrun())
    self._subrunLabel.setText(subrunLabel)
    
    self._view_manager.drawPlanes(self._event_manager)

  # This function prepares the buttons such as prev, next, etc and returns a layout
  def getEventControlButtons(self):

    # This is a box to allow users to enter an event
    self._eventEntry = QtGui.QLineEdit()
    self._eventEntry.setToolTip("Enter an event to skip to that event")
    self._eventEntry.returnPressed.connect(self.goToEventWorker)
    # These labels display current events
    self._runLabel = QtGui.QLabel("Run: 0")
    self._eventLabel = QtGui.QLabel("Ev.:")
    self._subrunLabel = QtGui.QLabel("Subrun: 0")

    # Jump to the next event
    self._nextButton = QtGui.QPushButton("Next")
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
    self._eventGrid.addWidget(self._eventLabel)
    self._eventGrid.addWidget(self._eventEntry)
    # Another horizontal box for the run/subrun
    self._runSubRunGrid = QtGui.QHBoxLayout()
    self._runSubRunGrid.addWidget(self._runLabel)
    self._runSubRunGrid.addWidget(self._subrunLabel)
    # Pack it all together
    self._eventControlBox.addLayout(self._eventGrid)
    self._eventControlBox.addLayout(self._runSubRunGrid)
    self._eventControlBox.addWidget(self._nextButton)
    self._eventControlBox.addWidget(self._prevButton)
    self._eventControlBox.addWidget(self._fileSelectButton)

    return self._eventControlBox
  

  # this function helps pass the entry of the line edit item to the event control
  def goToEventWorker(self):
    try:
      event = int(self._eventEntry.text())
    except:
      print "Error, must enter an integer"
      self._eventEntry.setText(str(self._event_manager.event()))
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

    # Pack the stuff into a layout

    self._drawingControlBox = QtGui.QVBoxLayout()
    self._drawingControlBox.addWidget(self._restoreDefaults)
    self._drawingControlBox.addWidget(self._maxRangeButton)
    self._drawingControlBox.addWidget(self._autoRangeBox)
    self._drawingControlBox.addWidget(self._lockAspectRatio)
    self._drawingControlBox.addWidget(self._drawWireOption)
    self._drawingControlBox.addWidget(self._unitDisplayOption)

    return self._drawingControlBox

  def autoRangeWorker(self):
    if self._autoRangeBox.isChecked():
      self._view_manager.autoRange()

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

  def initUI(self):


    # Get all of the widgets:
    eastWidget  = self.getEastLayout()
    westWidget  = self.getWestLayout()
    southLayout = self.getSouthLayout()

    # Area to hold data:
    nviews = self._geometry.nViews()
    # nviews = self._baseData._nviews
    for i in range(0, nviews):
      # These boxes hold the wire/time views:
      self._view_manager.addEvdDrawer(i)

    self._view_manager.connectStatusBar(self._statusBar)

    drawListWidget = self._view_manager.getDrawListWidget()


    # Connect the wire drawing box to the planes so that they may
    # update it
    # for i in range(0, nviews):
        # self._drawerList[i].connectWireDrawFunction(self.drawWire)

    # Put the layout together


    self.master = QtGui.QVBoxLayout()
    self.slave = QtGui.QHBoxLayout()
    self.slave.addWidget(westWidget)
    self.slave.addWidget(drawListWidget)
    self.slave.addWidget(eastWidget)
    self.master.addLayout(self.slave)
    self.master.addWidget(southLayout)

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



# def sigintHandler(*args):
#     """Handler for the SIGINT signal."""
#     sys.stderr.write('\r')
#     sys.exit()

# def main():
    
#   parser = argparse.ArgumentParser(description='Python based event display.')
#   geom = parser.add_mutually_exclusive_group()
#   geom.add_argument('-A', '--argoneut',action='store_true',help="Run with the argoneut geometry")
#   geom.add_argument('-U', '--uboone',action='store_true',help="Run with the microboone geometry")
#   geom.add_argument('-L', '--lariat',action='store_true',help="Run with the lariat geometry")
#   parser.add_argument('file',nargs='?',help="Optional input file to use")
#   parser.add_argument('-d',"--daq",action='store_true',help="Run the evd in daq mode.")
#   args = parser.parse_args()

#   app = QtGui.QApplication(sys.argv)
#   geometry = "uboone"
#   if args.argoneut:
#       geometry = "argoneut"
#   elif args.lariat:
#       geometry = "lariat"
#   if args.daq:
#       mode = "daq"
#       print "Running in daq mode"
#   else:
#     mode = ""
  
#   ex = gui(geometry,mode,args.file)

#   signal.signal(signal.SIGINT, sigintHandler)
#   timer = QtCore.QTimer()
#   timer.start(500)  # You may change this if you wish.
#   timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

#   app.exec_()
#   # sys.exit(app.exec_())


# if __name__ == '__main__':
#   main()


