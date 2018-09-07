import ROOT as rt
from ROOT import std
from larlite import larlite
from larlite import larutil

sce = larutil.SpaceChargeMicroBooNE()
print sce

NSAMPLES=10000
gen = rt.TRandom3(10)
pos_list = []

DY=2*117


foutpos = open('poslist.txt','w')
foutoff = open('offlist.txt','w')

for x in xrange(NSAMPLES):
    pos = std.vector("float")(3,0.0)
    pos[0] = gen.Rndm()*250.0
    pos[1] = gen.Rndm()*234.0-117.0
    pos[2] = gen.Rndm()*1000.0
    offsets = sce.GetPosOffsets( pos[0], pos[1], pos[2] )
    print >> foutpos,pos[0]," ",pos[1]," ",pos[2]
    print >> foutoff,offsets[0]," ",offsets[1]," ",offsets[2]
foutpos.close()
foutoff.close()
    
