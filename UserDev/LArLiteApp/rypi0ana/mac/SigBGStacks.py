import os,sys
import ROOT 


#Bring in the root files 
outfile = ROOT.TFile("tree.root", "recreate");



sig = ROOT.TFile("ncpi0_Signal.root")
pi0x = ROOT.TFile("bg_ncpi0x.root")
nopi0 = ROOT.TFile("bg_ncnopi0.root")
multipi0 = ROOT.TFile("bg_ncmultipi0.root")
ccpi0 = ROOT.TFile("bg_ccpi0.root")
ccnopi0 = ROOT.TFile("bg_ccnopi0.root")
#cos = ROOT.TFile("Cosmics_bg.root")

#Make the stack for Bg

bgstack = ROOT.THStack("Bgs","Mass peak Backgrounds")
mbgstack = ROOT.THStack("mBgs","Pi0 Momentum Backgrounds")

pi0x.MASSRECO.SetLineColor(1)
pi0x.MASSRECO.SetFillColor(5)
nopi0.MASSRECO.SetLineColor(1)
nopi0.MASSRECO.SetFillColor(8)
multipi0.MASSRECO.SetLineColor(1)
multipi0.MASSRECO.SetFillColor(6)
ccpi0.MASSRECO.SetLineColor(1)
ccpi0.MASSRECO.SetFillColor(4)
ccnopi0.MASSRECO.SetLineColor(1)
ccnopi0.MASSRECO.SetFillColor(3)
#cos.MASSRECO.SetLineColor(1)
#cos.MASSRECO.SetFillColor(7)

pi0x.MOMENTUMRECO.SetLineColor(1)
pi0x.MOMENTUMRECO.SetFillColor(5)
nopi0.MOMENTUMRECO.SetLineColor(1)
nopi0.MOMENTUMRECO.SetFillColor(8)
multipi0.MOMENTUMRECO.SetLineColor(1)
multipi0.MOMENTUMRECO.SetFillColor(6)
ccpi0.MOMENTUMRECO.SetLineColor(1)
ccpi0.MOMENTUMRECO.SetFillColor(4)
ccnopi0.MOMENTUMRECO.SetLineColor(1)
ccnopi0.MOMENTUMRECO.SetFillColor(3)
#cos.MOMENTUMRECO.SetLineColor(1)
#cos.MOMENTUMRECO.SetFillColor(7)
#
print " Events that reconstruct 1 and only 1 pi0"
print"Signal   = " ,sig.MASSRECO.GetEntries() 
print"1pi0X    = " ,pi0x.MASSRECO.GetEntries() 
print"multipi0 = " ,multipi0.MASSRECO.GetEntries() 
print"nopi0    = " ,nopi0.MASSRECO.GetEntries() 
print"ccpi0    = " ,ccpi0.MASSRECO.GetEntries() 
print"ccnopi0  = " ,ccnopi0.MASSRECO.GetEntries() 

print"\n"
stob = sig.MASSRECO.GetEntries()/(ccpi0.MASSRECO.GetEntries()+nopi0.MASSRECO.GetEntries()+pi0x.MASSRECO.GetEntries()+multipi0.MASSRECO.GetEntries()+ccnopi0.MASSRECO.GetEntries()) 

print"Signal/BG  = " , stob


 
bgstack.Add(multipi0.MASSRECO)
bgstack.Add(nopi0.MASSRECO)
bgstack.Add(pi0x.MASSRECO)
bgstack.Add(ccnopi0.MASSRECO)
bgstack.Add(ccpi0.MASSRECO)
bgstack.Draw()

Sbg = bgstack.Clone("SigBgOverlay")

mbgstack.Add(multipi0.MOMENTUMRECO)
mbgstack.Add(nopi0.MOMENTUMRECO)
mbgstack.Add(pi0x.MOMENTUMRECO)
mbgstack.Add(ccnopi0.MOMENTUMRECO)
mbgstack.Add(ccpi0.MOMENTUMRECO)
mbgstack.Draw()


# Write them to the output 
outfile.cd()
bgstack.Write()
mbgstack.Write()
sig.MASSRECO.Write()
sig.MOMENTUMRECO.Write()
outfile.Close()

