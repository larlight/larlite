import os,sys
from ROOT import std
from larlite import larlite
from larlite import larutil

for x in xrange(1000000):
    if x%1000==0:
        print x
    sce = larutil.SpaceChargeMicroBooNE()
    del sce
    #print sce

    # control sample
    #io = larlite.storage_manager()
    #del io




print "press entry to end"
raw_input()
