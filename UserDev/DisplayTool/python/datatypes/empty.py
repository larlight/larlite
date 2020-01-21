from .database import recoBase
from ROOT import evd, TVector3
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore
import math as mt

'''
How to add a class to the viewer!

You need a c++ class.  Checkout RecoViewer/template/empty.h and .cxx.
These classes can be copied and modified to make your own class.

If you want to draw a few things together, like tracks/showers and vertexes,
definitely look at the c++ class for drawing the individual object.  There
is frequently a function that will convert the larlite data product into the 
evd data product that you need, and you can call that function from your
own c++ code.  This will simplify your life.

In python, you need a class like the one below to manage interface with the c++
framework and the Qt based gui.  This class owns the instance of the c++ process
but that process is called from elsewhere.  The function drawObjects is called
with each new drawing iteration and it fetches the data from the process and
manipulates it into a reasonable screen rendering.

ALL UNITS IN THE VIEW ARE WIRE/TIMETICK.  This is because fractional time ticks or 
wires are challenging as pixels.  You can draw objects in fractional places, however.
There is nothing wrong with a vertex projected to a location of 45.3 wires and 1203.7 ticks.

When you have made your c++ class, and it compiles, you should add it to the linkdef
file too.  Then make your python class.  Add your python class to the __init__.py module definition.

Lastly, add your class to the drawableItems file with a line like this:
        self._drawableClasses.update({'MyProduct': [empty.empty,"myprod"]})

The first argument, in quotes, is the name that will appear in the viewer.  The list
is a reference to the class you made (below), so it's file.class and an instance will
be created when a user requests this class be drawn.  The final object is the larlite
data product that gets drawn.

In general, don't push your changes to the event display without talking to Corey Adams first.


'''



from .database import recoBase3D

class empty(recoBase):

    """docstring for empty"""

    def __init__(self):
        super(empty, self).__init__()
        self._productName = 'empty'
        self._process = evd.DrawEmpty()
        self.init()

    def drawObjects(self, view_manager):

        geom = view_manager._geometry
     
        # Loop over the planes, which each has it's own view
        for view in view_manager.getViewPorts():

            # Get the index of this plane:
            thisPlane = view.plane()

            # Initialize some space to keep track of what gets drawn:
            self._drawnObjects.append([])

            # Get the data for this plane from the c++ class
            _this_data = self._process.getDataByPlane(thisPlane)


            radBigW = 0.2 / view_manager._geometry.wire2cm()
            radBigT = (0.2) / view_manager._geometry.time2cm()
            offset = view_manager._geometry.offset(
                thisPlane) / view_manager._geometry.time2cm()

            # Loop over the data in this plane:
            for i in range(len(_this_data)):
                thisPoint = _this_data[i]

                # Need to scale back into wire time coordinates:

                # Do some drawing.  This is from spacepoints in 2D
                sW = thisPoint.w / view_manager._geometry.wire2cm()
                sT = thisPoint.t / view_manager._geometry.time2cm() + offset
                # Every 2D object inherits from QGraphicsItem, at some point
                # in it's ancestry.  This is an object that can be handled by
                # the Qt framework for drawing well.
                r = QtGui.QGraphicsEllipseItem(
                    sW -radBigW, sT-radBigT, 2*radBigW, 2*radBigT)

                r.setPen(pg.mkPen(255,0,255))
                # r.setBrush(pg.mkColor(255,0,255))
                # r.setBrush((0,0,0,opacity))

                # Make sure to save information about what is drawn, or 
                # updating events won't erase previous drawn objects
                self._drawnObjects[thisPlane].append(r)

                # Tell the view to draw this object:
                view._view.addItem(r)


##########################################################
# The following functions are reproduced here from database.py
# in case you need them.  You almost certainly don't, but a couple
# classes (cluster.py, for example) override these functions
##########################################################

    ### This function removes things from the view.
    ### Don't overwrite this unless you're doing complicated things
    
    # def clearDrawnObjects(self, view_manager):
    #     for view in view_manager.getViewPorts():
    #         thisPlane = view.plane()
    #         if len(self._drawnObjects) > thisPlane:
    #             for item in self._drawnObjects[thisPlane]:
    #                 view._view.removeItem(item)

    #     # clear the list:
    #     self._drawnObjects = []

    ### This function allows you to set a custom autorange
    ### You can, and probably should, just do this in the c++
    ### process.  But it's here if you need it.

    # def getAutoRange(self, plane):
    #     if self._process != None:
    #         w = self._process.getWireRange(plane)
    #         t = self._process.getTimeRange(plane)
    #         return [w.first, w.second], [t.first, t.second]
    #     return None, None

##########################################################
# End of functions you probably don't need
##########################################################


#Mandatory: every 3d drawing class must be wrapped
#in this try/except block.  This allows users to have the 2d
#viewer without needing the 3d viewer dependencies.
try:
    import pyqtgraph.opengl as gl
    import numpy as np

    class empty3D(recoBase3D):

        """docstring for empty3D"""

        def __init__(self):
            super(empty3D, self).__init__()
            self._productName = 'empty3D'
            self._process = evd.Drawempty3D()
            self.init()

        def drawObjects(self, view_manager):

            geom = view_manager._geometry
            view = view_manager.getView()

            collection = self._process.getData()
            i_color = 0

            for i in range(len(collection)):
                # Do something here to add the item to the plot
                
                # It has to be some sort of GLItem or inherited class.
                item = gl.GLItem() # GLItem is not real.

                # This line adds the item to the view, which is handed to you from the manager
                view.addItem(item)

                # Keep track of what is drawn, internally, to make it possible
                # to remove everything of one type (say, emptys) without
                # removing others
                self._drawnObjects.append(item)


except Exception as e:
    pass
