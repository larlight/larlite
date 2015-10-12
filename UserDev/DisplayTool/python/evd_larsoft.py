#!/usr/bin/env python

from gui import gui
from event import manager
from data import wire
import argparse
import sys
import signal
from PyQt4 import QtGui, QtCore
import os

from ROOT import evd

from geometry import *


class evd_manager(manager, wire, QtCore.QObject):

    eventChanged = QtCore.pyqtSignal()
    processLockUpdate = QtCore.pyqtSignal(bool)

    """docstring for evd_manager"""

    def __init__(self, geom, file=None):
        # super(evd_manager, self).__init__(geom,file)
        QtCore.QObject.__init__(self)
        manager.__init__(self, geom, file)
        wire.__init__(self)

        # override the wire drawing process for lariat
        self._process = evd.DrawUbSwiz()
        self._process.SetCorrectData(False)
        self._process.SetSaveData(False)
        self._process.initialize()

        self.setInputFile(file)

        # The lariat manager handles watching files and sending signals
        self._watcher = None
        self._stopFlag = None
        self._running = False

        # the manager also can cycle through the events in a file:
        self._cycling = False
        self._delay = 0.1

        self._masterLock = False
        self._processLock = False
        self._drawingLock = False
        self.processLockUpdate.connect(self.processLock)


    def setEventNo(self, event_no):
        self._event_no = event_no

    def event_no(self):
        return self._event_no

    def selectFile(self):
        filePath = str(QtGui.QFileDialog.getOpenFileName())
        self.parseFileName(filePath)
        print "Selected file is ", filePath

    def setNoiseFilter(self, runFilterBool):
        self._process.SetCorrectData(runFilterBool)

    # override the functions from manager as needed here
    def next(self):
        # Don't attempt to run if the process is locked
        if self.sender() != None and self._masterLock:
          # print "drawing: ", self._drawingLock
          # print "process: ", self._processLock
          # print "master:  ", self._masterLock
          # print self.sender()
          return

        # print "Called next"
        # Check that this isn't the last event:
        if self._event < self._process.n_events() - 1:
            self.goToEvent(self._event + 1)
        elif self._cycling:
            self.goToEvent(0)
        else:
            print "On the last event, can't go to next."

    def prev(self):
        # Don't attempt to run if the process is locked
        if self._masterLock:
          return
        if self._event != 0:
            self.goToEvent(self._event - 1)
        elif self._cycling:
            self.goToEvent(self._process.n_events() - 1)
        else:
            print "On the first event, can't go to previous."

    def goToEvent(self, event):
        # Don't attempt to run if the process is locked
        self.setEvent(event)
        self.processEvent()
        self.setRun(self._process.run())
        self.setSubRun(self._process.subrun())
        self.setEventNo(self._process.event_no())
        self.eventChanged.emit()

    def setInputFile(self, file):
        self._file = file
        if file == None:
            return
        else:
            file = str(file)
            self._process.setInput(file)
            self._hasFile = True
            self.goToEvent(0)

    def parseFileName(self, fileName):

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

    def drawingLock(self,drawLock):
        self._drawingLock = drawLock
        self._masterLock = self._processLock or self._drawingLock


    def processLock(self, procLock):
        self._processLock = procLock
        self._masterLock = self._processLock or self._drawingLock


    def isRunning(self):
        return self._running

    def isCycling(self):
        return self._cycling

    def startFileRun(self, fileName=None):
            # this function can be triggered by a button push, which implies it was stopped before;
            # In that case, refresh the thread and start over.
            # It can also be called by the parsefileName function, which implies a
            # file is ready
        self._running = True
        if self._watcher == None and fileName == None:
            print "ERROR: there is no file to watch, can not start a run."
            return
        self._stopFlag = threading.Event()
        self._watcher = fileWatcher(self._stopFlag, self._monitorFile)
        self._watcher.fileChanged.connect(self.setInputFile)
        self._running = True
        self._watcher.start()
        pass

    def stopFileRun(self):
        self._running = False
        self._stopFlag.set()
        return

    def startCycle(self, delay=None):
        if delay != None:
            self._delay = delay
        # set up a thread to call next event every so often
        self._cycling = True
        self._stopCycleFlag = threading.Event()
        self._cycleWatcher = delayTimer(self._stopCycleFlag, self._delay)
        self._cycleWatcher.delayExpired.connect(self.next)
        self._cycleWatcher.start()

    def stopCycle(self):
        self._cycling = False
        self._stopCycleFlag.set()

    def processEvent(self, force=False):
        if not self._hasFile:
            return
        if self._lastProcessed != self._event or force:
            self.processLockUpdate.emit(True)
            self._process.goToEvent(self._event)
            self._lastProcessed = self._event
            self.processLockUpdate.emit(False)

    def getPlane(self, plane):
        if self._hasFile:
            return super(evd_manager, self).getPlane(plane)

    def hasWireData(self):
        if self._hasFile:
            return True
        else:
            return False

