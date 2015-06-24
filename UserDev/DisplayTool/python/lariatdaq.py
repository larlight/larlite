#!/usr/bin/env python

from gui import gui
from event import manager
from data import wire
import argparse, sys, signal
from PyQt4 import QtGui, QtCore

from ROOT import evd

from geometry import *

class lariat_manager(manager, wire, QtGui.QWidget):
  """docstring for lariat_manager"""
  def __init__(self, geom, file=None):
    super(lariat_manager, self).__init__(geom,file)

    # override the wire drawing process for lariat
    self._process = evd.DrawLariatDaq(self._geom.tRange())
    self._process.initialize()

    self.setInputFile(file)

    # Lariat has special meanings to event/spill/run
    self._spill = 0

    # The lariat manager handles watching files and sending signals
    self._watcher = None
    self._stopFlag = None
    self._running = False

    # the manager also can cycle through the events in a file:
    self._cycling = False
    self._delay = 0.1

  def subrun(self):
    return self._spill

  def spill(self):
    return self._spill

  def setSpill(self,spill):
    self._spill = spill

  def selectFile(self):
    filePath = str(QtGui.QFileDialog.getOpenFileName())
    self.parseFileName(filePath)
    print "Selected file is ", filePath

  # override the functions from manager as needed here
  def next(self):
    # print "Called next"
    # Check that this isn't the last event:
    if self._event < self._process.n_events() - 1:
      self.goToEvent(self._event + 1)
    elif self._cycling:
      self.goToEvent(0)
    else:
      print "On the last event, can't go to next."

  def prev(self):
    if self._event != 0:
      self.goToEvent(self._event - 1)
    elif self._cycling:
      self.goToEvent(self._process.n_events() -1)  
    else:
      print "On the first event, can't go to previous."

  def goToEvent(self,event):
    self.setRun(self._process.run())
    self.setEvent(event)
    self.setSpill(self._process.spill())
    self.processEvent()
    # print "Event ", self.event(), ", spill ", self.spill(), ", run ", self.run()
    self._gui.update()

  def setInputFile(self, file):
    self._file = file
    if file == None:
      return
    else:
      self._process.setInput(file)
      self._hasFile = True
      self.goToEvent(0)
      # self._gui.update()

  def parseFileName(self,fileName):

    if type(fileName) != str:
      self._filePath = None
      return

    if fileName.endswith(".root"):
      # this is a data file, set it to display:
      self.setInputFile(fileName)

    elif fileName.endswith(".txt"):
        # parse the txt file to get the file name
        # automatically start a run of autoupdates
        self._monitorFile = fileName
        self.startSpillRun(fileName)
    else:
        self._filePath = None

  def isRunning(self):
    return self._running

  def isCycling(self):
    return self._cycling

  def startSpillRun(self,fileName=None):
      # this function can be triggered by a button push, which implies it was stopped before;
      # In that case, refresh the thread and start over.
      # It can also be called by the parsefileName function, which implies a file is ready
      self._running = True
      if self._watcher == None and fileName == None:
          print "ERROR: there is no file to watch, can not start a run."
          return
      self._stopFlag = threading.Event()
      self._watcher = fileWatcher(self._stopFlag, self._monitorFile)
      self.connect(self._watcher, QtCore.SIGNAL("fileChanged"),self.setInputFile)
      self._running = True
      self._watcher.start()
      pass

  def stopSpillRun(self):
    self._running = False
    self._stopFlag.set()
    return

  def startCycle(self,delay=None):
    if delay != None:
      self._delay = delay
    # set up a thread to call next event every so often
    self._cycling = True
    self._stopCycleFlag = threading.Event()
    self._cycleWatcher = delayTimer(self._stopCycleFlag,self._delay)
    self.connect(self._cycleWatcher, QtCore.SIGNAL("delayExpired"),self.next)
    self._cycleWatcher.start()

  def stopCycle(self):
    self._cycling = False
    self._stopCycleFlag.set()

  def processEvent(self,force = False):
    if not self._hasFile:
      return
    if self._lastProcessed != self._event or force:
      self._process.goToEvent(self._event)
      self._lastProcessed = self._event

  def getPlane(self,plane):
    if self._hasFile:
      return super(lariat_manager,self).getPlane(plane)


  def hasWireData(self):
    if self._hasFile:
      return True
    else:
      return False    

