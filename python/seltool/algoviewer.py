import sys, os
from ROOT import gSystem
from ROOT import ertool
from ROOT import larlite as fmwk
from basictool import GeoViewer, geoalgo
from seltool import ERViewer
import matplotlib.pyplot as plt

#viewers
plt.ion()

def getViewer(name):
    display = ERViewer(name)
    #display.set_window_title(name)
    return display



def viewAll(mcDisplay, mcData, mcPart, recoDisplay, recoData, recoPart):

    mcDisplay.clear()
    recoDisplay.clear()

    recoDisplay.add(recoPart,recoData,False)
    mcDisplay.add(mcPart,mcData,False)


    mcDisplay.show()
    recoDisplay.show()

    # The ranges for the two planes "should" be identical
    mcrange   = mcDisplay.get_ax_range()
    recorange = recoDisplay.get_ax_range()
    #for dim in xrange(3):
    #    if (recorange[0][dim] < mcrange[0][dim]): mcrange[0][dim] = recorange[0][dim]
    #    if (recorange[1][dim] > mcrange[1][dim]): mcrange[1][dim] = recorange[1][dim]

    #recoDisplay.set_ax_range(mcrange[0],mcrange[1])
    #mcDisplay.set_ax_range(mcrange[0],mcrange[1])


    


def view(display, data, part):

    display_reco = ERViewer("view")
    display_reco.set_window_title("Reco Objects")

   # del display_mc

    display_reco.clear()

    display_reco.add(part,data,False)

    display_reco.show()
