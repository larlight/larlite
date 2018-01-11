import sys, os, ROOT
from ROOT import *
gSystem.Load("libAnalysis")


if len(sys.argv) != 2:
    print
    print "*** Improper usage. Usage: python viewclusters.py /path/to/input/file.root ***"
    print


filename = sys.argv[1]
#filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlight_11.root"


my_proc = larlight.ana_processor()
my_proc.set_verbosity(larlight.MSG.DEBUG)
my_proc.set_io_mode(larlight.storage_manager.READ)
my_proc.add_input_file(filename)
larlight.storage_manager.get().set_in_rootdir("scanner")
larlight.storage_manager.get().set_data_to_read(larlight.DATA.MCTruth,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Shower,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.Calorimetry,False)
larlight.storage_manager.get().set_data_to_read(larlight.DATA.UserInfo,False)

my_proc.set_ana_output_file("")

#my_ana0 = larlight.ClusterMergeAlg()
my_ana0 = larreco.ClusterMergeAlg()
my_ana= larlight.ClusterViewer()

my_proc.add_process(my_ana0)
my_proc.add_process(my_ana)



gStyle.SetOptStat(0)

gStyle.SetOptStat(0)

c=TCanvas("c","Wire v. Time Cluster Viewer",900,600)
c.Divide(2,3)

c_graph=TCanvas("csub","Individual cluster start/end points",500,300)

while my_proc.process_event():
    currentview = 0;
    #First fill the 6 pads on the main canvas with stuff
    for pad in xrange(1,7,2):
        
        c.cd(pad)
        
        vReco = my_ana.GetHisto_Reco(int(currentview))
        vReco.at(0).Draw()
        for x in xrange(1,vReco.size()):
            vReco.at(x).Draw("same")
    
        c.cd(pad+1)
        my_ana.GetHisto_Hits(int(currentview)).Draw("COLZ")
    
        currentview = currentview + 1
        c.Update()
    

    #now draw individual clusters on the separate canvas
    c_graph.cd()
    for iview in xrange(0,3):
        for iclus in xrange(my_ana.GetClusterGraph_Reco(int(iview),bool(true)).size()):
            gstart=my_ana.GetClusterGraph_Reco(int(iview),bool(true)).at(iclus)
            gend  =my_ana.GetClusterGraph_Reco(int(iview),bool(false)).at(iclus)
            xmin=my_ana.GetHisto_Hits(int(iview)).GetXaxis().GetXmin()
            xmax=my_ana.GetHisto_Hits(int(iview)).GetXaxis().GetXmax()
            ymin=my_ana.GetHisto_Hits(int(iview)).GetYaxis().GetXmin()
            ymax=my_ana.GetHisto_Hits(int(iview)).GetYaxis().GetXmax()
            gstart.GetXaxis().SetLimits(xmin,xmax)
            gend.GetXaxis().SetLimits(xmin,xmax)       
            gstart.GetYaxis().SetRangeUser(ymin,ymax)
            gend.GetYaxis().SetRangeUser(ymin,ymax)
            gstart.SetTitle("View: %d, Cluster: %d"%(iview+1,iclus))
            gstart.SetMarkerSize(3)
            gstart.SetMarkerStyle(30)
            gend.SetMarkerSize(3)
            gend.SetMarkerStyle(29)
            gstart.Draw("ALP")
            gend.Draw("LP")
            my_ana.GetHisto_Reco(int(iview)).at(iclus).Draw("same")
            leg = TLegend(0.6,0.65,0.88,0.85)
            leg.AddEntry(gstart,"Start Point","p")
            leg.AddEntry(gend,"End Point","p")
            leg.Draw()
            c_graph.Update()
            print "Drawing cluster %d out of %d for view %d. To look at the next cluster hit enter." % (iclus,my_ana.GetClusterGraph_Reco(int(iview),bool(true)).size()-1,iview+1)
            sys.stdin.readline()

    print "Hit Enter to continue to next evt..."
    sys.stdin.readline()

# done!
