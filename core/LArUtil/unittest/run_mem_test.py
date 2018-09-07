import os,sys
from ROOT import std
from larlite import larlite
from larlite import larutil

for x in xrange(100000):
    if x%1000==0:
        print x
    sce = larutil.SpaceChargeMicroBooNE()
    #print sce
    del sce


print "press entry to end"
raw_input()
