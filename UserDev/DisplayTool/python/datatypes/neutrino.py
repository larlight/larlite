from database import recoBase
from ROOT import evd
from pyqtgraph.Qt import QtGui, QtCore
from connectedObjects import boxCollection
import pyqtgraph as pg
from track import polyLine

class neutrino(recoBase):

    """docstring for vertex"""

    def __init__(self):
        super(neutrino, self).__init__()
        self._listOfClusters = []
        self._productName = 'neutrino'
        self._process = evd.DrawNeutrino()
        self._brush = (0, 0, 0)
        self.init()

    # this is the function that actually draws the hits.
    def drawObjects(self, view_manager):
        geom = view_manager._geometry

        for view in view_manager.getViewPorts():
            
            # extend the list of clusters
            self._listOfClusters.append([])
            
            #   # get the showers from the process:
            self._drawnObjects.append([])
            nu_v = self._process.getDataByPlane(view.plane())

            print 'looking at neutrinos!'

            for nu in nu_v:

                offset = geom.offset(view.plane()) / geom.time2cm()

                #print 'found %i extra tracks'%(nu._trk_v.size())
                for trk in nu._trk_v:

                    points = []
                    # Remeber - everything is in cm, but the display is in
                    # wire/time!
                    for pair in trk.track():
                        x = pair.first / geom.wire2cm()
                        y = pair.second / geom.time2cm() + offset
                        points.append(QtCore.QPointF(x, y))
                    
                    thisPoly = polyLine(points)
                    pen = pg.mkPen((255, 127, 36), width=4)
                    thisPoly.setPen(pen)
                    
                    view._view.addItem(thisPoly)
                    
                    self._drawnObjects[view.plane()].append(thisPoly)


                # draw track
                muon = nu._muon
                # construct a polygon for this track:
                points = []
                # Remeber - everything is in cm, but the display is in
                # wire/time!
                for pair in muon.track():
                    x = pair.first / geom.wire2cm()
                    y = pair.second / geom.time2cm() + offset
                    points.append(QtCore.QPointF(x, y))
                    
                # self._drawnObjects[view.plane()].append(thisPoly)

                thisPoly = polyLine(points)
                pen = pg.mkPen((255,0,0), width=4)
                thisPoly.setPen(pen)
                # polyLine.draw(view._view)
                
                view._view.addItem(thisPoly)
                
                self._drawnObjects[view.plane()].append(thisPoly)

                # add vertex
                vertex = nu._vtx
                # Draws a circle at (x,y,radius = 0.5cm)
                radBigW = 0.5 / view_manager._geometry.wire2cm()
                radBigT = (0.5) / view_manager._geometry.time2cm()

                offset = view_manager._geometry.offset(
                    view.plane()) / view_manager._geometry.time2cm()

                sW = vertex.w / view_manager._geometry.wire2cm()
                sT = vertex.t / view_manager._geometry.time2cm() + offset

                r = QtGui.QGraphicsEllipseItem(
                    sW-radBigW, sT-radBigT, 2*radBigW, 2*radBigT)

                r.setPen(pg.mkPen(None))
                r.setBrush(pg.mkColor(255,0,0))
                self._drawnObjects[view.plane()].append(r)
                view._view.addItem(r)
                
                # draw clusters for shower-like pfparticls
                clusters = nu._clus_v
                print 'there are %i clusters to be drawn'%(clusters.size())
                for i in xrange(len(clusters)):
                    cluster = clusters[i]
                    # Now make the cluster
                    cluster_box_coll = boxCollection()
                    cluster_box_coll.setColor( (0,0,255) )
                    cluster_box_coll.setPlane(view.plane())


                    # draw the hits in this cluster:
                    cluster_box_coll.drawHits(view, cluster)

                    # Keep track of the cluster for drawing management
                    self._listOfClusters[view.plane()].append(cluster_box_coll)
                    

    def clearDrawnObjects(self, view_manager):

        # erase the clusters
        i_plane = 0
        for plane in self._listOfClusters:
            view = view_manager.getViewPorts()[i_plane]
            i_plane += 1
            for cluster in plane:
                cluster.clearHits(view)

        # clear the list:
        self._listOfClusters = []


        for view in view_manager.getViewPorts():
            if len(self._drawnObjects) > view.plane():
                for item in self._drawnObjects[view.plane()]:
                    view._view.removeItem(item)

        # clear the list:
        self._drawnObjects = []
                    
