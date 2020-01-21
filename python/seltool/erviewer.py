from ROOT import ertool
from basictool import GeoViewer,fcllite
import os

class ERViewer (GeoViewer):

    def __init__(self,name,width=10,height=5):
        super(ERViewer,self).__init__(width,height)
        self._holder = ertool.GeoViewerERToolBackend()
        self._cfg_mgr = fcllite.ConfigManager()
        self.set_window_title(name)
        self.setConfigFile(ertool.GetDefaultConfigFileName())

    def setConfigFile(self, fname):

        if not os.path.isfile(fname):
            print('Error! not a file:',fname)
            raise Exception
        self._cfg_mgr.Reset()
        self._cfg_mgr.AddCfgFile(fname);
        self._holder.AcceptPSet(self._cfg_mgr.Config())
        
    def add(self,arg1,arg2,arg3=False):
        self._holder.Add(arg1,arg2,arg3)

    def setColorsRandom(self, arg1):
        self._randomColors = arg1
