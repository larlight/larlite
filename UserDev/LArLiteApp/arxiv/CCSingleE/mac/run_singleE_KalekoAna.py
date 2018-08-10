import sys, os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import ertool
from seltool.ccsingleeDef import GetCCSingleEInstance
from cull_infiles import trim_filelist
#ertool.Manager

# open file on which to write values
#myfile = open("misid_vs_posRes.txt","a")
#posres = float(sys.argv[-1])
#print "Position resolution set to: ", posres
#myfile.write(str(posres)+"\t")

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Get Default CCSingleE Algorithm instance
# this sets default parameters
# this information is loaded from:
# $LARLITE_BASEDIR/python/seltool/GetCCSingleEInstance
# and the algorithm instance is the return of the
# function GetCCSingleEInstance()
my_algo = GetCCSingleEInstance()

# Create ERTool filter
# This filter removes any track that
# is less than 3 mm in length
# these tracks exist in "perfect reco"
# but at this stage it is unreasonable
# to assume we will be able to
# reconstruct them
my_filter = ertool.FilterTrackLength()
my_filter.setLengthCut(0.3)

# Create MC Filter
# This filter is if you want to look at CC1E events
MCfilter = fmwk.PerfectReco_1eNp0else_Filter()

# Set input root file
for ifile in trim_filelist(sys.argv):
    my_proc.add_input_file(ifile)

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("kaleko_singleE_ana_out.root")

# here set E-cut for Helper & Ana modules
#This cut is applied in helper... ertool showers are not made if the energy of mcshower or reco shower
#is below this threshold. This has to be above 0 or else the code may segfault. This is not a "physics cut".
#Do not change this value unless you know what you are doing.
Ecut = 20 # in MeV

my_ana = ertool.ERAnaSingleEKaleko()
my_ana.SetECut(Ecut)

my_anaunit = fmwk.ExampleERSelection()
my_anaunit._mgr.SetAlgo(my_algo)
my_anaunit._mgr.SetFilter(my_filter)
my_anaunit._mgr.SetAna(my_ana)
#This cut is applied in helper... ertool showers are not made if the energy of mcshower or reco shower
#is below this threshold. This has to be above 0 or else the code may segfault. This is not a "physics cut".
#Do not change this value unless you know what you are doing.
my_anaunit.SetMinEDep(Ecut)
my_anaunit._mgr._mc_for_ana = True

# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
#my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetShowerProducer(False,"showerreco3plane");
#my_anaunit.SetShowerProducer(False,"newdefaultreco");
#my_anaunit.SetShowerProducer(False,"pandoraNuShower");
#my_anaunit.SetShowerProducer(False,"mergeall");
my_anaunit.SetTrackProducer(True,"mcreco");
#my_anaunit.SetTrackProducer(False,"stitchkalmanhit");
#my_anaunit.SetVtxProducer(True,"generator");
# ************************************************

# Add MC filter and analysis unit
# to the process to be run
my_proc.add_process(MCfilter)
my_proc.add_process(my_anaunit)

my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

