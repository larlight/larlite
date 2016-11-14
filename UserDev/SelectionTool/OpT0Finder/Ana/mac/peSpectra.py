import sys,array
from ROOT import *

# This script runs on the output (out.root) of running anaunit PEDistAna: 
# > python run_mcflashana.py INPUT_FILE
# INPUT_FILE needs to contain simch, ophit, opflash, opdigit,trigger information.
# Be sure to set the proper producers in run_mcflashana
# > python peSpectra.py out.root
# This script produces plots like those in docdb# 5026 slides 17,18

fout=TFile.Open("plots.root","RECREATE")

event=TChain("ev_tree")
event.AddFile(sys.argv[1])

x_array = [x for x in xrange(32)]

c=TCanvas("c","",600,500)


j = 0

for e in event:

    g4_xvalues = array.array('d', x_array)
    g4_yvalues = array.array('d', event.g4_pe_v)
    g4_graph = TGraphErrors(32, g4_xvalues, g4_yvalues)
    g4_graph.SetMarkerStyle(20)
    g4_graph.SetMarkerColor(kRed)
    g4_graph.SetTitle("PE Distrib Comparison, G4(red), WF(green), OpHit(blue), OpFlash(yellow) ")
    g4_graph.GetXaxis().SetTitle("PMT") 
    g4_graph.GetYaxis().SetTitle("Number PE hit")
    g4_graph.SetMaximum(1000)
    g4_graph.SetMinimum(0)


    wf_xvalues = array.array('d', x_array)
    wf_yvalues = array.array('d', event.wf_pe_v)
    wf_graph = TGraphErrors(32, wf_xvalues, wf_yvalues)
    wf_graph.SetMarkerStyle(20)
    wf_graph.SetMarkerColor(kGreen)

    hit_xvalues = array.array('d', x_array)
    hit_yvalues = array.array('d', event.hit_pe_v)
    hit_graph = TGraphErrors(32, hit_xvalues, hit_yvalues)
    hit_graph.SetMarkerStyle(20)
    hit_graph.SetMarkerColor(kBlue)

    flash_xvalues = array.array('d', x_array)
    flash_yvalues = array.array('d', event.flash_pe_v)
    flash_graph = TGraphErrors(32, flash_xvalues, flash_yvalues)
    flash_graph.SetMarkerStyle(20)
    flash_graph.SetMarkerColor(kYellow)


    g4_graph.Draw("AP")
    wf_graph.Draw("P")
    hit_graph.Draw("P")
    flash_graph.Draw("P")

    c.Update()
    gStyle.SetOptStat(0)
    gApplication.Run()
    sys.stdin.readline()


    g4_graph.SetName("g4_%g" % j)
    wf_graph.SetName("wf_%g" % j)
    hit_graph.SetName("hit_%g" % j)
    flash_graph.SetName("flash_%g" % j)
    c.SetName("all_%g" % j )
    fout.cd()

    g4_graph.Write()
    wf_graph.Write()
    hit_graph.Write()
    flash_graph.Write()
    c.Write()
    j += 1

fout.Close()

