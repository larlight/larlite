from gui import gui, view_manager
from pyqtgraph.Qt import QtGui, QtCore
try:
    from evdmanager.ubdaq_fileglobber import *
    from evdmanager import ubdaq_manager
except:
    pass
    
# override the gui to give the live display special features:
class livegui(gui):

    """special live gui"""
    eventUpdate = QtCore.pyqtSignal(bool)
    fileUpdate = QtCore.pyqtSignal(str)
    toggleNoiseFilter = QtCore.pyqtSignal(bool)

    def __init__(self, geometry, manager = None):
        super(livegui, self).__init__(geometry)
        
        self._timer = QtCore.QTimer()
        self._timer.timeout.connect(self.eventTimeoutTest)
        self._minEventUpdateTime = 30.0
        self._minFileUpdateTime = 3
        self._fileChecker = UBDaqFileGlobber()

        # Make sure a manager is set up:
        if manager is None:
            manager = ubdaq_manager(geometry)
        self.initManager(manager)




    def eventTimeoutTest(self):
        self._event_manager.next()

    # override the initUI function to change things:
    def initUI(self):
        super(livegui, self).initUI()
        # self._viewButtonArray[-1].setDown(True)
        self.update()
        # self._view_manager.setRangeToMax()

    def initManager(self, manager):
        super(livegui, self).initManager(manager)
        self._event_manager.eventChanged.connect(self.update)
        self._event_manager.runStarted.connect(self.runStartWorker)
        self._fileChecker.new_file_ready.connect(self._event_manager.setInputFile)
        self.toggleNoiseFilter.connect(self._event_manager.toggleNoiseFilter)


    # override the update function for larsoft:
    def update(self):
        # Set a lock to
        # set the text boxes correctly:
        eventLabel = "Ev: " + str(self._event_manager.event_no())
        self._eventLabel.setText(eventLabel)
        runLabel = "Run: " + str(self._event_manager.run())
        self._runLabel.setText(runLabel)
        subrunLabel = "SubRun: " + str(self._event_manager.subrun())
        self._subrunLabel.setText(subrunLabel)
        self._view_manager.drawPlanes(self._event_manager)
        self._larliteEventEntry.setText(str(self._event_manager.event()))
        # Also update the larsoft text boxes, just in case:
        if self._fileChecker.isActive():
            self._fileUpdatePauseButton.setText("PAUSE")
            self._fileUpdateLabel.setText("File update ON")
        else:
            self._fileUpdatePauseButton.setText("START")
            self._fileUpdateLabel.setText("File update OFF")
        if self._timer.isActive():
            self._eventUpdatePauseButton.setText("PAUSE")
            self._autoRunLabel.setText("Event update ON")
        else:
            self._eventUpdatePauseButton.setText("START")
            self._autoRunLabel.setText("Event update OFF")

    def quit(self):
        QtCore.QCoreApplication.instance().quit()

    # This function sets up the eastern widget
    def getEastLayout(self):
        # This function just makes an eastern layout to use.
        label1 = QtGui.QLabel("UBDAQ Viewer")
        label2 = QtGui.QLabel("Online Monitor")
        font = label1.font()
        font.setBold(True)
        label1.setFont(font)
        label2.setFont(font)

        self._noiseFilterCheck = QtGui.QCheckBox("Run Noise Filter")
        self._noiseFilterCheck.setToolTip(
            "Run the noise filter on data.  Runs slower and updates on next event.")
        self._noiseFilterCheck.setTristate(False)
        self._noiseFilterCheck.stateChanged.connect(self.noiseFilterWorker)

        self._reprocessButton = QtGui.QPushButton("Redraw")
        self._reprocessButton.clicked.connect(self._event_manager.reprocessEvent)
        self._reprocessButton.setToolTip("Force this event to redraw.")

        # This label tells the user that the event switching is on
        self._autoRunLabel = QtGui.QLabel("Event Update OFF")
        # This label is showing the delay between event updates
        self._eventUpdateDelayLabel = QtGui.QLabel("Delay (s):")
        self._eventUpdateDelayEntry = QtGui.QLineEdit("45")
        self._eventUpdateDelayEntry.returnPressed.connect(self.eventUpdateEntryHandler)
        self._eventUpdateDelayEntry.setMaximumWidth(45)
        self._eventUpdatePauseButton = QtGui.QPushButton("START")
        self._eventUpdatePauseButton.clicked.connect(
            self.eventUpdateButtonHandler)

        self._fileUpdateLabel = QtGui.QLabel("File Update OFF")
        self._fileUpdateDelayLabel = QtGui.QLabel("Delay (min):")
        self._fileUpdateDelayEntry = QtGui.QLineEdit("5")
        self._fileUpdateDelayEntry.returnPressed.connect(self.fileUpdateEntryHandler)
        self._fileUpdateDelayEntry.setMaximumWidth(45)
        self._fileUpdatePauseButton = QtGui.QPushButton("START")
        self._fileUpdatePauseButton.clicked.connect(
            self.fileUpdateButtonHandler)

        self._eastWidget = QtGui.QWidget()
        # This is the total layout
        self._eastLayout = QtGui.QVBoxLayout()
        # add the information sections:
        self._eastLayout.addWidget(label1)
        self._eastLayout.addWidget(label2)
        self._eastLayout.addStretch(1)
        self._eastLayout.addWidget(self._noiseFilterCheck)
        self._eastLayout.addWidget(self._reprocessButton)
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

        # Add the controls for file update:
        self._eastLayout.addWidget(self._fileUpdateLabel)
        fileDelayLayout = QtGui.QHBoxLayout()
        fileDelayLayout.addWidget(self._fileUpdateDelayLabel)
        fileDelayLayout.addWidget(self._fileUpdateDelayEntry)
        self._eastLayout.addLayout(fileDelayLayout)       
        self._eastLayout.addWidget(self._fileUpdatePauseButton)
        self._eastLayout.addStretch(1)

        self._eastWidget.setLayout(self._eastLayout)
        self._eastWidget.setMaximumWidth(150)
        self._eastWidget.setMinimumWidth(100)
        return self._eastWidget

    def fileUpdateEntryHandler(self):
        try:
            delay = float(self._fileUpdateDelayEntry.text())
        except Exception, e:
            delay = self._minFileUpdateTime
            self._fileUpdateDelayEntry.setText(str(delay))
            return
        if delay < self._minFileUpdateTime:
            delay = self._minFileUpdateTime
            self._fileUpdateDelayEntry.setText(str(delay))
            return


    def eventUpdateEntryHandler(self):
        try:
            delay = float(self._eventUpdateDelayEntry.text())
        except Exception, e:
            delay = self._minEventUpdateTime
            self._eventUpdateDelayEntry.setText(str(delay))
            return
        if delay < self._minEventUpdateTime:
            delay = self._minEventUpdateTime
            self._eventUpdateDelayEntry.setText(str(delay))
            return

    def noiseFilterWorker(self):
        if self._noiseFilterCheck.isChecked():
            self.toggleNoiseFilter.emit(True)
        else:
            self.toggleNoiseFilter.emit(False)

    def fileUpdateButtonHandler(self):
        if self._fileChecker.isActive():
            self._fileChecker.stop()
            self._fileUpdatePauseButton.setText("START")
            self._fileUpdateLabel.setText("File update OFF")
        else:
            try:
                delay = float(self._fileUpdateDelayEntry.text())
            except Exception, e:
                delay = self._minFileUpdateTime
            if delay < self._minFileUpdateTime:
                delay = self._minFileUpdateTime
            self._fileUpdateDelayEntry.setText(str(delay))
            self._fileChecker.setPeriod(delay*60*1000)
            self._fileChecker.start()
            self._fileUpdatePauseButton.setText("PAUSE")
            self._fileUpdateLabel.setText("File update ON")

    def eventUpdateButtonHandler(self):
        if self._timer.isActive():
            self._timer.stop()
            self._eventUpdatePauseButton.setText("START")
            self._autoRunLabel.setText("Event update OFF")
        else:
            try:
                delay = float(self._eventUpdateDelayEntry.text())
            except Exception, e:
                delay = self._minEventUpdateTime
            if delay < self._minEventUpdateTime:
                delay = self._minEventUpdateTime
            self._eventUpdateDelayEntry.setText(str(delay))
            self._eventUpdatePauseButton.setText("PAUSE")
            self._timer.setInterval(delay*1000)
            self._timer.start()
            self._autoRunLabel.setText("Event update ON")
            self.eventUpdate.emit(True)

    def runStartWorker(self):
        self.fileUpdateButtonHandler()
        # self.eventUpdateButtonHandler()
