import sys
import binascii
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
parser.add_argument("-s","--source",nargs='*',help="Name of input file")
parser.add_argument("-o","--data-output",help="Output data file, if event is changed")
parser.add_argument("-a","--ana-output",help="Analysis output file")
parser.add_argument("-n","--num-events",help="Number of events to process")
parser.add_argument("-d","--display",help="Turn on the display to see each view before and after." )
parser.add_argument("-A","--argoneut",action='store_true',help="Set geometry to argonuet.")
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
    args.num_events = 99999

num_events = int(args.num_events)


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

chit=TCanvas("chit","chit",600,500)
chit.SetGridx(1)
chit.SetGridy(1)
algo = cluster.ClusterParamsExecutor()
algo.setNeuralNetPath("../../FANN/trained_nets/cascade_net.net")

if args.argoneut:
    algo.SetArgoneutGeometry()

#algo.SetUseHitBlurring(false);

processed_events=0

fGSer = larutil.GeometryUtilities.GetME()

while mgr.next_event():

   

    # Get event_mctruth ... std::vector<larlight::mctruth>
    mcshower_v = mgr.get_data(fmwk.DATA.MCShower)

    # Get event_cluster ... std::vector<larlight::cluster>
    #cluster_v = mgr.get_data(fmwk.DATA.FuzzyCluster)

    
  #  if cluster_v.event_id() != 280 or cluster_v.subrun() != 171 :
  # 	continue;
    
    # Get the primary particl generator vtx position
    mct_vtx=None
    mct_mom=None
    if mcshower_v and mcshower_v.size():
        if mcshower_v.size()>1:
            print "Found more than 1 MCTruth. Only use the 1st one... \n \n"
        if mcshower_v.at(0).MotherPDGID() == 11:      ## electron    
            mct_vtx = mcshower_v.at(0).MotherPosition()
            mct_mom = mcshower_v.at(0).DaughterMomentum()
            print "\n electron \n"
        elif mcshower_v.at(0).MotherPDGID() == 22:    
            mct_vtx = mcshower_v.at(0).MotherPosition()
            mct_mom = mcshower_v.at(0).DaughterMomentum()
            print "\n gamma \n"
        if mcshower_v.at(0).MotherPDGID() == 13:      ## muon    
            mct_vtx = mcshower_v.at(0).MotherPosition()
            mct_mom = mcshower_v.at(0).DaughterMomentum()
            print "\n muon \n"
    #PdgCode

    if num_events < processed_events:
        break
        
    for plane in xrange(larutil.Geometry.GetME().Nplanes()):

        if algo.LoadAllHits(mgr.get_data(larlight.DATA.GausHit), plane) == -1 :
            continue;


     #   if algo.LoadAllHits(mgr.get_data(larlight.DATA.FFTHit), plane) == -1 :
	#  continue;
    #for x in xrange(cluster_v.size()):

        

       # print "  Cluster ID:",cluster_v.at(x).ID()," Event ID ", 
       # print " return from", algo.LoadCluster(cluster_v.at(x),
        #                 mgr.get_data(cluster_v.get_hit_type()));
        #if algo.LoadCluster(cluster_v.at(x),
         #                mgr.get_data(cluster_v.get_hit_type())) == -1 :
	 #     continue;		     

        # algo.FillParams(True,True,True,True,True)
        algo.GetAverages(True)
        algo.GetRoughAxis(True)
        algo.GetProfileInfo(True)
        # algo.RefineStartPoints(True)
        # algo.RefineDirection(True)
        algo.RefineStartPointAndDirection()
        algo.FillPolygon()
        algo.GetFinalSlope(True)
        algo.TrackShowerSeparation(True)
        #algo.Report()
        #algo.PrintFANNVector()
        result = algo.GetParams()

        print "(%g,%g) => (%g,%g), plane: %s" % (result.start_point.w,
                                                 result.start_point.t,
                                                 result.end_point.w,
                                                 result.end_point.t,
                                                 result.start_point.plane)

        mc_begin=None
        mcangle=None
        if(mct_vtx):
            print "MC Particle Start Point: (%g,%g,%g)" % (mct_vtx[0],mct_vtx[1],mct_vtx[2])

            # Usage example 1: std::vector<double> in python
            # std.vector(ROOT.Double) is equivalent of std::vector<double> template specialization
            #my_vec=ROOT.std.vector(ROOT.Double)(3,0) 
            #my_vec[0]=mct_vtx[0]
            #my_vec[1]=mct_vtx[1]
            #my_vec[2]=mct_vtx[2]
            #mcpoint=fGSer.Get2DPointProjectionCM(my_vec,result.start_point.plane)

            # Usage example 2: double[3] C-array like object in python
            # 'd' specifies "double precision", 2nd argument specifies the array (length & values)
            my_vec = array('d',[0,0,0]) 
            my_vec[0] = mct_vtx[0]
            my_vec[1] = mct_vtx[1]
            my_vec[2] = mct_vtx[2]
            mcpoint=fGSer.Get2DPointProjectionCM(my_vec,
                                                 int(binascii.b2a_hex(result.start_point.plane)))

            # Example 1 & 2 should have the same return here (checked)
            # print " Start point in w,t  (%g,%g)" % (mcpoint.w,mcpoint.t)   

            mc_begin = TGraph(2)
            mc_begin.SetPoint(0, mcpoint.w, mcpoint.t)
            mc_begin.SetMarkerStyle(29)
            mc_begin.SetMarkerColor(ROOT.kRed)
            mc_begin.SetLineWidth(1)
            mc_begin.SetLineColor(ROOT.kRed)
            mc_begin.SetMarkerSize(3)

            mct_mom = TVector3(mct_mom[0]/sqrt(pow(mct_mom[0],2) + pow(mct_mom[1],2) + pow(mct_mom[2],2)),
                               mct_mom[1]/sqrt(pow(mct_mom[0],2) + pow(mct_mom[1],2) + pow(mct_mom[2],2)),
                               mct_mom[2]/sqrt(pow(mct_mom[0],2) + pow(mct_mom[1],2) + pow(mct_mom[2],2)))

            mc_angle = fGSer.Get2DangleFrom3D(int(binascii.b2a_hex(result.start_point.plane)), mct_mom)
            print 
            print 'mom:',mct_mom[0],mct_mom[1],mct_mom[2]
            print 'plane:', int(binascii.b2a_hex(result.start_point.plane))
            print '2D angle: MC = %g ... Reco = %g' % (mc_angle / TMath.Pi() * 180, result.angle_2d)
            print '2D angle diff:', mc_angle / TMath.Pi() * 180 - result.angle_2d
            print
            mc_begin.SetPoint(1, 9999. * TMath.Cos(mc_angle), 9999. * TMath.Sin(mc_angle))

        if mc_begin:
            dist = sqrt( pow(mcpoint.w - result.start_point.w,2) + pow(mcpoint.t - result.start_point.t,2))
            print "Distant:",dist
            #if dist > 10: continue

        #Add black star to mark begin point and black square to mark end point
        begin = TGraph(1)
        end = TGraph(1)
        begin.SetPoint(0,result.start_point.w, result.start_point.t)
        end.SetPoint(0,result.end_point.w, result.end_point.t)
        begin.SetLineWidth(0)
        end.SetLineWidth(0)
        # Set Start & End point TGraph
        begin.SetLineColor(1)
        begin.SetMarkerStyle(29)
        begin.SetMarkerColor(ROOT.kBlack)
        begin.SetMarkerSize(3)
        end.SetLineColor(1)
        end.SetMarkerStyle(21)
        end.SetMarkerColor(ROOT.kBlack)
        end.SetMarkerSize(2)

        func=TF1("nf","[1]*x+[0]",result.start_point.w-50,result.end_point.w+50);
        func.SetParameter(0,algo.RoughIntercept());     
        func.SetParameter(1,algo.RoughSlope());   
        func.SetLineWidth(1);   
        func.SetLineColor(kBlue)

        if result.start_point.w > result.end_point.w:
            func.SetRange(result.end_point.w-50,result.start_point.w+50);
            
        dwire=30;
        dtime=30*TMath.Tan(result.angle_2d*TMath.Pi()/180);
        if result.angle_2d > -90 and  result.angle_2d <0 :
            dwire=30;
            dtime=30*TMath.Tan(result.angle_2d*TMath.Pi()/180);
        elif result.angle_2d < -90. and  result.angle_2d > -180 :
            dwire=-30;
            dtime=30*TMath.Tan(-1*result.angle_2d*TMath.Pi()/180); 
        elif result.angle_2d > 90. and  result.angle_2d < 180 :
            dwire=-30;
            dtime=30*TMath.Tan(-1*result.angle_2d*TMath.Pi()/180);     
         
        angleline=TLine(result.start_point.w,result.start_point.t,result.start_point.w+dwire,result.start_point.t+dtime);
        angleline.SetLineColor(kBlack);
        angleline.SetLineWidth(3);
         
        # print "testing slopes: %g   from angle: %g" % ( algo.RoughSlope(),TMath.Tan(result.angle_2d*TMath.Pi()/180));
        # Set Polygon
        gPolygon = None
        if result.PolyObject.Size() > 0:
            gPolygon = TGraph(result.PolyObject.Size() + 1)
            for x in xrange(result.PolyObject.Size()):
                gPolygon.SetPoint(x,
                                  result.PolyObject.Point(x).first,
                                  result.PolyObject.Point(x).second)

            gPolygon.SetPoint(result.PolyObject.Size(),
                              result.PolyObject.Point(0).first,
                              result.PolyObject.Point(0).second)

            gPolygon.SetMarkerStyle(20)
            gPolygon.SetMarkerSize(1)
            gPolygon.SetMarkerColor(kBlue)

        # Draw Hit 2D View & points
        chit.cd()
        chit.SetLogz()
        hHits = algo.GetHitView()
        hHits.Draw("COLZ")
        begin.Draw("P same")
        end.Draw("P same")
        func.Draw("same");
        angleline.Draw("same");

        leg = TLegend(.7,.7,.95,.95)
        leg.AddEntry(begin, "Start Point (reco)","P")
        leg.AddEntry(end, "End Point (reco)","P")
        leg.AddEntry(angleline, "2D Angle")
        

        if(mc_begin):
            leg.AddEntry(mc_begin, "Start Point (mc)","P")
            mc_begin.Draw("PL same")
        #if(gPolygon):
        #    gPolygon.Draw("PL same")
        leg.Draw("same")
        # Update canvas
        chit.Update()
        sys.stdin.readline()
        processed_events+=1
        print "Finished event %g of %s" % (processed_events, num_events)
        
        


