from .gui import gui, view_manager
from pyqtgraph.Qt import QtGui, QtCore

# override the gui to give the larsoft display any special features:
class larsoftgui(gui):

    """special live gui"""
    eventUpdate = QtCore.pyqtSignal(bool)
    fileUpdate = QtCore.pyqtSignal(str)
    toggleNoiseFilter = QtCore.pyqtSignal(bool)

    def __init__(self, geometry,manager = None):
        super(larsoftgui, self).__init__(geometry)
        if manager is None:
            manager = larsoft_manager(geometry)
        super(larsoftgui, self).initManager(manager)
        self._event_manager.eventChanged.connect(self.update)
        self.toggleNoiseFilter.connect(self._event_manager.setNoiseFilter)

    # override the initUI function to change things:
    def initUI(self):
        super(larsoftgui, self).initUI()


    # override the update function for larsoft:
    def update(self):

        # set the text boxes correctly:
        eventLabel = "Ev: " + str(self._event_manager.event_no())
        self._eventLabel.setText(eventLabel)
        runLabel = "Run: " + str(self._event_manager.run())
        self._runLabel.setText(runLabel)
        subrunLabel = "SubRun: " + str(self._event_manager.subrun())
        self._subrunLabel.setText(subrunLabel)
        self._view_manager.drawPlanes(self._event_manager)
        self._larliteEventEntry.setText(str(self._event_manager.event()))

    def quit(self):
        try:
            self._event_manager.quit()
        except:
            pass

        QtCore.QCoreApplication.instance().quit()

    # This function sets up the eastern widget
    def getEastLayout(self):
        # This function just makes a dummy eastern layout to use.
        label1 = QtGui.QLabel("Larsoft Viewer")
        label2 = QtGui.QLabel("Raw Digit Only")
        font = label1.font()
        font.setBold(True)
        label1.setFont(font)
        label2.setFont(font)

        self._noiseFilterCheck = QtGui.QCheckBox("Run Noise Filter")
        self._noiseFilterCheck.setToolTip(
            "Run the noise filter on data.  Runs slower and updates on next event.")
        self._noiseFilterCheck.setTristate(False)
        # self._noiseFilterCheck.setChecked(True)
        self.noiseFilterWorker()
        self._noiseFilterCheck.stateChanged.connect(self.noiseFilterWorker)

        self._reprocessButton = QtGui.QPushButton("Redraw")
        self._reprocessButton.clicked.connect(self._event_manager.reprocessEvent)
        self._reprocessButton.setToolTip("Force this event to redraw.")


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

        # add it to the widget:
        self._eastLayout.addStretch(1)

        # Add the controls for file update:
        self._eastLayout.addStretch(1)

        self._eastWidget.setLayout(self._eastLayout)
        self._eastWidget.setMaximumWidth(150)
        self._eastWidget.setMinimumWidth(100)
        return self._eastWidget


    def noiseFilterWorker(self):
        if self._noiseFilterCheck.isChecked():
            self.toggleNoiseFilter.emit(True)
        else:
            self.toggleNoiseFilter.emit(False)
