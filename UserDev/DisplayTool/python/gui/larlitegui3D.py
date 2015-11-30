try:
    import pyqtgraph.opengl as gl
except:
    print("ERROR: Must have opengl for this display.")

from gui3D import gui3D
from pyqtgraph.Qt import QtGui, QtCore
from evdmanager import larlite_manager_3D


class ComboBoxWithKeyConnect(QtGui.QComboBox):

    def __init__(self):
        super(ComboBoxWithKeyConnect, self).__init__()

    def connectOwnerKPE(self, kpe):
        self._owner_KPE = kpe

    def keyPressEvent(self, e):
        if e.key() == QtCore.Qt.Key_Up:
            super(ComboBoxWithKeyConnect, self).keyPressEvent(e)
            return
        if e.key() == QtCore.Qt.Key_Down:
            super(ComboBoxWithKeyConnect, self).keyPressEvent(e)
            return
        else:
            self._owner_KPE(e)
        # if e.key() == QtCore.Qt.Key_N:
        #     self._owner_KPE(e)
        #     pass
        # if e.key() == QtCore.Qt.Key_P:
        #     self._owner_KPE(e)
        #     pass
        # else:
        #     super(ComboBoxWithKeyConnect, self).keyPressEvent(e)
        #     self._owner_KPE(e)

# This is a widget class that contains the label and combo box
# It also knows what to do when updating


class recoBox(QtGui.QWidget):
    activated = QtCore.pyqtSignal(str)

    """docstring for recoBox"""

    def __init__(self, owner, name, product, producers):
        super(recoBox, self).__init__()
        self._label = QtGui.QLabel()
        self._name = name
        self._label.setText(self._name.capitalize() + ": ")
        self._box = ComboBoxWithKeyConnect()
        self._box.activated[str].connect(self.emitSignal)
        self._product = product
        if producers == None:
            self._box.addItem("--None--")
        else:
            self._box.addItem("--Select--")
            for producer in producers:
                self._box.addItem(producer)

        self._box.connectOwnerKPE(owner.keyPressEvent)

        # This is the widget itself, so set it up
        self._layout = QtGui.QVBoxLayout()
        self._layout.addWidget(self._label)
        self._layout.addWidget(self._box)
        self.setLayout(self._layout)

    def keyPressEvent(self, e):
        self._box.keyPressEvent(e)
        super(recoBox, self).keyPressEvent(e)

    def emitSignal(self, text):
        self.activated.emit(text)

    def product(self):
        return self._product

    def name(self):
        return self._name

# Inherit the basic gui to extend it
# override the gui to give the lariat display special features:


class larlitegui3D(gui3D):

    """special larlite gui for 3D"""

    def __init__(self, geometry, manager=None):
        super(larlitegui3D, self).__init__(geometry)
        if manager is None:
            manager = larlite_manager_3D(geometry)
        super(larlitegui3D, self).initManager(manager)
        self.initUI()
        self._event_manager.fileChanged.connect(self.drawableProductsChanged)
        self._event_manager.eventChanged.connect(self.update)

    # override the initUI function to change things:
    def initUI(self):
        super(larlitegui3D, self).initUI()
        # Change the name of the labels for lariat:
        self.update()

    # This function sets up the eastern widget
    def getEastLayout(self):
        # This function just makes a dummy eastern layout to use.
        label1 = QtGui.QLabel("Larlite EVD 3D")
        geoName = self._geometry.name()
        label2 = QtGui.QLabel(geoName.capitalize())
        font = label1.font()
        font.setBold(True)
        label1.setFont(font)
        label2.setFont(font)

        self._eastWidget = QtGui.QWidget()
        # This is the total layout
        self._eastLayout = QtGui.QVBoxLayout()
        # add the information sections:
        self._eastLayout.addWidget(label1)
        self._eastLayout.addWidget(label2)
        self._eastLayout.addStretch(1)


        # Now we get the list of items that are drawable:
        drawableProducts = self._event_manager.getDrawableProducts()
        self._listOfRecoBoxes = []
        for product in drawableProducts:
            thisBox = recoBox(self,
                              product,
                              drawableProducts[product][1],
                              self._event_manager.getProducers(
                                  drawableProducts[product][1]))
            self._listOfRecoBoxes.append(thisBox)
            thisBox.activated[str].connect(self.recoBoxHandler)
            self._eastLayout.addWidget(thisBox)
        self._eastLayout.addStretch(2)

        self._eastWidget.setLayout(self._eastLayout)
        self._eastWidget.setMaximumWidth(150)
        self._eastWidget.setMinimumWidth(100)
        return self._eastWidget

    def drawableProductsChanged(self):
        # self.removeItem(self._eastLayout)
        self._eastWidget.close()
        east = self.getEastLayout()
        self.slave.addWidget(east)
        self.update()

        # self._eastLayout.setVisible(False)
        # self._eastLayout.setVisible(True)


    def recoBoxHandler(self, text):
        sender = self.sender()
        # print sender.product(), "was changed to" , text
        if text == "--Select--" or text == "--None--":
            self._event_manager.redrawProduct(sender.name(),
                                              sender.product(),
                                              None,
                                              self._view_manager)
            return
        self._event_manager.redrawProduct(sender.name(),
                                          sender.product(),
                                          text,
                                          self._view_manager)
