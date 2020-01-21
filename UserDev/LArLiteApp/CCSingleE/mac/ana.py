import sys,array
from ROOT import *

ch=TChain("_result_tree")
ch.AddFile(sys.argv[1])

chBkgd=TChain("_result_tree")
chBkgd.AddFile(sys.argv[2])

# Cut value arrays
fid_cuts    = [ x   for x in range(100) ] # 5cm steps, spans from 0=>60 cm
BDtW_cuts = [ x*5 for x in range(80)   ] # 5cm steps, spans from 0=>100 cm
BDtTW_cuts = [ x*5 for x in range(100)   ] # 5cm steps, spans from 0=>100 cm
angle_cuts = [x*5 for x in range(40,72) ] #5degree steps from 200->360 cm

# Efficiency arrays for first daughter
fidSig_eff    = [0 for x in fid_cuts]
BDtWSig_eff = [0 for x in BDtW_cuts]
BDtTWSig_eff = [0 for x in BDtTW_cuts]
angleSig_eff = [0 for x in angle_cuts]

fidBkgd_eff    = [0 for x in fid_cuts]
BDtWBkgd_eff = [0 for x in BDtW_cuts]
BDtTWBkgd_eff = [0 for x in BDtTW_cuts]
angleBkgd_eff = [0 for x in angle_cuts]

c=TCanvas("c","",600,500)

for x in range(len(fid_cuts)):
    fidSig_eff[x] = float(ch.GetEntries("_distToWall>=%g" % fid_cuts[x]))/ch.GetEntries()
    fidBkgd_eff[x] = 1-float(chBkgd.GetEntries("_distToWall>=%g" % fid_cuts[x]))/chBkgd.GetEntries()
 #   print 'Fid eff, cuts: ', fidSig_eff[x], ' ',fid_cuts[x]

for x in range(len(BDtW_cuts)):
    BDtWSig_eff[x] = float(ch.GetEntries("_distToWall > 0 && _distBackAlongTraj>=%g" % BDtW_cuts[x]))/ch.GetEntries()
    BDtWBkgd_eff[x] = 1-float(chBkgd.GetEntries(" _distToWall > 0 &&_distBackAlongTraj>=%g" % BDtW_cuts[x]))/chBkgd.GetEntries()

for x in range(len(BDtTW_cuts)):
    BDtTWSig_eff[x] = float(ch.GetEntries(" _distToTopWall>=%g" % BDtTW_cuts[x]))/ch.GetEntries()
    BDtTWBkgd_eff[x] = 1-float(chBkgd.GetEntries("_distToTopWall>=%g" % BDtTW_cuts[x]))/chBkgd.GetEntries()

for x in range(len(angle_cuts)):
    angleSig_eff[x] = float(ch.GetEntries("( _distToWall > 5 && (_distToTopWall < 0 || _distToTopWall > 30) && _angle_Norm <180 || _angle_Norm>=%g)" % angle_cuts[x]))/ch.GetEntries()
    angleBkgd_eff[x] = 1-float(chBkgd.GetEntries("_distToWall > 5 && (_distToTopWall < 0 || _distToTopWall > 30) && (_angle_Norm <180 || _angle_Norm>=%g)" % angle_cuts[x]))/chBkgd.GetEntries()


angleSig_eff_xvalues = array.array('d', angle_cuts)
angleSig_eff_yvalues = array.array('d', angleSig_eff)
angleSig_eff_graph = TGraphErrors(len(angle_cuts), angleSig_eff_xvalues, angleSig_eff_yvalues)
angleSig_eff_graph.SetMarkerStyle(20)
angleSig_eff_graph.SetMarkerColor(kBlue)
angleSig_eff_graph.SetTitle("Efficiency Angle Cut, (180 - AngleCut) deg")

angleSig_eff_graph.SetMaximum(1)
angleSig_eff_graph.SetMinimum(0)
angleSig_eff_graph.GetXaxis().SetTitle("Angle Cut [deg]")
angleSig_eff_graph.GetYaxis().SetTitle("Efficiency")

angleBkgd_eff_xvalues = array.array('d', angle_cuts)
angleBkgd_eff_yvalues = array.array('d', angleBkgd_eff)
angleBkgd_eff_graph = TGraphErrors(len(angle_cuts), angleBkgd_eff_xvalues, angleBkgd_eff_yvalues)
angleBkgd_eff_graph.SetMarkerStyle(20)
angleBkgd_eff_graph.SetMarkerColor(kRed)

#angleSig_eff_graph.Draw("AP")
#angleBkgd_eff_graph.Draw("P")
#c.Update()
#sys.stdin.readline()

