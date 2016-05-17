import hit
import match
import shower
import track
import wire
import cluster
import endpoint2d
import vertex
import mctrack
import mcshower
import spacepoint
import simch
import opflash
import seed
import pfpart
import roi2d
import neutrino

# This is the class that maintains the list of drawable items.
# If your class isn't here, it can't be drawn
import collections


class drawableItems(object):

    """This class exists to enumerate the drawableItems"""
    # If you make a new drawing class, add it here

    def __init__(self):
        super(drawableItems, self).__init__()
        # items are stored as pointers to the classes (not instances)
        self._drawableClasses = collections.OrderedDict()
        self._drawableClasses.update({'Hit': [hit.hit,"hit"]})
        self._drawableClasses.update({'Cluster': [cluster.cluster,"cluster"]})
        self._drawableClasses.update({'Match': [match.match,"pfpart"]})
        self._drawableClasses.update({'Shower': [shower.shower,"shower"]})
        self._drawableClasses.update({'Track': [track.track,"track"]})
        self._drawableClasses.update({'Neutrino': [neutrino.neutrino,"ass"]})
        self._drawableClasses.update({'Endpoint 2D': [endpoint2d.endpoint2d,"endpoint2d"]})
        self._drawableClasses.update({'Vertex': [vertex.vertex,"vertex"]})
        self._drawableClasses.update({'roi2d': [roi2d.roi2d,"PiZeroROI"]})
        self._drawableClasses.update({'SPS': [spacepoint.spacepoint,"sps"]})

    def getListOfTitles(self):
        return self._drawableClasses.keys()

    def getListOfItems(self):
        return zip(*self._drawableClasses.values())[1]

    def getDict(self):
        return self._drawableClasses


try:
    import pyqtgraph.opengl as gl
    class drawableItems3D(object):

        """This class exists to enumerate the drawableItems in 3D"""
        # If you make a new drawing class, add it here

        def __init__(self):
            super(drawableItems3D, self).__init__()
            # items are stored as pointers to the classes (not instances)
            self._drawableClasses = collections.OrderedDict()
            self._drawableClasses.update({'Spacepoints': [spacepoint.spacepoint3D,"sps"]})
            self._drawableClasses.update({'PFParticle': [pfpart.pfpart3D,"pfpart"]})
            self._drawableClasses.update({'Seed': [seed.seed3D,"seed"]})
            self._drawableClasses.update({'Vertex': [vertex.vertex3D,"vertex"]})
            self._drawableClasses.update({'Shower': [shower.shower3D,"shower"]})
            self._drawableClasses.update({'Track': [track.track3D,"track"]})
            self._drawableClasses.update({'Opflash': [opflash.opflash3D,"opflash"]})
            self._drawableClasses.update({'MCTrack': [mctrack.mctrack3D,"mctrack"]})
            self._drawableClasses.update({'MCShower': [mcshower.mcshower3D,"mcshower"]})
            self._drawableClasses.update({'Simch': [simch.simch3D,"simch"]})

        def getListOfTitles(self):
            return self._drawableClasses.keys()

        def getListOfItems(self):
            return zip(*self._drawableClasses.values())[1]

        def getDict(self):
            return self._drawableClasses



except:
    pass

