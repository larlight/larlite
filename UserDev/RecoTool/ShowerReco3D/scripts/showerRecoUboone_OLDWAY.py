import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)


# Import the needed modules.  Need larlite and several of it's namespaces
from ROOT import gSystem,TMath
from larlite import larlite as fmwk
from larlite import larutil
from recotool import cmtool, showerreco
from ROOT import calo
# from recotool.showerDef import DefaultShowerReco3D


# Copied from the recotool/matchDef.py file.
# In general, use that one, but I reproduced it here for simplicity
def DefaultMatch():
  palgo_array = cmtool.CPAlgoArray()
  
  palgo1 = cmtool.CPAlgoNHits()
  palgo1.SetMinHits(25)
  
  palgo2 = cmtool.CPAlgoIgnoreTracks()
  
  palgo_array.AddAlgo(palgo1)
  palgo_array.AddAlgo(palgo2)

  algo_array = cmtool.CFAlgoArray()
  #algo_array.SetMode(cmtool.CFAlgoArray.kPositiveAddition)
  algo_array.AddAlgo(cmtool.CFAlgoTimeOverlap())
  #algo_array.AddAlgo(cmtool.CFAlgoTimeProf())
  #algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
  #algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())

  return palgo_array, algo_array


def DefaultShowerReco3D():

    # Create analysis unit
    ana_unit = fmwk.ShowerReco3D()
    
    # Attach shower reco alg
    sralg = showerreco.ShowerRecoAlg()
    # sralg.SetDebug()
    sralg.Verbose(False)
    # sralg.Verbose(True)
    sralg.SetUseArea(True)
    # Attach calo alg
    calg = calo.CalorimetryAlg()
    sralg.CaloAlgo(calg)
    sralg.SetUseModBox(True)
    ana_unit.AddShowerAlgo(sralg)

    # 
    # Attach Matching algorithm
    #
    palgo_array, algo_array = DefaultMatch()
    ana_unit.GetManager().AddPriorityAlgo(palgo_array)
    ana_unit.GetManager().AddMatchAlgo(algo_array)

    return ana_unit


# Create ana_processor instance
my_proc = fmwk.ana_processor()


# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify analysis output root file name
my_proc.set_ana_output_file("showersUboone_OLDWAY_Ana.root");

# Specify data output root file name
my_proc.set_output_file("showersUboone_OLDWAY.root")


ana_unit=DefaultShowerReco3D()
ana_unit.SetInputProducer("mergeall")

# Uboone specific:

ana_unit.SetOutputProducer("showerreco")

my_proc.add_process(ana_unit)

# Add an ana unit to do the shower quality:
quality_unit = fmwk.ShowerAna()

# quality_unit.SetShowerProducer("showerreco")

my_proc.add_process(quality_unit)

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run()
# my_proc.process_event(2)



sys.exit()

