# from ROOT import evd


# These classes provide the basic interfaces for drawing objects
# It's meant to be inherited from for specific instances
# There is a class for drawing the following things:
#   - Raw wire data
#   - hits
#   - clusters
#   - nothing else ... yet

# To add a class, what you have to do is write a module to process it (larlite ana_processor)
# Put your class in RecoViewer or wherever.  I use the namespace evd for all of these classes
# so if you use something else, be sure to import it above from ROOT

# Your class needs to include getter methods for the data you need, and a setProducer function
# which sets the producer of the data type.  Then, write a python class that inherits from
# recoBase below.  Your class must override the function drawObjects to display it's data
# on the views.  Your class is responsible for maintaining the items being drawn, and must
# clear them with the clearDrawnObjects function.  Store drawn objects in
# _drawnObjects.

# Lastly, extend the drawableItems class with one line:
# self._drawableClasses.update({'dataProduct':yourPythonClassName})
# This gives the viewer the connection between the class you made and the
# product you want to draw

# Beyond that, inside your larlite or python class you can do pretty much whatever you want
# Run merging, matching, ertool, who cares.



class dataBase(object):

    """basic class from which data objects inherit"""

    def __init__(self):
        super(dataBase, self).__init__()
        self._productName = "null"
        self._producerName = "null"
        self._process = None

    def productName(self):
        return self._productName

    def producerName(self):
        return self._producerName

    def setProducer(self, name):
        self._producerName = name


# Reco base, all reco objects must inherit from this
class recoBase(dataBase):

    """docstring for recoBase"""

    def __init__(self):
        super(recoBase, self).__init__()
        self._drawnObjects = []
        self._process = None

    def init(self):
        self._process.initialize()

    def clearDrawnObjects(self, view_manager):
        for view in view_manager.getViewPorts():
            thisPlane = view.plane()
            if len(self._drawnObjects) > thisPlane:
                for item in self._drawnObjects[thisPlane]:
                    view._view.removeItem(item)

        # clear the list:
        self._drawnObjects = []

    # override set producer
    def setProducer(self, producer):
        self._producerName = producer
        self._process.setProducer(str(producer))

    def getDrawnObjects(self):
        return self._drawnObjects

    def drawObjects(self, view_manager):
        pass

    def getAutoRange(self, plane):
        if self._process != None:
            w = self._process.getWireRange(plane)
            t = self._process.getTimeRange(plane)
            return [w.first, w.second], [t.first, t.second]
        return None, None

class recoBase3D(dataBase):

    """docstring for recoBase3D"""

    def __init__(self):
        super(recoBase3D, self).__init__()
        self._drawnObjects = []
        self._process = None

    def init(self):
        self._process.initialize()

    def clearDrawnObjects(self, view_manager):
        view = view_manager.getView()
        for item in self._drawnObjects:
            view.removeItem(item)
        # clear the list:
        self._drawnObjects = []

    # override set producer
    def setProducer(self, producer):
        self._producerName = producer
        self._process.setProducer(str(producer))

    def getDrawnObjects(self):
        return self._drawnObjects

    def drawObjects(self, view_manager):
        pass

    def getAutoRange(self, plane):
        if self._process != None:
            x = self._process.getXRange
            y = self._process.getYRange
            z = self._process.getZRange
            return [x.first, x.second], [y.first, y.second], [z.first, z.second]
        return None, None, None