import threading


class fileWatcher(QtCore.QObject, threading.Thread):

    """This file watcher class monitors a file and
    calls a function when it's contents change"""
    fileChanged = QtCore.pyqtSignal(str)

    def __init__(self, event, file):
        QtCore.QObject.__init__(self)
        threading.Thread.__init__(self)
        # super(fileWatcher, self).__init__()
        self._stopped = event
        # keep track of the last file displayed, only send a signal when it
        # changes
        self._prevFile = None
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
            fileName, fileExtension = os.path.splitext(fileToDraw)
            fileExtension = fileExtension.rstrip()
            if fileToDraw == self._prevFile:
                # print "Not drawing because file has not changed."
                continue
            if fileExtension == ".root":
                print "Refreshing file!"
                self.fileChanged.emit(fileToDraw)
                self._prevFile = fileToDraw
            else:
                print "File has changed but does not appear to be a root file."
                self._prevFile = fileToDraw


class delayTimer(QtCore.QObject, threading.Thread):

    """docstring for funcCaller"""
    delayExpired = QtCore.pyqtSignal()

    def __init__(self, event, delay):
        QtCore.QObject.__init__(self)
        threading.Thread.__init__(self)
        self._stopped = event
        self._delay = delay
        self._lock = False

    def run(self):
        # print "Called run"
        while not self._stopped.wait(self._delay):
            self.delayExpired.emit()



