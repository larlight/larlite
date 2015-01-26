from ROOT import ertool
from basictool import GeoViewer

class ERViewer (GeoViewer):


    def __init__(self):
        super(ERViewer,self).__init__()
        self._holder = ertool.GeoViewerERToolBackend()
        
    def add(self,arg1,arg2,arg3=False):
        self._holder.Add(arg1,arg2,arg3)

    def setColorsRandom(self, arg1):
        self._randomColors = arg1
