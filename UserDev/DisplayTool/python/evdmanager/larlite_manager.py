from pyqtgraph.Qt import QtCore
from event import manager, event
import datatypes
from ROOT import larlite as fmwk
import os
from ROOT import TFile


class larlite_manager_base(manager, QtCore.QObject):
    fileChanged = QtCore.pyqtSignal()
    eventChanged = QtCore.pyqtSignal()
    clusterParamsChanged = QtCore.pyqtSignal(bool)

    """docstring for lariat_manager"""

    def __init__(self, geom, file=None):
        super(larlite_manager_base, self).__init__(geom, file)
        manager.__init__(self, geom, file)
        QtCore.QObject.__init__(self)
        # For the larlite manager, need both the ana_processor and
        # the storage manager
        self._process = fmwk.ana_processor()
        self._mgr = fmwk.storage_manager()

        self._keyTable = dict()
        self._drawnClasses = dict()

        if file != None:
            self.setInputFiles(file)

        self._n_entries = 0

        # Toggle whether or not to draw wires:
        self._drawWires = False
        self._drawParams = False
        self._wireDrawer = None
        self._truthDrawer = None

        # Lariat has special meanings to event/spill/run
        self._spill = 0

    def pingFile(self, file):
        """
        this function opens the file and 
        determines what is available to draw
        """
        # This function opens the file to see
        # what data products are available

        # Open the file
        f = TFile(file)
        # Use the larlite_id_tree to find out how many entries are in the file:
        self._n_entries += f.larlite_id_tree.GetEntries()
        # prepare a dictionary of data products
        lookUpTable = dict()
        # Loop over the keys (list of trees)
        for key in f.GetListOfKeys():
            # keys are dataproduct_producer_tree
            thisKeyList = key.GetName().split('_')
            # gets three items in thisKeyList, which is a list
            # [dataProduct, producer, 'tree'] (don't care about 'tree')
            # check if the data product is in the dict:
            if thisKeyList[0] in lookUpTable:
                # extend the list:
                lookUpTable[thisKeyList[0]] += (thisKeyList[1], )
            else:
                lookUpTable.update({thisKeyList[0]: (thisKeyList[1],)})

        self._keyTable.update(lookUpTable)

    def setInputFile(self, file):
        f = [file, ]
        self.setInputFiles(f)

    def setInputFiles(self, files):

        # reset the storage manager and process
        self._mgr.reset()
        self._process.reset()

        if files == None:
            return

        for file in files:
            # First, check that the file exists:
            try:
                if not os.path.exists(file):
                    print "ERROR: requested file does not exist."
                    continue
            except Exception, e:
                print e
                return
            # Next, verify it is a root file:
            if not file.endswith(".root"):
                print "ERROR: must supply a root file."
                continue

            # Finally, ping the file to see what is available to draw
            self.pingFile(file)
            if len(self._keyTable) > 0:
                self._hasFile = True
                # add this file to the storage manager here
                self._mgr.add_in_filename(file)
                self._mgr.set_io_mode(fmwk.storage_manager.kREAD)
                # setup the processor in the same way
                self._process.add_input_file(file)
                self._process.set_io_mode(fmwk.storage_manager.kREAD)

        # Open the manager
        self._mgr.open()
        self._lastProcessed = -1
        self.goToEvent(0)
        self.fileChanged.emit()

    # This function will return all producers for the given product
    def getProducers(self, product):
        try:
            return self._keyTable[product]
        except:
            return None

    # This function returns the list of products that can be drawn:
    def getDrawableProducts(self):
        return self._drawableItems.getDict()

    # override the run,event,subrun functions:
    def run(self):
        if not self._mgr.is_open():
            return 0
        return self._mgr.run_id()

    def event(self):
        if not self._mgr.is_open():
            return 0

        return self._mgr.event_id()

    def subrun(self):
        if not self._mgr.is_open():
            return 0

        return self._mgr.subrun_id()

    def internalEvent(self):
        return self._event

    # override the functions from manager as needed here
    def next(self):
        # print "Called next"
        # Check that this isn't the last event:
        if self._event < self._n_entries - 1:
            self.goToEvent(self._event + 1)
        else:
            print "On the last event, can't go to next."

    def prev(self):
        if self._event != 0:
            self.goToEvent(self._event - 1)
        else:
            print "On the first event, can't go to previous."

    def processEvent(self, force=False):
        if len(self._drawnClasses) == 0 and not self._drawWires:
            self._mgr.go_to(self._event)
            return
        if self._lastProcessed != self._event or force:
            self._process.process_event(self._event)
            self._mgr.go_to(self._event)
            self._lastProcessed = self._event

    def goToEvent(self, event, force=False):
        self.setEvent(event)
        self.processEvent()
        # self.clearAll()
        if self._view_manager != None:
            self._view_manager.drawPlanes(self)
        self.drawFresh()
        self.eventChanged.emit()