# override the gui to give the larsoft display special features:
class larsoftgui(gui):

    """special larsoft gui"""
    drawingLock = QtCore.pyqtSignal(bool)

    def __init__(self, geometry, manager):
        super(larsoftgui, self).__init__(geometry, manager)
        self._watcher = None
        self._stopFlag = None
        self._running = False
        self._event_manager.eventChanged.connect(self.update)
        self.drawingLock.connect(self._event_manager.drawingLock)


    # override the initUI function to change things:
    def initUI(self):
        super(larsoftgui, self).initUI()
        self.update()
        self._view_manager.setRangeToMax()

    # override the update function for larsoft:
    def update(self):
        # Set a lock to 
        self.drawingLock.emit(True)
        # set the text boxes correctly:
        eventLabel = "Ev: " + str(self._event_manager.event_no())
        self._eventLabel.setText(eventLabel)
        runLabel = "Run: " + str(self._event_manager.run())
        self._runLabel.setText(runLabel)
        spillLabel = "SubRun: " + str(self._event_manager.subrun())
        self._subrunLabel.setText(spillLabel)
        self._view_manager.drawPlanes(self._event_manager)
        self._larliteEventEntry.setText(str(self._event_manager.event()))
        # Also update the larsoft text boxes, just in case:
        if self._event_manager.isRunning():
            self._spillUpdatePauseButton.setText("PAUSE")
            self._spillUpdateLabel.setText("File update ON")
        else:
            self._spillUpdatePauseButton.setText("START")
            self._spillUpdateLabel.setText("File update OFF")
        if self._event_manager.isCycling():
            self._eventUpdatePauseButton.setText("PAUSE")
            self._autoRunLabel.setText("Event update ON")
        else:
            self._eventUpdatePauseButton.setText("START")
            self._autoRunLabel.setText("Event update OFF")
        self.drawingLock.emit(False)

    def quit(self):
        if self._event_manager.isRunning():
            self._event_manager.stopSpillRun()
        if self._event_manager.isCycling():
            self._event_manager.stopCycle()
        QtCore.QCoreApplication.instance().quit()

    # This function sets up the eastern widget
    def getEastLayout(self):
        # This function just makes a dummy eastern layout to use.
        label1 = QtGui.QLabel("Larsoft Viewer")
        label2 = QtGui.QLabel("Online Monitor")
        font = label1.font()
        font.setBold(True)
        label1.setFont(font)
        label2.setFont(font)

        self._noiseFilterCheck = QtGui.QCheckBox("Run Noise Filter")
        self._noiseFilterCheck.setToolTip("Run the noise filter on data.  Runs slower and updates on next event.")
        self._noiseFilterCheck.setTristate(False)
        self._noiseFilterCheck.stateChanged.connect(self.noiseFilterWorker)  

        # This label tells the user that the event switching is on
        self._autoRunLabel = QtGui.QLabel("Event Update OFF")
        # This label is showing the delay between event updates
        self._eventUpdateDelayLabel = QtGui.QLabel("Delay (s):")
        self._eventUpdateDelayEntry = QtGui.QLineEdit("1")
        self._eventUpdateDelayEntry.setMaximumWidth(35)
        self._eventUpdatePauseButton = QtGui.QPushButton("START")
        self._eventUpdatePauseButton.clicked.connect(
            self.eventUpdateButtonHandler)

        self._spillUpdateLabel = QtGui.QLabel("File Update ON")
        self._spillUpdatePauseButton = QtGui.QPushButton("START")
        self._spillUpdatePauseButton.clicked.connect(
            self.spillUpdateButtonHandler)

        self._eastWidget = QtGui.QWidget()
        # This is the total layout
        self._eastLayout = QtGui.QVBoxLayout()
        # add the information sections:
        self._eastLayout.addWidget(label1)
        self._eastLayout.addWidget(label2)
        self._eastLayout.addStretch(1)
        self._eastLayout.addWidget(self._noiseFilterCheck)
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

    def noiseFilterWorker(self):
      if self._noiseFilterCheck.isChecked():
        self._event_manager.setNoiseFilter(True)
      else:
        self._event_manager.setNoiseFilter(False)

    def spillUpdateButtonHandler(self):
        if self._event_manager.isRunning():
            self._event_manager.stopSpillRun()
            self._spillUpdatePauseButton.setText("START")
            self._spillUpdateLabel.setText("File update OFF")
        else:
            self._event_manager.startSpillRun()
            self._spillUpdatePauseButton.setText("PAUSE")
            self._spillUpdateLabel.setText("File update ON")

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
    parser.add_argument('file', nargs='?', help="Optional input file to use")
    geom = parser.add_mutually_exclusive_group()
    geom.add_argument('-A', '-a', '--argoneut',
                      action='store_true',
                      help="Run with the argoneut geometry")
    geom.add_argument('-U', '-u', '--uboone',
                      action='store_true',
                      help="Run with the microboone geometry")
    geom.add_argument('-L', '-l', '--lariat',
                      action='store_true',
                      help="Run with the lariat geometry")
    args = parser.parse_args()

    app = QtGui.QApplication(sys.argv)

    if args.argoneut:
        geometry = argoneut()
    elif args.lariat:
        geometry = lariat()
    else:
        geometry = microboone()

    # If a file was passed, give it to the manager:

    manager = evd_manager(geometry)
    manager.parseFileName(args.file)

    thisgui = larsoftgui(geometry, manager)
    thisgui.initUI()
    # manager.goToEvent(0)

    signal.signal(signal.SIGINT, sigintHandler)
    timer = QtCore.QTimer()
    timer.start(500)  # You may change this if you wish.
    timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

    app.exec_()
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
