import os,sys
from ROOT import std
from larlite import larlite
from larlite import larutil

sce = larutil.SpaceChargeMicroBooNE()
print sce

fpos = open("poslist.txt",'r')
foff = open("offlist.txt",'r')

poslines = fpos.readlines()
offlines = foff.readlines()

fpos.close()
foff.close()

nbad = 0
for n in xrange(len(poslines)):
    lpos = poslines[n].strip().split()
    pos = [ float(lpos[x]) for x in xrange(len(lpos)) ]
    #print pos
    
    loff = offlines[n].strip().split()
    off = [ float(loff[x]) for x in xrange(len(loff)) ]
    #print off

    offsets = sce.GetPosOffsets( pos[0], pos[1], pos[2] )

    diff  = [ offsets.at(x)-off[x] for x in xrange(len(off)) ]
    #print diff
    for d in diff:
        if d>1.0e-6:
            nbad += 1
            break

print "Number of different values: ",nbad," of ",len(poslines)
if nbad==0:
    sys.exit(0)
else:
    sys.exit(1)
