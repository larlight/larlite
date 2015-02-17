import sys, os
from ROOT import gSystem
from ROOT import ertool
from ROOT import larlite as fmwk
fmwk.geo.PlaneID
from basictool import GeoViewer, geoalgo
from seltool import ERViewer
import matplotlib.pyplot as plt

#viewers
plt.ion()
display_reco = ERViewer()
display_reco.set_window_title("Reco Objects")
display_mc   = ERViewer()
display_mc.set_window_title("MC Objects")


def viewAll(mcData, mcPart, recoData, recoPart):

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


def view(data, part):

    del display_mc

    display_reco.clear()

    display_reco.add(part,data,False)

    display_reco.show()
