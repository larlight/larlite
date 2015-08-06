import sys,array
from ROOT import *

ch=TChain("_pi0_tree")
ch.AddFile(sys.argv[1])

chReco=TChain("_pi0_tree")
chReco.AddFile(sys.argv[2])

# Cut value arrays
mom_cuts    = [ x*50 for x in xrange(2,20) ] # 100cm steps, spans from -100=>1600 cm
#print mom_cuts

# Efficiency arrays for first daughter
momSig_eff  = [0 for x in mom_cuts]
momReco_eff  = [0 for x in mom_cuts]

c=TCanvas("c","",600,500)

length = len(mom_cuts) - 1
print 'length is ', length  

total = 0
totalTrue = 0 

for x in xrange(length):
    print "Cuts are : ", mom_cuts[x], " and ", mom_cuts[x+1]
    print "entries between bounds: ", ch.GetEntries("_p_tot_pi0 >= %g && _p_tot_pi0 < %g" % (mom_cuts[x], mom_cuts[x+1]) )
    if ( ch.GetEntries("_p_tot_pi0 >= %g && _p_tot_pi0 < %g" % (mom_cuts[x], mom_cuts[x+1]) ) > 0 ):
	print "number of entries wiht 1 MCC: ", ch.GetEntries("_p_tot_pi0 >= %g && _p_tot_pi0 <= %g" % (mom_cuts[x], mom_cuts[x+1]))
	print "number of entries with 1 Reco: ", chReco.GetEntries("_nNC1Pi0 == 1 && _p_tot_pi0 >= %g && _p_tot_pi0 <= %g " % (mom_cuts[x],  mom_cuts[x+1]) )
	momSig_eff[x] = float(ch.GetEntries("_nNC1Pi0 == 1 && _p_tot_pi0 >= %g && _p_tot_pi0 <= %g " % (mom_cuts[x],  mom_cuts[x+1]) ))/ch.GetEntries("_p_tot_pi0 >= %g && _p_tot_pi0 <= %g" % (mom_cuts[x], mom_cuts[x+1]) )
	momReco_eff[x] = float(chReco.GetEntries("_nNC1Pi0 == 1 && _p_tot_pi0 >= %g && _p_tot_pi0 <= %g " % (mom_cuts[x],  mom_cuts[x+1]) ))/ch.GetEntries("_p_tot_pi0 >= %g && _p_tot_pi0 <= %g" % (mom_cuts[x], mom_cuts[x+1]) )

	total +=  ch.GetEntries("_nNC1Pi0 == 1 && _p_tot_pi0 >= %g && _p_tot_pi0 <= %g " % (mom_cuts[x],  mom_cuts[x+1]) )
	totalTrue +=  chReco.GetEntries("_p_tot_pi0 >= %g && _p_tot_pi0 <= %g " % (mom_cuts[x],  mom_cuts[x+1]) )

print "Total entries: ", total, ", ", totalTrue


momSig_eff_xvalues = array.array('d', mom_cuts[:length])
momSig_eff_yvalues = array.array('d', momSig_eff[:length])
momSig_eff_graph = TGraphErrors(length, momSig_eff_xvalues, momSig_eff_yvalues)
momSig_eff_graph.SetMarkerStyle(20)
momSig_eff_graph.SetMarkerColor(kRed)
momSig_eff_graph.SetTitle("Selection Efficiency of Pi0 vs Total Momentum")

momSig_eff_graph.SetMaximum(5)
momSig_eff_graph.SetMinimum(0)
momSig_eff_graph.GetXaxis().SetTitle("Momentum [MeV]")
momSig_eff_graph.GetYaxis().SetTitle("Efficiency")

momReco_eff_xvalues = array.array('d', mom_cuts[:length])
momReco_eff_yvalues = array.array('d', momReco_eff[:length])
momReco_eff_graph = TGraphErrors(length, momReco_eff_xvalues, momReco_eff_yvalues)
momReco_eff_graph.SetMarkerStyle(20)
momReco_eff_graph.SetMarkerColor(kBlue)
#momReco_eff_graph.SetTitle("Selection Efficiency of Pi0 vs Total Momentum")

momSig_eff_graph.Draw("AP")
momReco_eff_graph.Draw("P")
c.Update()
sys.stdin.readline()


#momXSig_eff_xvalues = array.array('d', mom_x_cuts)
#momXSig_eff_yvalues = array.array('d', momXSig_eff)
#momXSig_eff_graph = TGraphErrors(len(mom_x_cuts), momXSig_eff_xvalues, momXSig_eff_yvalues)
#momXSig_eff_graph.SetMarkerStyle(20)
#momXSig_eff_graph.SetMarkerColor(kBlue)
#momXSig_eff_graph.SetTitle("Selection Efficiency of Pi0 vs X Momentum")
#
#momXSig_eff_graph.SetMaximum(1)
#momXSig_eff_graph.SetMinimum(0)
#momXSig_eff_graph.GetXaxis().SetTitle("Momentum [MeV]")
#momXSig_eff_graph.GetYaxis().SetTitle("Efficiency")
#
#momXSig_eff_graph.Draw("AP")
#c.Update()
#sys.stdin.readline()
#
#momYSig_eff_xvalues = array.array('d', mom_y_cuts)
#momYSig_eff_yvalues = array.array('d', momYSig_eff)
#momYSig_eff_graph = TGraphErrors(len(mom_y_cuts), momYSig_eff_xvalues, momYSig_eff_yvalues)
#momYSig_eff_graph.SetMarkerStyle(20)
#momYSig_eff_graph.SetMarkerColor(kBlue)
#momYSig_eff_graph.SetTitle("Selection Efficiency of Pi0 vs Y Momentum")
#
#momYSig_eff_graph.SetMaximum(1)
#momYSig_eff_graph.SetMinimum(0)
#momYSig_eff_graph.GetXaxis().SetTitle("Momentum [MeV]")
#momYSig_eff_graph.GetYaxis().SetTitle("Efficiency")
#
#momYSig_eff_graph.Draw("AP")
#c.Update()
#sys.stdin.readline()
#
#momZSig_eff_xvalues = array.array('d', mom_z_cuts)
#momZSig_eff_yvalues = array.array('d', momZSig_eff)
#momZSig_eff_graph = TGraphErrors(len(mom_z_cuts), momZSig_eff_xvalues, momZSig_eff_yvalues)
#momZSig_eff_graph.SetMarkerStyle(20)
#momZSig_eff_graph.SetMarkerColor(kBlue)
#momZSig_eff_graph.SetTitle("Selection Efficiency of Pi0 vs Z Momentum")
#
#momZSig_eff_graph.SetMaximum(1)
#momZSig_eff_graph.SetMinimum(0)
#momZSig_eff_graph.GetXaxis().SetTitle("Momentum [MeV]")
#momZSig_eff_graph.GetYaxis().SetTitle("Efficiency")
#
#momZSig_eff_graph.Draw("AP")
#c.Update()
#sys.stdin.readline()









