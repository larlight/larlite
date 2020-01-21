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



def getShowerRecoAlgModular():
  # This function returns the default shower reco module
  # If you want to extend, customize, or otherwise alter the default
  # reco (which you should!) it's recommended that you do one of two
  # things:
  #   1) Copy this function, and change the lines you need
  #   2) Use this function as is and replace the modules with the built-in functions
  #       to ShowerRecoAlgModular
  # Then, use the return value (alg) in place of ShowerRecoAlg in your python script

  # It should be noted, however, that this "default" set of modules is not yet developed
  # and if you are developing you ought to be updating it here!

  alg = showerreco.ShowerRecoAlgModular()

  # 3D Axis Module:
  axis3D = showerreco.Axis3DModule()
  axis3D.setMaxIterations(50)
  axis3D.setNStepsMin(7)
  axis3D.setTargetError(0.00001)

  alg.AddShowerRecoModule(axis3D                           )
  alg.AddShowerRecoModule(showerreco.StartPoint2DModule()  )
  alg.AddShowerRecoModule(showerreco.StartPoint3DModule()  )
  alg.AddShowerRecoModule(showerreco.ShowerChargeModule()  )

  alg.PrintModuleList()

  return alg

# Copied from the recotool/matchDef.py file.
# In general, use that one, but I reproduced it here for simplicity
def DefaultMatchUboone():
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


# Unless you have the argoneut repository checked out and built,
# this matching algorithm won't even run.
# It's not exclusive, you can try it if you want at 
# https://github.com/coreyjadams/argoneut_electrons
# But it's not tuned for anything but argoneut
def DefaultMatchArgo():

    palgo_array = cmtool.CPAlgoArray()
    
    palgo1 = cmtool.CPAlgoNHits()
    palgo1.SetMinHits(60)
        
    palgo_array.AddAlgo(palgo1)

    algo_array = cmtool.CFAlgoArray()
    wireAlg    = cmtool.CFAlgoShowerWireMatch()
    timeAlg = cmtool.CFAlgoTimeOverlap()
    timeAlg.RequireThreePlanes(False)
    algo_array.AddAlgo(wireAlg)
    algo_array.AddAlgo(timeAlg)

    return palgo_array, algo_array


# This function creates the actual shower reconstruction object
def DefaultShowerReco3DArgo():
    # Create analysis unit
    ana_unit = fmwk.ShowerReco3D()
    larutil.LArUtilManager.Reconfigure(fmwk.geo.kArgoNeuT)
    
    # Attach shower reco alg
    sralg = getShowerRecoAlgModular()
    sralg.SetDebug()
    # sralg.Verbose(False)
    # sralg.Verbose(True)
    # sralg.SetUseArea(True)
    # Attach calo alg
    # calg = calo.CalorimetryAlg()
    # sralg.CaloAlgo(calg)
    #sralg.SetUseModBox(True)
    ana_unit.AddShowerAlgo(sralg)

    # 
    # Attach Matching algorithm
    #
    palgo_array, algo_array = DefaultMatchArgo()
    ana_unit.GetManager().AddPriorityAlgo(palgo_array)
    ana_unit.GetManager().AddMatchAlgo(algo_array)

    return ana_unit

def DefaultShowerReco3DUboone():

    # Create analysis unit
    ana_unit = fmwk.ShowerReco3D()
    
    # Attach shower reco alg
    sralg = getShowerRecoAlgModular()
    sralg.SetDebug()
    # sralg.Verbose(False)
    # sralg.Verbose(True)
    # sralg.SetUseArea(True)
    # Attach calo alg
    # calg = calo.CalorimetryAlg()
    # sralg.CaloAlgo(calg)
    #sralg.SetUseModBox(True)
    ana_unit.AddShowerAlgo(sralg)

    # 
    # Attach Matching algorithm
    #
    palgo_array, algo_array = DefaultMatchUboone()
    ana_unit.GetManager().AddPriorityAlgo(palgo_array)
    ana_unit.GetManager().AddMatchAlgo(algo_array)

    return ana_unit


# Create ana_processor instance
my_proc = fmwk.ana_processor()


# Set input root file
for x in range(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify analysis output root file name
my_proc.set_ana_output_file("elecShowersAna.root");

# Specify data output root file name
my_proc.set_output_file("elecShowers.root")


########################################################
# Change here to switch between microboone and argoneut!
########################################################

# Argoneut specific:
ana_unit=DefaultShowerReco3DArgo()
ana_unit.SetInputProducer("ccMergedFinal")

# Uboone specific:
# ana_unit = DefaultShowerReco3DUboone()
# ana_unit.SetInputProducer("mergedFuzzy")

########################################################

ana_unit.SetOutputProducer("showerreco")

my_proc.add_process(ana_unit)

# Add an ana unit to do the shower quality:
quality_unit = fmwk.MCShowerAna()

# quality_unit.SetShowerProducer("showerreco")

my_proc.add_process(quality_unit)

print()
print("Finished configuring ana_processor. Start event loop!")
print()

my_proc.run(0,50)
# my_proc.process_event(2)



sys.exit()

