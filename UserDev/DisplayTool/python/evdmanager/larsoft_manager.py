from pyqtgraph.Qt import QtGui, QtCore
from .event import manager, event
from datatypes import wire
# from ROOT import larlite as fmwk
from ROOT import evd

class larsoft_manager(manager, wire, QtCore.QObject):

    eventChanged = QtCore.pyqtSignal()
    runStarted = QtCore.pyqtSignal()

    """docstring for larsoft_manager"""

    def __init__(self, geom, file=None):
        # super(larsoft_manager, self).__init__(geom,file)
        QtCore.QObject.__init__(self)
        manager.__init__(self, geom, file)
        wire.__init__(self)

        # override the wire drawing process
        self._type = None
        self._process = evd.DrawUbSwiz()

        # Set up the noise filter and initialize
        self._process.SetCorrectData(False)
        self._process.SetSaveData(False)
        self._process.SetStepSizeByPlane(48, 0)
        self._process.SetStepSizeByPlane(48, 1)
        self._process.SetStepSizeByPlane(96, 2)
        for plane in range(geom.nViews()):
            self._process.setYDimension(geom.readoutWindowSize(), plane)
        self._process.initialize()
        self.setInputFile(file)

        # The lariat manager handles watching files and sending signals
        self._watcher = None
        self._stopFlag = None
        self._running = False

        self._event_no = 0



    def setEventNo(self, event_no):
        self._event_no = event_no

    def event_no(self):
        return self._event_no

    def selectFile(self):
        filePath = str(QtGui.QFileDialog.getOpenFileName())
        self.setInputFile(filePath)
        print("Selected file is ", filePath)

    def setNoiseFilter(self, runFilterBool):
        self._process.SetCorrectData(runFilterBool)

    # override the functions from manager as needed here
    def __next__(self):
        # Check that this isn't the last event:
        if self._event < self._process.n_events() - 1:
            self.goToEvent(self._event + 1)
        else:
            print("On the last event, can't go to next.")

    def prev(self):
        if self._event != 0:
            self.goToEvent(self._event - 1)
        else:
            print("On the first event, can't go to previous.")

    def goToEvent(self, event):
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
            if file.endswith(".root"):
                self._type = "root"
                self._process.setInput(file)
                self._hasFile = True
                self.goToEvent(0)
                # if self._view_manager != None:
                    # self._view_manager.setRangeToMax()
            else:
                return


    def processEvent(self, force=False):
        if not self._hasFile:
            return
        if self._lastProcessed != self._event or force:
            self._process.goToEvent(self._event)
            self._lastProcessed = self._event

    def getPlane(self, plane):
        if self._hasFile:
            return super(larsoft_manager, self).getPlane(plane)

    def hasWireData(self):
        if self._hasFile:
            return True
        else:
            return False

    def setNoiseFilter(self, noiseFilterBool):
        self._process.SetCorrectData(noiseFilterBool)

    def reprocessEvent(self):
        self.processEvent(True)
        self.eventChanged.emit()