fidSig_eff_xvalues = array.array('d', fid_cuts)
fidSig_eff_yvalues = array.array('d', fidSig_eff)
fidSig_eff_graph = TGraphErrors(len(fid_cuts), fidSig_eff_xvalues, fidSig_eff_yvalues)
fidSig_eff_graph.SetMarkerStyle(20)
fidSig_eff_graph.SetMarkerColor(kBlue)
fidSig_eff_graph.SetTitle("Efficiency Fid Vol Cut, E > 20MeV")

fidSig_eff_graph.SetMaximum(1)
fidSig_eff_graph.SetMinimum(0)
fidSig_eff_graph.GetXaxis().SetTitle("Fid Volume Cut [cm]")
fidSig_eff_graph.GetYaxis().SetTitle("Efficiency")

fidBkgd_eff_xvalues = array.array('d', fid_cuts)
fidBkgd_eff_yvalues = array.array('d', fidBkgd_eff)
fidBkgd_eff_graph = TGraphErrors(len(fid_cuts), fidBkgd_eff_xvalues, fidBkgd_eff_yvalues)
fidBkgd_eff_graph.SetMarkerStyle(20)
fidBkgd_eff_graph.SetMarkerColor(kRed)

fidSig_eff_graph.Draw("AP")
fidBkgd_eff_graph.Draw("P")
c.Update()
sys.stdin.readline()


#for x in xrange(len(BDtW_cuts)):
    #print BDtW_cuts[x], BDtWSig_eff[x],'...'

print()

BDtWSig_eff_xvalues = array.array('d', BDtW_cuts)
BDtWSig_eff_yvalues = array.array('d', BDtWSig_eff)
BDtWSig_eff_graph = TGraphErrors(len(BDtW_cuts), BDtWSig_eff_xvalues, BDtWSig_eff_yvalues)
BDtWSig_eff_graph.SetMarkerStyle(20)
BDtWSig_eff_graph.SetMarkerColor(kBlue)
BDtWSig_eff_graph.SetTitle("Efficiency BDtW Cut, E > 100MeV")

BDtWSig_eff_graph.SetMaximum(1)
BDtWSig_eff_graph.SetMinimum(0)
BDtWSig_eff_graph.GetXaxis().SetTitle("BDtW Cut [cm]")
BDtWSig_eff_graph.GetYaxis().SetTitle("Efficiency")

BDtWBkgd_eff_xvalues = array.array('d', BDtW_cuts)
BDtWBkgd_eff_yvalues = array.array('d', BDtWBkgd_eff)
BDtWBkgd_eff_graph = TGraphErrors(len(BDtW_cuts), BDtWBkgd_eff_xvalues, BDtWBkgd_eff_yvalues)
BDtWBkgd_eff_graph.SetMarkerStyle(20)
BDtWBkgd_eff_graph.SetMarkerColor(kRed)


BDtWSig_eff_graph.Draw("AP")
BDtWBkgd_eff_graph.Draw("P")
c.Update()
sys.stdin.readline()


BDtTWSig_eff_xvalues = array.array('d', BDtTW_cuts)
BDtTWSig_eff_yvalues = array.array('d', BDtTWSig_eff)
BDtTWSig_eff_graph = TGraphErrors(len(BDtTW_cuts), BDtTWSig_eff_xvalues, BDtTWSig_eff_yvalues)
BDtTWSig_eff_graph.SetMarkerStyle(20)
BDtTWSig_eff_graph.SetMarkerColor(kBlue)
BDtTWSig_eff_graph.SetTitle("Efficiency BDtTW Cut, E > 20MeV")

BDtTWSig_eff_graph.SetMaximum(1)
BDtTWSig_eff_graph.SetMinimum(0)
BDtTWSig_eff_graph.GetXaxis().SetTitle("BDtTW Cut [cm]")
BDtTWSig_eff_graph.GetYaxis().SetTitle("Efficiency")

BDtTWBkgd_eff_xvalues = array.array('d', BDtTW_cuts)
BDtTWBkgd_eff_yvalues = array.array('d', BDtTWBkgd_eff)
BDtTWBkgd_eff_graph = TGraphErrors(len(BDtTW_cuts), BDtTWBkgd_eff_xvalues, BDtTWBkgd_eff_yvalues)
BDtTWBkgd_eff_graph.SetMarkerStyle(20)
BDtTWBkgd_eff_graph.SetMarkerColor(kRed)


BDtTWSig_eff_graph.Draw("AP")
BDtTWBkgd_eff_graph.Draw("P")
c.Update()
sys.stdin.readline()

