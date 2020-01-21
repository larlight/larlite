import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem,TMath
from ROOT import larlite as fmwk, cmtool, showerreco, larutil

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in range(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify input TDirectory name if given
#if len(sys.argv) > 2:

#    my_proc.set_input_rootdir(sys.argv[2])

# Specify analysis output root file name
my_proc.set_ana_output_file("");

# Specify data output root file name
my_proc.set_output_file("out.root")

#
# Kazu disables to read-in shower data product from input (only speeds up)
#
my_proc.set_rootdir('scanner')
# Create analysis unit
ana_unit = fmwk.ShowerReco3D()

# Attach shower reco alg
#sralg = showerreco.ShowerRecoAlg()
sralg = showerreco.Pi0ShowerRecoAlg()
sralg.Verbose(False)
ana_unit.SetShowerAlgo(sralg)

# Specify cluster type
ana_unit.SetClusterProducer("ncfilter")

# 
# Attach Matching algorithm
#
palgo_array = cmtool.CPAlgoArray()

palgo1 = cmtool.CPAlgoNHits()
palgo1.SetMinHits(25)

palgo2 = cmtool.CPAlgoIgnoreTracks()

palgo_array.AddAlgo(palgo1)
palgo_array.AddAlgo(palgo2)
ana_unit.GetManager().AddPriorityAlgo(palgo_array)

#Andrzej: The algorithms below are ranked by their effectiveness-- TimeOverlap is best, 
#then 3DAngle, then StartPoint . Right now, only TimeOverlap is called.
algo_array = cmtool.CFAlgoArray()
#algo_array.SetMode(cmtool.CFAlgoArray.kPositiveAddition)
#algo_array.AddAlgo(cmtool.CFAlgoTimeOverlap())
algo_array.AddAlgo(cmtool.CFAlgoTimeProf())
#algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
#algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())

ana_unit.GetManager().AddMatchAlgo(algo_array)

my_proc.add_process(ana_unit)

print()
print("Finished configuring ana_processor. Start event loop!")
print()

my_proc.run(0)

sys.exit(0)

