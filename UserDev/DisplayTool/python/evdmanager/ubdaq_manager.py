from PyQt4 import QtCore, QtGui
from event import manager, event
from datatypes import wire

from ROOT import evd


# This section checks to see if daq data types can be read:
try:
    a = evd.DrawUbDaq()
    has_daq_types = True
except Exception, e:
    has_daq_types = False


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


class ubdaq_manager(manager, wire, QtCore.QObject):

    eventChanged = QtCore.pyqtSignal()
    processLockUpdate = QtCore.pyqtSignal(bool)
    runStarted = QtCore.pyqtSignal()

    """docstring for ubdaq_manager"""

    def __init__(self, geom, file=None):
        # super(ubdaq_manager, self).__init__(geom,file)
        QtCore.QObject.__init__(self)
        manager.__init__(self, geom, file)
        wire.__init__(self)

        # override the wire drawing process
        self._type = None
        self._process = None

        self.setInputFile(file)

        # The lariat manager handles watching files and sending signals
        self._watcher = None
        self._stopFlag = None
        self._running = False

        self._event_no = 0

        # the manager also can cycle through the events in a file:
        self._cycling = False
        self._delay = 0.1

        self._masterLock = False
        self._processLock = False
        self._drawingLock = False
        self.processLockUpdate.connect(self.processLock)

        if not has_daq_types:
            print bcolors.WARNING + "NOTIFICATION: uboonedaq types are not configured to run.  \
Unless you are attempting to draw *.ubdaq, disregard this notification." + bcolors.ENDC

    def initProcess(self):

        # Set up the noise filter and initialize
        self._process.SetCorrectData(False)
        self._process.SetSaveData(False)
        self._process.SetStepSizeByPlane(48, 0)
        self._process.SetStepSizeByPlane(48, 1)
        self._process.SetStepSizeByPlane(96, 2)
        self._process.initialize()

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
        self._type = None
        if file == None:
            return
        else:
            file = str(file).rstrip('\n')
            self._file = file
            if file.endswith(".ubdaq"):
                if has_daq_types:
                    self.initProcess()
                else:
                    print bcolors.FAIL + "ERROR: Can not open a daq file because daq types are not loaded" + bcolors.ENDC
                    return
            else:
                return
            self._process.setInput(file)
            self._hasFile = True
            self.goToEvent(0)
            if self._view_manager != None:
                self._view_manager.setRangeToMax()


    def parseFileName(self, fileName):

        if type(fileName) != str:
            self._filePath = None
            return

        if fileName.endswith(".root") or fileName.endswith(".ubdaq"):
            # this is a data file, set it to display:
            self.setInputFile(fileName)

        elif fileName.endswith(".txt"):
            # parse the txt file to get the file name
            # automatically start a run of autoupdates
            self._monitorFile = fileName
            self.startFileRun(fileName)
            self.runStarted.emit()
        else:
            self._filePath = None

    def drawingLock(self, drawLock):
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
            if self._type == "daq":
                self._process.nextEvent()
            else:
                self._process.goToEvent(self._event)
            self._lastProcessed = self._event
            self.processLockUpdate.emit(False)

    def getPlane(self, plane):
        if self._hasFile:
            return super(ubdaq_manager, self).getPlane(plane)

    def hasWireData(self):
        if self._hasFile:
            return True
        else:
            return False

    def reprocessEvent(self):
        print "Calling event process"
        self.processEvent(True)
        self.eventChanged.emit()