class larlite_manager(larlite_manager_base):


    truthLabelChanged = QtCore.pyqtSignal(str)
    '''
    Class to handle the 2D specific aspects of larlite viewer
    '''

    def __init__(self, geom, file=None):
        super(larlite_manager, self).__init__(geom, file)
        self._drawableItems = datatypes.drawableItems()
        self._drawTruth = False

    # this function is meant for the first request to draw an object or
    # when the producer changes
    def redrawProduct(self, name, product, producer, view_manager):
        # print "Received request to redraw ", product, " by ",producer
        # First, determine if there is a drawing process for this product:
        if producer is None:
            if name in self._drawnClasses:
                self._drawnClasses[name].clearDrawnObjects(self._view_manager)
                self._drawnClasses.pop(name)
            return
        if name in self._drawnClasses:
            self._drawnClasses[name].setProducer(producer)
            self.processEvent(True)
            self._drawnClasses[name].clearDrawnObjects(self._view_manager)
            self._drawnClasses[name].drawObjects(self._view_manager)
            return

        # Now, draw the new product
        if name in self._drawableItems.getListOfTitles():
            # drawable items contains a reference to the class, so instantiate
            # it
            drawingClass = self._drawableItems.getDict()[name][0]()
            # Special case for clusters, connect it to the signal:
            if name == 'Cluster':
                self.clusterParamsChanged.connect(
                    drawingClass.setParamsDrawing)
                drawingClass.setParamsDrawing(self._drawParams)
            if name == 'Match':
                self.clusterParamsChanged.connect(
                    drawingClass.setParamsDrawing)
                drawingClass.setParamsDrawing(self._drawParams)

            drawingClass.setProducer(producer)
            self._process.add_process(drawingClass._process)
            self._drawnClasses.update({name: drawingClass})
            # Need to process the event
            self.processEvent(True)
            drawingClass.drawObjects(self._view_manager)

    def clearAll(self):
        for recoProduct in self._drawnClasses:
            self._drawnClasses[recoProduct].clearDrawnObjects(
                self._view_manager)
        self.clearTruth()

    def drawFresh(self):
        # # wires are special:
        # if self._drawWires:
        #   self._view_manager.drawPlanes(self)
        self.clearAll()
        # Draw objects in a specific order defined by drawableItems
        order = self._drawableItems.getListOfTitles()
        self.drawTruth()
        for item in order:
            if item in self._drawnClasses:
                self._drawnClasses[item].drawObjects(self._view_manager)

    def getAutoRange(self, plane):
        # This gets the max bounds
        xRangeMax, yRangeMax = super(larlite_manager, self).getAutoRange(plane)
        xRange = xRangeMax
        yRange = yRangeMax
        for process in self._drawnClasses:
            x, y = self._drawnClasses[process].getAutoRange(plane)
            # Check against all four of the parameters:
            if x != None:
                if x[0] > xRange[0]:
                    xRange[0] = x[0]
                if x[1] < xRange[1]:
                    xRange[1] = x[1]
            if y != None:
                if y[0] > yRange[0]:
                    yRange[0] = y[0]
                if y[1] < yRange[1]:
                    yRange[1] = y[1]

        # Pad the ranges by 1 cm to accommodate
        padding = 1
        xRange[0] = min(xRangeMax[0], xRange[0] - padding/self._geom.wire2cm())
        xRange[1] = max(xRangeMax[1], xRange[1] + padding/self._geom.wire2cm())
        yRange[0] = min(yRangeMax[0], yRange[0] - padding/self._geom.time2cm())
        yRange[1] = max(yRangeMax[1], yRange[1] + padding/self._geom.time2cm())
        return xRange, yRange

    # handle all the wire stuff:
    def toggleWires(self, product):
        # Now, either add the drawing process or remove it:

        if product == 'wire':
            if 'wire' not in self._keyTable:
                print "No wire data available to draw"
                self._drawWires = False
                return
            self._drawWires = True
            self._wireDrawer = datatypes.recoWire()
            self._process.add_process(self._wireDrawer._process)
            self.processEvent(True)

        elif product == 'rawdigit':
            if 'rawdigit' not in self._keyTable:
                print "No raw digit data available to draw"
                self._drawWires = False
                return
            self._drawWires = True
            self._wireDrawer = datatypes.rawDigit(self._geom)
            self._process.add_process(self._wireDrawer._process)
            self.processEvent(True)
        else:
            self._wireDrawer = None
            self._drawWires = False

    def toggleTruth(self, truthBool):
        if truthBool == False:
            self.clearTruth()
            self._drawTruth = False
            self._truthDrawer = None
            return
        if 'mctruth' not in self._keyTable:
            print "No truth information to display"
            self._drawTruth = False
            return
        self._drawTruth = True
        self._truthDrawer = datatypes.mctruth()
        self._process.add_process(self._truthDrawer._process)

    def clearTruth(self):
        if self._truthDrawer is not None:
            self._truthDrawer.clearDrawnObjects(self._view_manager)


    def drawTruth(self):
        if self._drawTruth:
            print "Emiting this message: {msg}".format(msg=self._truthDrawer.getLabel())
            self.truthLabelChanged.emit(self._truthDrawer.getLabel())
            self._truthDrawer.drawObjects(self._view_manager)
        else:
            print "Emiting this message: {msg}".format(msg="")
            self.truthLabelChanged.emit("")


    def toggleParams(self, paramsBool):
        self._drawParams = paramsBool
        self.clusterParamsChanged.emit(paramsBool)
        if 'Cluster' in self._drawnClasses or 'Match' in self._drawnClasses:
            self.drawFresh()

    def getPlane(self, plane):
        if self._drawWires:
            return self._wireDrawer.getPlane(plane)

    def hasWireData(self):
        if self._drawWires:
            return True
        else:
            return False

    def drawHitsOnWire(self, plane, wire):
        if not 'Hit' in self._drawnClasses:
            return
        else:
            # Get the hits:
            hits = self._drawnClasses['Hit'].getHitsOnWire(plane,wire)
            self._view_manager.drawHitsOnPlot(hits)

