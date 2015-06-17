import sys, os
from ROOT import gSystem
from ROOT import ertool
from ROOT import larlite as fmwk
from basictool import GeoViewer, geoalgo
from seltool import ERViewer
import matplotlib.pyplot as plt

#viewers
plt.ion()

def setDisplay(name):

    display = ERViewer()
    display.set_window_title(name)    
    
    return display


def viewAll(mcData, mcPart, recoData, recoPart, display_mc, display_reco):


    display_reco.clear()
    display_mc.clear()

    print "display reco"
    display_reco.add(recoPart,recoData,False)
    print "display mc"
    display_mc.add(mcPart,mcData,False)

    display_mc.show()
    display_reco.show()
    
    # The ranges for the two planes "should" be identical
    mcrange = display_mc.get_ax_range()
    recorange = display_reco.get_ax_range()
    for dim in xrange(3):
        if (recorange[0][dim] < mcrange[0][dim]): mcrange[0][dim] = recorange[0][dim]
        if (recorange[1][dim] > mcrange[1][dim]): mcrange[1][dim] = recorange[1][dim]

    display_reco.set_ax_range(mcrange[0],mcrange[1])
    display_mc.set_ax_range(mcrange[0],mcrange[1])


def view(data, part, display):

    display.clear()

    display.add(part,data,False)

    display.show()
