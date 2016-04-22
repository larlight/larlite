import sys
from array import array
from ROOT import *
import ROOT
gSystem.Load("libBase")
gSystem.Load("libLArUtil")
gSystem.Load("libClusterRecoUtil")
from ROOT import larlight as fmwk
from ROOT import cluster


import argparse

parser = argparse.ArgumentParser(description="Python script to process and merge showers.")
group = parser.add_mutually_exclusive_group()
group.add_argument("-v", "--verbose", help="Turn on verbose output",
                    action="store_true")
group.add_argument("-q", "--quiet", help="Turn off most output",
                    action="store_true")
parser.add_argument("-s","--source",help="Name of input file")
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

mgr.add_in_filename(args.source)
if len(sys.argv) > 2:
    mgr.set_in_rootdir("scanner")

mgr.open()

chit=TCanvas("chit","chit",600,500)
chit.SetGridx(1)
chit.SetGridy(1)
algo = cluster.ClusterParamsExecutor()

algo.SetUseHitBlurring(false);

processed_events=0

fGSer = larutil.GeometryUtilities.GetME()
fGeo  = larutil.Geometry.GetME()

while mgr.next_event():

    # Get event_cluster ... std::vector<larlight::cluster>
    cluster_v = mgr.get_data(fmwk.DATA.ShowerAngleCluster)

    # Get event_mctruth ... std::vector<larlight::mctruth>
    mctruth_v = mgr.get_data(fmwk.DATA.MCTruth)

    # Get event_hit     ... std::vector<larlight::hit>
    hits_v    = mgr.get_data(fmwk.DATA.GausHit)

    print mctruth_v
    if not hits_v:
        print "No Hits! \n"
        continue

    # Get the primary particl generator vtx position
    mct_vtx=None
    if mctruth_v and mctruth_v.size():
        if mctruth_v.size()>1:
            print "Found more than 1 MCTruth. Only use the 1st one... \n \n"
        if mctruth_v.at(0).GetParticles().at(0).PdgCode() == 11:      ## electron    
	    mct_vtx = mctruth_v.at(0).GetParticles().at(0).Trajectory().at(0).Position()
	    print "\n electron \n"
	elif mctruth_v.at(0).GetParticles().at(0).PdgCode() == 22:    
	    trajsize= mctruth_v.at(0).GetParticles().at(0).Trajectory().size()
	    mct_vtx = mctruth_v.at(0).GetParticles().at(0).Trajectory().at(trajsize-1).Position()
	    print "\n gamma \n"
   #PdgCode

    if args.num_events == processed_events:
        exit()
        
    if not cluster_v:
        continue

    print "Event:",cluster_v.event_id()


    #Loop over all planes
    
    for pl in range( fGeo.Nviews() ):
        print "View: ",pl
        cluster_counter = 0
        #Get all hit coordinates and make TH2D of them
        hit_wires  = []
        hit_times  = []
        hit_charge = []
        wire_min = 10000
        wire_max = 0
        time_min = 10000
        time_max = 0
        polygons = []
        
        print "Number of Clusters in Event: ",cluster_v.size()
        for x in xrange(cluster_v.size()):
            cluster_counter += 1
            
            if  ( cluster_v.at(x).View() == fGeo.PlaneToView(pl) ):

                print "  Cluster ID:",cluster_v.at(x).ID()
                algo.LoadCluster(cluster_v.at(x),
                                 mgr.get_data(cluster_v.get_hit_type()))
                
                # algo.FillParams(True,True,True,True,True)
                algo.GetAverages(True)
                algo.GetRoughAxis(True)
                algo.GetProfileInfo(True)
                algo.RefineStartPoints(True)
                algo.RefineDirection(True)
                algo.FillPolygon()
                
                # algo.GetFinalSlope(True)
                #algo.Report()
        
                result = algo.GetParams()
                
                
                for u in range ( (mgr.get_data(cluster_v.get_hit_type())).size() ):
                    hit = mgr.get_data(cluster_v.get_hit_type()).at(u)
                    if (hit.View() == fGeo.PlaneToView(pl) ):
                        hit_wires.append(hit.Wire());
                        hit_times.append(hit.PeakTime());
                        hit_charge.append(hit.Charge());
                        if ( hit.Wire() < wire_min ):
                            wire_min = hit.Wire()
                        if ( hit.Wire() > wire_max ):
                            wire_max = hit.Wire()
                        if ( hit.PeakTime() < time_min ):
                            time_min = hit.PeakTime()
                        if ( hit.PeakTime() > time_max ):
                            time_max = hit.PeakTime()


                print "Wire Min: ", wire_min
                print "Wire Max: ", wire_max

                #print "(%g,%g) => (%g,%g), plane: %g" % (result.start_point.w,
                #                                         result.start_point.t,
                #                                         result.end_point.w,
                #                                         result.end_point.t,result.start_point.plane)
                    
                mc_begin=None
                if(mct_vtx):
                #    print "MC Particle Start Point: (%g,%g,%g)" % (mct_vtx[0],mct_vtx[1],mct_vtx[2])

                        
                    my_vec = array('d',[0,0,0]) 
                    my_vec[0] = mct_vtx[0]
                    my_vec[1] = mct_vtx[1]
                    my_vec[2] = mct_vtx[2]
                    mcpoint=fGSer.Get2DPointProjectionCM(my_vec,result.start_point.plane)
                    
                    mc_begin = TGraph(1)
                    mc_begin.SetPoint(0,mcpoint.w,mcpoint.t)
                    mc_begin.SetMarkerStyle(29)
                    mc_begin.SetMarkerColor(ROOT.kRed)
                    mc_begin.SetMarkerSize(3)
                    begin = TGraph(1)
                    end = TGraph(1)
                    begin.SetPoint(0,result.start_point.w, result.start_point.t)
                    end.SetPoint(0,result.end_point.w, result.end_point.t)
                    
                    begin.SetLineColor(1)
                    begin.SetMarkerStyle(29)
                    begin.SetMarkerColor(ROOT.kBlack)
                    begin.SetMarkerSize(3)
                    end.SetLineColor(1)
                    end.SetMarkerStyle(21)
                    end.SetMarkerColor(ROOT.kBlack)
                    end.SetMarkerSize(2)
                    
                    
                    gPolygon = None
                    if result.container_polygon.size() > 0:
                        gPolygon = TGraph(result.container_polygon.size() + 1)
                        for x in xrange(result.container_polygon.size()):
                            gPolygon.SetPoint(x,
                                              result.container_polygon.at(x).w,
                                              result.container_polygon.at(x).t)
                            print result.container_polygon.at(x).w, result.container_polygon.at(x).t
                            
                        gPolygon.SetPoint(result.container_polygon.size(),
                                          result.container_polygon.at(0).w,
                                          result.container_polygon.at(0).t)
                        
                        gPolygon.SetMarkerStyle(20)
                        gPolygon.SetMarkerSize(1)
                        gPolygon.SetMarkerColor(kRed)
                    polygons.append(gPolygon)

        hHits=ROOT.TH2D("AllHits","Hits and Polygons; Wire [cm]; Time [cm]",
                        100, 0.9*wire_min*fGSer.WireToCm(), 1.1*wire_max*fGSer.WireToCm(),
                        100, 0.9*time_min*fGSer.TimeToCm(), 1.1*time_max*fGSer.TimeToCm())
        print "Length of Hit array: {0}".format(len(hit_wires))
        for x in range(len(hit_wires)):
            hHits.Fill( hit_wires[x]*fGSer.WireToCm(), hit_times[x]*fGSer.TimeToCm(), hit_charge[x])
        chit.cd()
        hHits.Draw()
        for y in polygons:
            y.Draw("PL same")
        chit.Update()
        sys.stdin.readline()
        hHits.Delete()
        
        


