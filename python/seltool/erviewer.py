from ROOT import ertool
from basictool import GeoViewer

class ERViewer (GeoViewer):


    def __init__(self,name):
        super(ERViewer,self).__init__()
        self._holder = ertool.GeoViewerERToolBackend()
        self.set_window_title(name)
        
    def add(self,arg1,arg2,arg3=False):
        self._holder.Add(arg1,arg2,arg3)

    def setColorsRandom(self, arg1):
        self._randomColors = arg1
