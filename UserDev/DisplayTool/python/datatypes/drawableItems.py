import hit
import match
import shower
import track
import wire
import cluster

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
        self._drawableClasses.update({'Match': [match.match,"cluster"]})
        self._drawableClasses.update({'Shower': [shower.shower,"shower"]})
        self._drawableClasses.update({'Track': [track.track,"track"]})

    def getListOfTitles(self):
        return self._drawableClasses.keys()

    def getListOfItems(self):
        return zip(*self._drawableClasses.values())[1]

    def getDict(self):
        return self._drawableClasses