import threading
class fileWatcher(QtCore.QObject,threading.Thread):
  """This file watcher class monitors a file and calls a function when it's contents change"""

  def __init__(self, event,file):
    QtCore.QObject.__init__(self)
    threading.Thread.__init__(self)
    # super(fileWatcher, self).__init__()
    self._stopped = event
    # keep track of the last file displayed, only send a signal when it changes
    self._prevFile=None
    self._fileToWatch = file
    # self.fileChanged = QtCore.pyqtSignal()

  def run(self):
    while not self._stopped.wait(1.5):
      # print "Thread called"
      # open the file:
      try:
        f = open(self._fileToWatch)
      except Exception, e:
        print "The file selected for watching does not exist!"
        raise e
      # At this point, the file exists.
      fileToDraw = f.readline()
      # print "Contents of file are: ", fileToDraw
      if fileToDraw == self._prevFile:
        # print "Not drawing because file has not changed."
        continue
      if fileToDraw.endswith(".root"):
        print "Attempting to refresh picture"
        self.emit(QtCore.SIGNAL("fileChanged"),fileToDraw)
        # self._signal.fileChanged.emit()
        # self._func(fileToDraw)
        self._prevFile = fileToDraw
      else:
        print "File has changed but does not appear to be a root file."

class delayTimer(QtCore.QObject,threading.Thread):
  """docstring for funcCaller"""
  def __init__(self, event, delay):
    QtCore.QObject.__init__(self)
    threading.Thread.__init__(self)
    self._stopped = event
    self._delay = delay
    
  def run(self):
    # print "Called run"
    while not self._stopped.wait(self._delay):
      self.emit(QtCore.SIGNAL("delayExpired"))