try:
    import pyqtgraph.opengl as gl

    class larlite_manager_3D(larlite_manager_base):

        """This class handles file I/O and drawing for 3D viewer"""

        def __init__(self, geom, file=None):
            super(larlite_manager_3D, self).__init__(geom, file)
            self._drawableItems = datatypes.drawableItems3D()

        def getAutoRange(self):
            pass

        # this function is meant for the first request to draw an object or
        # when the producer changes
        def redrawProduct(self, name, product, producer, view_manager):
            # print "Received request to redraw ", product, " by ",producer
            # First, determine if there is a drawing process for this product:
            if producer is None:
                if name in self._drawnClasses:
                    self._drawnClasses[name].clearDrawnObjects(
                        self._view_manager)
                    self._drawnClasses.pop(name)
                return
            if name in self._drawnClasses:
                self._drawnClasses[name].setProducer(producer)
                self.processEvent(True)
                self._drawnClasses[name].clearDrawnObjects(self._view_manager)
                self._drawnClasses[name].drawObjects(self._view_manager)
                return



            # Now, draw the new product
            if name in self._drawableItems.getListOfTitles():
                # drawable items contains a reference to the class, so
                # instantiate it
                drawingClass = self._drawableItems.getDict()[name][0]()
                # Special case for clusters, connect it to the signal:
                # if name == 'Cluster':
                #     self.clusterParamsChanged.connect(
                #         drawingClass.setParamsDrawing)
                #     drawingClass.setParamsDrawing(self._drawParams)
                # if name == 'Match':
                #     self.clusterParamsChanged.connect(
                #         drawingClass.setParamsDrawing)
                #     drawingClass.setParamsDrawing(self._drawParams)

                drawingClass.setProducer(producer)
                self._process.add_process(drawingClass._process)
                self._drawnClasses.update({name: drawingClass})
                # Need to process the event
                self.processEvent(True)
                drawingClass.drawObjects(self._view_manager)

        def clearAll(self):
            for recoProduct in self._drawnClasses:
                self._drawnClasses[recoProduct].clearDrawnObjects(
                    self._view_manager)

        def drawFresh(self):
            # # wires are special:
            # if self._drawWires:
            #   self._view_manager.drawPlanes(self)
            self.clearAll()
            # Draw objects in a specific order defined by drawableItems
            order = self._drawableItems.getListOfTitles()
            for item in order:
                if item in self._drawnClasses:
                    self._drawnClasses[item].drawObjects(self._view_manager)

        def goToEvent(self, event, force=False):
            self.setEvent(event)
            self.processEvent()
            self.drawFresh()
            self.eventChanged.emit()


except:
    pass
