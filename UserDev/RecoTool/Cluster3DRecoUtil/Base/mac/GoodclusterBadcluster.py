import sys
from array import array
from ROOT import *
import ROOT
gSystem.Load("libBase")
gSystem.Load("libLArUtil")
gSystem.Load("libClusterRecoUtil")
gSystem.Load("libFANN")
from ROOT import larlight as fmwk
from ROOT import cluster


import argparse

parser = argparse.ArgumentParser(description="Python script to process and merge showers.")
group = parser.add_mutually_exclusive_group()
group.add_argument("-v", "--verbose", help="Turn on verbose output",
                    action="store_true")
group.add_argument("-q", "--quiet", help="Turn off most output",
                    action="store_true")
parser.add_argument("-s","--source",nargs='*',help="Name of input file")
parser.add_argument("-o","--data-output",help="Output data file, if event is changed")
parser.add_argument("-a","--ana-output",help="Analysis output file")
parser.add_argument("-n","--num-events",help="Number of events to process")
parser.add_argument("-d","--display",help="Turn on the display to see each view before and after." )
args = parser.parse_args()

if len(sys.argv) == 1:
    print "\n-------You forgot to include a source file!-------\n"
    parser.print_help()

if args.verbose:
    print "Verbose mode turned on."
    if args.source != None:
        print "\tSource file is " + args.source
    if args.data_output != None:
        print "\tData output file is " + args.data_output
    if args.ana_output != None:
        print "\tAna output file is " + args.ana_output

if args.source == None:
    print "Error: please specificy an input file with -s or --source."
    quit()

if args.num_events == None:
    args.num_events = -1

if args.data_output == None:
    args.data_output = "default_event_output.root"
    print "No event output file selected.  If necessary, output will go to:"
    print "\t"+args.data_output

if args.ana_output == None:
    args.ana_output = "default_ana_output.root"
    print "No ana output file selected.  If necessary, output will go to:"
    print "\t"+args.ana_output



# ana_proc = larlight.ana_processor()

# if args.verbose:
    # ana_proc.set_verbosity(larlight.MSG.DEBUG)

# Not sure what this does
# ana_proc.set_io_mode(larlight.storage_manager.BOTH)

# Add the input file.  Not sure if the above takes multiple input files yet
# ana_proc.add_input_file(args.source)

mgr = fmwk.storage_manager()

mgr.set_io_mode(mgr.READ)

for source in args.source:
    mgr.add_in_filename(source)

if len(sys.argv) > 2:
    mgr.set_in_rootdir("scanner")

mgr.open()



algo = cluster.ClusterParamsExecutor()


algo.SetUseHitBlurring(false);

processed_events=0

fGSer = larutil.GeometryUtilities.GetME()

#Setting up TTree to store histograms
HistoFile=ROOT.TFile("outputhistos.root","recreate")
HistoTree=ROOT.TTree("HistoTree","Tree for Histograms")

#All Histograms are here
hArea=ROOT.TH1D("hArea", "Area for all Polygon",  1000, 0, 200)
hSmallArea=ROOT.TH1D("SmallPolyArea", "Small Area for all Polygon",  500, 0, 10)
hPolyHitDensity=ROOT.TH1D("Poly Hit Density", "Poly Hit Density",  500, 0, 25)
hPolyChargeDensity=ROOT.TH1D("Poly Charge Density", "Poly Charge Density",  500, 0, 10000)
hHitDensityVSHits=ROOT.TH2D("HitDensityVSHits", "hit density vs cluster hits; Hits/Area; Number of Hits in Cluster",
                            200, 0, 25, 200, 0, 800)
hHitDensityVSCharge=ROOT.TH2D("HitDensityVSCharge", "hit density vs cluster charge; Hits/Area; Cluster Charge",
                              200, 0, 25, 200, 0, 300000)
hHitDensityVSChargePerHit=ROOT.TH2D("HitDensityVSChargePerHit", "hit density vs charge/hit; Hits/Area; Charge in Cluster / Hits in Cluster",
                                    200, 0, 25, 200, 0, 5000)
AreaVSHits=ROOT.TH2D("AreaVSHits", "Area vs Hits in Cluster; Area; Number of Hits in Cluster",
                     1000, 0, 200, 200, 0, 1000)
AreaVSCharge=ROOT.TH2D("AreaVSCharge", "Area vs Charge in Cluster; Area; Cluster Charge",
                       1000, 0, 200, 200, 0, 300000)
AreaVSHitDensity=ROOT.TH2D("AreaVSHitDensity", "Area VS Hit Density; Area; Hit Density",
                           1000, 0, 200, 200, 0, 25)


while mgr.next_event():

    # Get event_cluster ... std::vector<larlight::cluster>
    cluster_v = mgr.get_data(fmwk.DATA.FuzzyCluster)

   #PdgCode

    if args.num_events == processed_events:
        exit()
        
    if not cluster_v:
        continue

    print "Event:",cluster_v.event_id()

    for x in xrange(cluster_v.size()):

        #Hits as vector of hits

        algo.LoadCluster(cluster_v.at(x),
                         mgr.get_data(cluster_v.get_hit_type()))

        # algo.FillParams(True,True,True,True,True)
        algo.GetAverages(True)
        #algo.GetRoughAxis(True)
        #algo.GetProfileInfo(True)
        #algo.RefineStartPoints(True)
        #algo.RefineDirection(True)
        algo.FillPolygon()


#
#      fGSer->SelectPolygonHitList(fHitVector,fParams.container_polygon);
#

        # algo.GetFinalSlope(True)
        #algo.Report()
        
        result = algo.GetParams()
        #geometric area of polygon
        polyArea = result.PolyObject.Area()
        #number of hits in cluster
        Nhits = result.N_Hits
        TotCharge = result.sum_charge
        #twoDHitDensity = result.hit_density_2D
        if (polyArea != 0.0 ):
            PolyHitDensity = Nhits/polyArea
            PolyChargeDensity = TotCharge/polyArea
            hPolyHitDensity.Fill(PolyHitDensity)
            hPolyChargeDensity.Fill(PolyChargeDensity)
            hHitDensityVSHits.Fill( Nhits/polyArea, Nhits)
            hHitDensityVSCharge.Fill( Nhits/polyArea, TotCharge)
            hHitDensityVSChargePerHit.Fill( Nhits/polyArea, TotCharge/Nhits)
            AreaVSHitDensity.Fill( polyArea, Nhits/polyArea)
        hArea.Fill(polyArea)
        AreaVSHits.Fill( polyArea, Nhits)
        AreaVSCharge.Fill( polyArea, TotCharge)
        if (polyArea < 50.0):
            hSmallArea.Fill(polyArea)

        HistoTree.Fill()

HistoTree.Print()
HistoFile.Write()

print "Finished Looping over events! Output Histograms in outputhistos.root"
        
        


