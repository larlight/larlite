from database import dataBase
from ROOT import evd
import pyqtgraph as pg


class wire(dataBase):

    """docstring for wire"""

    def __init__(self):
        super(wire, self).__init__()
        self._process = None

    def getPlane(self, plane):
        return self._process.getArrayByPlane(plane)


class recoWire(wire):

    def __init__(self):
        super(recoWire, self).__init__()
        self._process = evd.DrawRaw()
        self._process.initialize()


class rawDigit(wire):

    def __init__(self, geom):
        super(rawDigit, self).__init__()
        self._process = evd.DrawRawDigit()
        for i in xrange(len(geom._pedestals)):
            self._process.setPedestal(geom._pedestals[i], i)
        self._process.initialize()
        self._process.SetStepSizeByPlane(48, 0)
        self._process.SetStepSizeByPlane(48, 1)
        self._process.SetStepSizeByPlane(96, 2)
        self._process.SetCorrectData(True)
        self._process.SetSaveData(False)