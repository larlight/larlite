import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)


from ROOT import gSystem,TMath
from ROOT import larlite as fmwk, cmtool, showerreco,larutil


# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
my_proc.add_input_file(sys.argv[1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify input TDirectory name if given
if len(sys.argv) > 2:

    my_proc.set_input_rootdir(sys.argv[2])

# Specify analysis output root file name
my_proc.set_ana_output_file("from_test_ana_you_can_remove_me.root");

# Specify data output root file name
my_proc.set_output_file("out.root")

# Create analysis unit
ana_unit = fmwk.ShowerReco3D()

# Attach shower reco alg
ana_unit.SetShowerAlgo(showerreco.ShowerRecoAlg())

# Specify cluster type
ana_unit.SetClusterProducer("mergedfuzzy")

priority_algo = cmtool.CPAlgoNHits()
priority_algo.SetMinHits(20)
ana_unit.GetManager().AddPriorityAlgo(priority_algo)

# 
# Attach Matching algorithm
#

#Andrzej: The algorithms below are ranked by their effectiveness-- TimeOverlap is best, 
#then 3DAngle, then StartPoint . Right now, only TimeOverlap is called.
algo_array = cmtool.CFAlgoArray()
#algo_array.SetMode(cmtool.CFAlgoArray.kPositiveAddition)
algo_array.AddAlgo(cmtool.CFAlgoTimeOverlap())
#algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
#algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())

ana_unit.GetManager().AddMatchAlgo(algo_array)

my_proc.add_process(ana_unit)
my_proc.add_process(fmwk.MCShowerAna())

print
print  "Finished configuring ana_processor. Start event loop!"
print

fGSer = larutil.GeometryUtilities.GetME()

while my_proc.process_event():

    storage=ana_unit.GetCurrentData()
    print storage
    mctruth_v = storage.get_data(fmwk.data.kMCTruth,"generator")
    # Get event_mctruth ... std::vector<larlight::mctruth>
   # 

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
        lep_dcosx_truth = mctruth_v.at(0).GetParticles().at(0).Trajectory().at(0).Px()/mctruth_v.at(0).GetParticles().at(0).Trajectory().at(0).E();
        lep_dcosy_truth = mctruth_v.at(0).GetParticles().at(0).Trajectory().at(0).Py()/mctruth_v.at(0).GetParticles().at(0).Trajectory().at(0).E();
	lep_dcosz_truth = mctruth_v.at(0).GetParticles().at(0).Trajectory().at(0).Pz()/mctruth_v.at(0).GetParticles().at(0).Trajectory().at(0).E();
    
        print "-----  cx,cy,cz ",lep_dcosx_truth," ",lep_dcosy_truth," ",lep_dcosz_truth,"\n";
        
        fMCPhistart= 0.0 
        fMCThetastart=  0.0
        if (not(lep_dcosx_truth == 0.0 and lep_dcosz_truth == 0.0)) :
	  fMCPhistart=TMath.ATan2(lep_dcosx_truth,lep_dcosz_truth)
	if  (not(lep_dcosx_truth == 0.0 and lep_dcosy_truth == 0.0 and lep_dcosz_truth == 0.0)) :
	  fMCThetastart = TMath.Pi()*0.5-TMath.ATan2(TMath.Sqrt(lep_dcosx_truth*lep_dcosx_truth + lep_dcosz_truth*lep_dcosz_truth),lep_dcosy_truth)        
	print " MCPHI, MCTHETA ",fMCPhistart*180/3.1415," ",fMCThetastart*180/3.1415,"\n";
	print "MC Particle Start Point: (%g,%g,%g)" % (mct_vtx[0],mct_vtx[1],mct_vtx[2])
	print "MC Energy: ", mctruth_v.at(0).GetParticles().at(0).Trajectory().at(0).E()
   ##PdgCode
    print "in loop \n"
    sys.stdin.readline()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