# override the gui to give the lariat display special features:
class lariatgui(gui):
  """special lariat gui"""
  def __init__(self, geometry,manager):
    super(lariatgui, self).__init__(geometry,manager)

    self._watcher = None
    self._stopFlag = None
    self._running = False


  # override the initUI function to change things:
  def initUI(self):
    super(lariatgui,self).initUI()
    # Change the name of the labels for lariat:
    self._subrunLabel.setText("Spill: 0")      


  # override the update function for lariat:
  def update(self):
    # set the text boxes correctly:
    eventLabel = "Ev: " 
    self._eventLabel.setText(eventLabel)
    runLabel = "Run: " + str(self._event_manager.run())
    self._runLabel.setText(runLabel)
    spillLabel = "Spill: "  + str(self._event_manager.spill())
    self._subrunLabel.setText(spillLabel)
    self._view_manager.drawPlanes(self._event_manager)
    self._eventEntry.setText(str(self._event_manager.event()))
    # Also update the lariat text boxes, just in case:
    if self._event_manager.isRunning():
      self._spillUpdatePauseButton.setText("PAUSE")
      self._spillUpdateLabel.setText("Spill update ON")
    else:
      self._spillUpdatePauseButton.setText("START")
      self._spillUpdateLabel.setText("Spill update OFF")    
    if self._event_manager.isCycling():
      self._eventUpdatePauseButton.setText("PAUSE")
      self._autoRunLabel.setText("Event update ON")
    else:
      self._eventUpdatePauseButton.setText("START")
      self._autoRunLabel.setText("Event update OFF")

  def stopRun(self):
    self._runControlButton.setText("Start Run")
    self._running = False
    self._stopFlag.set()
    pass

  def runControl(self):
    if not self._running:
      self.startRun()
      return
    else:
      self.stopRun()

  def updateFile(self, file):
      if self._running:
        self.stopRun()
      self.parseFileName(file)
      self._filePath = file

  # this function is ONLY meant to be called by a thread for auto updating.
  # Do not call this yourself, use updateFile instead
  def autoUpdateFile(self,file):
    # Checking that the file is a .root file is left to the thread
    # print "Called function to update image ", file
    self._filePath = file
    self.initData()
    self.updateDataChoices()
    self.goToEvent(0)
    for view in range(0,self._baseData._nviews):
      self._drawerList[view].setVisible(False)
      self._drawerList[view].setVisible(True)


  def quit(self): 
    if self._event_manager.isRunning():
      self._event_manager.stopSpillRun()
    if self._event_manager.isCycling():
      self._event_manager.stopCycle()
    QtCore.QCoreApplication.instance().quit()


  # This function sets up the eastern widget
  def getEastLayout(self):
    # This function just makes a dummy eastern layout to use.
    label1 = QtGui.QLabel("Lariat DQM")
    label2 = QtGui.QLabel("Online Monitor")
    font = label1.font()
    font.setBold(True)
    label1.setFont(font)
    label2.setFont(font)
    # This label tells the user that the event switching is on
    self._autoRunLabel = QtGui.QLabel("Event Update OFF")
    # This label is showing the delay between event updates
    self._eventUpdateDelayLabel = QtGui.QLabel("Delay (s):")
    self._eventUpdateDelayEntry = QtGui.QLineEdit("1")
    self._eventUpdateDelayEntry.setMaximumWidth(35)
    self._eventUpdatePauseButton = QtGui.QPushButton("START")
    self._eventUpdatePauseButton.clicked.connect(self.eventUpdateButtonHandler)

    self._spillUpdateLabel = QtGui.QLabel("Spill Update ON")
    self._spillUpdatePauseButton = QtGui.QPushButton("START")
    self._spillUpdatePauseButton.clicked.connect(self.spillUpdateButtonHandler)

    self._eastWidget = QtGui.QWidget()
    # This is the total layout
    self._eastLayout = QtGui.QVBoxLayout()
    # add the information sections:
    self._eastLayout.addWidget(label1)
    self._eastLayout.addWidget(label2)
    self._eastLayout.addStretch(1)
    # Add the auto event switch stuff:
    self._eastLayout.addWidget(self._autoRunLabel)
    autoDelayLayout = QtGui.QHBoxLayout()
    autoDelayLayout.addWidget(self._eventUpdateDelayLabel)
    autoDelayLayout.addWidget(self._eventUpdateDelayEntry)
    # add it to the widget:
    self._eastLayout.addLayout(autoDelayLayout)
    self._eastLayout.addWidget(self._eventUpdatePauseButton)
    self._eastLayout.addStretch(1)

    # Add the controls for spill update:
    self._eastLayout.addWidget(self._spillUpdateLabel)
    self._eastLayout.addWidget(self._spillUpdatePauseButton)
    self._eastLayout.addStretch(1)

    self._eastWidget.setLayout(self._eastLayout)
    self._eastWidget.setMaximumWidth(150)
    self._eastWidget.setMinimumWidth(100)
    return self._eastWidget

  def spillUpdateButtonHandler(self):
    if self._event_manager.isRunning():
      self._event_manager.stopSpillRun()
      self._spillUpdatePauseButton.setText("START")
      self._spillUpdateLabel.setText("Spill update OFF")
    else:
      self._event_manager.startSpillRun()
      self._spillUpdatePauseButton.setText("PAUSE")
      self._spillUpdateLabel.setText("Spill update ON")

  def eventUpdateButtonHandler(self):
    if self._event_manager.isCycling():
      self._event_manager.stopCycle()
      self._eventUpdatePauseButton.setText("START")
      self._autoRunLabel.setText("Event update OFF")
    else:
      try:
        delay = float(self._eventUpdateDelayEntry.text())
      except Exception, e:
        delay = 1.0
      if delay < 0.1:
        delay = 0.1
      self._eventUpdatePauseButton.setText("PAUSE")
      self._event_manager.startCycle(delay)
      self._autoRunLabel.setText("Event update ON")

def sigintHandler(*args):
    """Handler for the SIGINT signal."""
    sys.stderr.write('\r')
    sys.exit()


def main():
    
  parser = argparse.ArgumentParser(description='Python based event display.')
  parser.add_argument('file',nargs='?',help="Optional input file to use")
  args = parser.parse_args()

  app = QtGui.QApplication(sys.argv)

  geom = lariat()

  # If a file was passed, give it to the manager:

  manager = lariat_manager(geom)
  manager.parseFileName(args.file)

  thisgui = lariatgui(geom,manager)
  thisgui.initUI()
  manager.goToEvent(0)


  signal.signal(signal.SIGINT, sigintHandler)
  timer = QtCore.QTimer()
  timer.start(500)  # You may change this if you wish.
  timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

  app.exec_()
  # sys.exit(app.exec_())


if __name__ == '__main__':
  main()