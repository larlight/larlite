import ROOT
from ROOT import larlite as fmwk
from ROOT import TCanvas
import sys,numpy
from play import *

def dir(r1,r2):

    length=dist(r1,r2)
    return ( (r2.X() - r1.X())/length,
             (r2.Y() - r1.Y())/length,
             (r2.Z() - r1.Z())/length )

def dist(r1,r2):

    return numpy.sqrt( numpy.power(r1.X()-r2.X(),2) +
                       numpy.power(r1.Y()-r2.Y(),2) +
                       numpy.power(r1.Z()-r2.Z(),2) )


def middle(r1,r2):

    return (r1.X() + (r2.X()-r1.X()) / 2.,
            r1.Y() + (r2.Y()-r1.Y()) / 2.,
            r1.Z() + (r2.Z()-r1.Z()) / 2.)

s=fmwk.storage_manager()

s.set_io_mode(fmwk.storage_manager.kREAD)

for x in xrange(len(sys.argv)-1):

    s.add_in_filename(sys.argv[x+1])

s.open()
c1=TCanvas("c1","",600,500)
c2=TCanvas("c2","",600,500)
while s.next_event():

    mct_v=s.get_data(fmwk.data.kMCTrack,'mcreco')

    flash_v=s.get_data(fmwk.data.kOpFlash,'opflash')

    mct = mct_v[0]

    pt_v=[]
    for i in xrange(mct.size()-1):

        step1 = mct[i]
        step2 = mct[i+1]

        step_size = dist(step1,step2)

        if step_size < 5:
            mid_pt = middle(step1,step2)
            pt_v.append( (mid_pt[0],
                          mid_pt[1],
                          mid_pt[2],
                          step1.E() - step2.E()) )
        else:
            nsteps = int(step_size / 5.)
            ux, uy, uz = dir(step1,step2)
            unit_q = (step2.E() - step1.E())/(float(nsteps+1))
            for j in xrange(nsteps):
                pt_v.append(((step1.X() + ux * (2.5+5*j)),
                             (step1.Y() + uy * (2.5+5*j)),
                             (step1.Z() + uz * (2.5+5*j)),
                             unit_q) )
            pt_v.append( (step2.X(),
                          step2.Y(),
                          step2.Z(),
                          step2.E() - step1.E() - unit_q*nsteps) )
    c1.cd()
    g=hypothesis(pt_v)
    g.Draw("AP")
    c1.Update()

    c2.cd()
    g2=TGraph(32)
    q_v=[0 for x in xrange(32)]
    for i in xrange(flash_v.size()):
        f = flash_v[i]
        if f.TotalPE() < 10: continue
        for j in xrange(32):
            q_v[j] += f.PE(j)
    q_max=0
    for i in xrange(32):
        if q_v[i] > q_max: q_max = q_v[i]
    for i in xrange(32):
        g2.SetPoint(i,i,q_v[i]/q_max)
    g2.SetMarkerStyle(22)
    g2.SetMarkerColor(ROOT.kRed)
    g2.SetMarkerSize(1)
    g2.Draw("AP")
    c2.Update()
    sys.stdin.readline()
    del g
    del g2
s.close()
