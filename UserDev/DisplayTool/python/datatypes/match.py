from database import recoBase
from ROOT import evd, protoshower
import pyqtgraph as pg
from connectedObjects import connectedBox, connectedCircle, boxCollection
from cluster import clusterParams

class match(recoBase):

    """Class to handle drawing of matches"""

    def __init__(self):
        super(match, self).__init__()
        self._process = evd.DrawMatch()
        self.init()
        self._alg = protoshower.ProtoShowerAlgClusterParams()
        # self._alg = protoshower.ProtoShowerAlgArgoNeuT()
        # self._alg = protoshower.ProtoShowerAlgOpenCV()
        self._process.set_proto_shower_alg(self._alg)


        self._listOfClusters = []
        self._listOfCParams  = []

        # Defining the cluster colors:
        self._matchColors = [
            (0, 147, 147),  # dark teal
            (0, 0, 252),   # bright blue
            (156, 0, 156),  # purple
            (255, 0, 255),  # pink
            (255, 0, 0),  # red
            (175, 0, 0),  # red/brown
            (252, 127, 0),  # orange
            (102, 51, 0),  # brown
            (127, 127, 127),  # dark gray
            (210, 210, 210),  # gray
            (100, 253, 0)  # bright green
        ]

    def setParamsDrawing(self, paramsBool):
        self._drawParams = paramsBool

    def drawObjects(self, view_manager):

        for view in view_manager.getViewPorts():
            colorIndex = 0
            # get the plane
            thisPlane = view.plane()

            # extend the list of clusters
            self._listOfClusters.append([])

            self._listOfCParams.append([])

            matchedClusters = self._process.getDataByPlane(thisPlane)

            for i in xrange(len(matchedClusters)):
                cluster = matchedClusters[i]



                # Check that this match is good
                if not cluster.is_good():
                    self._listOfClusters[thisPlane].append(None)
                    self._listOfCParams[thisPlane].append(None)
                    colorIndex += 1
                    if colorIndex >= len(self._matchColors):
                        colorIndex = 0
                    continue

                # If the match is good, make the cluster and such

                # Now make the cluster
                cluster_box_coll = boxCollection()
                cluster_box_coll.setColor(self._matchColors[colorIndex])
                cluster_box_coll.setPlane(thisPlane)

                # Keep track of the cluster for drawing management
                self._listOfClusters[thisPlane].append(cluster_box_coll)
                self._listOfCParams[thisPlane].append(None)

                # draw the hits in this cluster:
                cluster_box_coll.drawHits(view, cluster)
                
                colorIndex += 1
                if colorIndex >= len(self._matchColors):
                    colorIndex = 0

                if self._drawParams:
                    if cluster.params().N_Hits > 10:

                        cParams = clusterParams(
                            cluster.params(), view_manager._geometry)
                        self._listOfCParams[thisPlane][-1] = cParams
                        cluster_box_coll.attachParams(cParams)
                        self._listOfCParams[thisPlane][-1].draw(view)

                        # Connect the cluster to the params:
                        self._listOfClusters[thisPlane][-1].connect(
                            self._listOfCParams[thisPlane][-1]
                        )

                        # Connect the params to the cluster:
                        self._listOfCParams[thisPlane][-1].connect(
                            self._listOfClusters[thisPlane][-1]
                        )

        # Now, connect all the clusters across the planes to make the highlights match:
        # Check that the vectors are the same across all planes first
        for plane in xrange(len(self._listOfClusters)):
            if len(self._listOfClusters[plane]) != len(self._listOfClusters[0]):
                print("ERROR: matched cluster sizes don't match")
                exit(-1)

        # At this point, we've confirmed that everything is at least the same size
        # So go through and try to connect everything
        for plane in xrange(len(self._listOfClusters)):
            for i_cluster in xrange(len(self._listOfClusters[plane])):
                if self._listOfClusters[plane][i_cluster] != None:
                    # In this case, connect to the other clusters.
                    otherPlanes = range(len(self._listOfClusters))
                    otherPlanes.remove(plane)
                    for otherPlane in otherPlanes:
                        if self._listOfClusters[otherPlane][i_cluster] != None:
                            self._listOfClusters[plane][i_cluster].connect(
                                self._listOfClusters[otherPlane][i_cluster])


    def clearDrawnObjects(self, view_manager):
        i_plane = 0
        # erase the clusters
        for plane in self._listOfClusters:
            view = view_manager.getViewPorts()[i_plane]
            i_plane += 1
            for cluster in plane:
                if cluster != None:
                    cluster.clearHits(view)
        self._listOfClusters = []

        # Erase the cparams
        i_plane = 0
        for plane in self._listOfCParams:
            view = view_manager.getViewPorts()[i_plane]
            i_plane += 1
            for cparams in plane:
                if cparams != None:
                    cparams.clear(view)

        self._listOfCParams = []

# Shower drawing is currently "experimental"
