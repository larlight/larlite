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
from ROOT import protoshower
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
  alg.SetDebug(False)

  angle3D = showerreco.Angle3DFromVtxQweighted()

  energy = showerreco.LinearEnergy()
  energy.SetUseModBox(True)
  energy.SetUseArea(True)
  energy.setVerbosity(False)
  energy.SetFillTree(True)
  energy.SetMinNumHits(20)

  dqdx = showerreco.dQdxModule()
  dqdx.setVerbosity(False)

  dedx = showerreco.dEdxFromdQdx()
  dedx.setVerbosity(False)
  
  alg.AddShowerRecoModule(showerreco.StartPoint3DModule()  )
  alg.AddShowerRecoModule(angle3D)
  filter = showerreco.Filter3DDir()
  filter.SetDotProdCut(0.5)
  alg.AddShowerRecoModule(filter)
  alg.AddShowerRecoModule(energy)
  alg.AddShowerRecoModule(dqdx)
  alg.AddShowerRecoModule(dedx)

  alg.AddShowerRecoModule(showerreco.GeoModule())

  alg.PrintModuleList()

  return alg

def DefaultShowerReco3D():

    # Create analysis unit
    ana_unit = fmwk.ShowerReco3D()

    # require PDG == 11 for PFParticles
    ana_unit.SetRequirePDG11(False)
    
    # Attach shower reco alg
    sralg = getShowerRecoAlgModular()
    # sralg.SetDebug()
    # sralg.Verbose(False)
    # sralg.Verbose(True)
    # sralg.SetUseArea(True)
    # Attach calo alg
    # calg = calo.CalorimetryAlg()
    # sralg.CaloAlgo(calg)
    #sralg.SetUseModBox(True)
    ana_unit.AddShowerAlgo(sralg)


    return ana_unit


# Create ana_processor instance
my_proc = fmwk.ana_processor()


# Set input root file
for x in range(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify analysis output root file name
my_proc.set_ana_output_file("showerRecoUboone_ana.root")
# Specify data output root file name
my_proc.set_output_file("showerreco3.root")



ana_unit=DefaultShowerReco3D()
# set ProtoShower Algo to go from data-products to a ProtoShower object
#protoshoweralg = protoshower.ProtoShowerAlgLoadVertex()
protoshoweralg = protoshower.ProtoShowerAlgOpenCV()
ana_unit.GetProtoShowerHelper().setProtoShowerAlg( protoshoweralg )
#ana_unit.SetInputProducer("fuzzyclustermerger")
ana_unit.SetInputProducer("ImageClusterHit")

ana_unit.SetOutputProducer("showerreco")

#my_proc.enable_event_alignment(False)
    
my_proc.add_process(ana_unit)

print()
print("Finished configuring ana_processor. Start event loop!")
print()

my_proc.run(0,50)
#my_proc.process_event(2)



sys.exit()

