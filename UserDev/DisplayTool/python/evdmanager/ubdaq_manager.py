from pyqtgraph.Qt import QtGui, QtCore
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
        self._filterData = True

        self.setInputFile(file)

        self._event_no = 0


        if not has_daq_types:
            print bcolors.WARNING + "NOTIFICATION: uboonedaq types are not configured to run.  \
Unless you are attempting to draw *.ubdaq, disregard this notification." + bcolors.ENDC

    def initProcess(self):
        if self._process is None:
            self._process = evd.DrawUbDaq()
        # Set up the noise filter and initialize
        self._process.SetCorrectData(self._filterData)
        self._process.SetSaveData(False)
        self._process.SetStepSizeByPlane(48, 0)
        self._process.SetStepSizeByPlane(48, 1)
        self._process.SetStepSizeByPlane(96, 2)
        self._process.initialize()
        self._process.setInput(self._file)

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
        # Only support for sequential access so just verify we're not on last event
        # And then go forward

        # Check that this isn't the last event:
        if self._event < self._process.n_events() - 1:
            self._process.nextEvent()
        else:
            print "On the last event, can't go to next."
        self.setRun(self._process.run())
        self.setSubRun(self._process.subrun())
        self.setEventNo(self._process.event_no())
        self.eventChanged.emit()

    def prev(self):
        if self._event != 0:
            self.goToEvent(self._event - 1)
        else:
            print "On the first event, can't go to previous."

    def goToEvent(self, event):
        # No support for random access so bail:
        return
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
            print file
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
            self.next()
            if self._view_manager != None:
                self._view_manager.setRangeToMax()


    def parseFileName(self, fileName):

        if type(fileName) != str:
            self._filePath = None
            return

        if fileName.endswith(".ubdaq"):
            # this is a data file, set it to display:
            self.setInputFile(fileName)
        else:
            self._filePath = None

    def processEvent(self, force=False):
        if not self._hasFile:
            return
        if self._lastProcessed != self._event or force:
            if self._type == "daq":
                self._process.nextEvent()
            else:
                self._process.goToEvent(self._event)
            self._lastProcessed = self._event


    def getPlane(self, plane):
        if self._hasFile:
            return super(ubdaq_manager, self).getPlane(plane)

    def hasWireData(self):
        if self._hasFile:
            return True
        else:
            return False

    def toggleNoiseFilter(self,doit):
        self._filterData = doit
        if self._process is not None:
            self._process.SetCorrectData(doit)

    def reprocessEvent(self):
        print "Calling event re-process"
        self._process.reprocessEvent()
        self.eventChanged.emit()
