#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT, sys
from ROOT import *
from ROOT import gSystem
import time

gSystem.Load("libLArLite_Base")
gSystem.Load("libLArLite_Analysis")
gSystem.Load("libLArLite_LArUtil")
gSystem.Load("libBasicTool_GeoAlgo")

# Now import ana_processor & your class. For this example, ana_base.
from ROOT import larlite as fmwk
from ROOT import geoalgo

example = geoalgo.SegmentPoCA()

p1 = ROOT.vector('double')()
p1.push_back(0.01)
p1.push_back(0.02)
p1.push_back(-0.01)

p2 = ROOT.vector('double')()
p2.push_back(2)
p2.push_back(0)
p2.push_back(0)

t1 = ROOT.vector('double')()
t1.push_back(0.3)
t1.push_back(0.02)
t1.push_back(1.3)

t2 = ROOT.vector('double')()
t2.push_back(3.34)
t2.push_back(-0.04)
t2.push_back(5.0)

example.TestPoCA(p1,p2,t1,t2)


# done!
