import sys,os

# Import the needed modules.  Need larlite and several of it's namespaces
from ROOT import gSystem,TMath
from larlite import larlite as fmwk
from recotool import showerreco
from ROOT import protoshower

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
    alg.SetDebug(True)
    alg.SetVerbose(False)

    # filteralgo = showerreco.FilterPFPart()
    # filteralgo.setMinNHitsAbsolute(5)
    # filteralgo.setMinNHitsLargest(10)
    
    # angle3D = showerreco.Angle3DFormula()
    # angle3D.setMaxAngleError(0.1)
    # angle3D.setValidateDirection(True)
    # angle3D.setVerbosity(True)

    angle3D = showerreco.Angle3DFromVtxQweighted()
    angle3D.setVerbosity(True)
    
    startPt = showerreco.YPlaneStartPoint3D()
    startPt.setVerbosity(True)

    energy = showerreco.LinearEnergy()
    energy.setVerbosity(True)

    # implement position-dependent calibration
    energy.CreateResponseMap(20)
    dQdsAVG = 248.
    #fin = open('/a/share/westside/dcaratelli/ll_shower/UserDev/RecoTool/ShowerReco3D/dqds_mc_xyz.txt','r')
    fin = open('/home/vgenty/sw/larlite/UserDev/RecoTool/ShowerReco3D/dqds_mc_xyz.txt','r')
    for line in fin:
        words = line.split()
        x = float(words[0])
        y = float(words[1])
        z = float(words[2])
        q = float(words[3])
        energy.SetResponse(x,y,z,dQdsAVG/q)

    energy.SetElectronLifetime(1e6) # in us DATA value
    energy.SetRecombFactor(0.62)
    #energy.SetElecGain(243.) # MCC8.0 data
    energy.SetElecGain(200.) # MCC8.0 value
    energy.setVerbosity(False)
    energy.SetFillTree(True)

    dqdx = showerreco.dQdxModule()
    dqdx.setTrunkLength(3.)
    # dqdx.SetFillTree(True)
    
    # shrFilter = showerreco.FilterShowers()
    # shrFilter.setAngleCut(15.)
    # shrFilter.setVerbosity(False)

    # alg.AddShowerRecoModule( filteralgo )
    alg.AddShowerRecoModule(angle3D)
    #alg.AddShowerRecoModule( showerreco.StartPoint3DModule() )
    #alg.AddShowerRecoModule( showerreco.NearestStartPoint3D() )
    alg.AddShowerRecoModule( startPt )
    alg.AddShowerRecoModule( dqdx )
    alg.AddShowerRecoModule(energy)
    alg.AddShowerRecoModule( showerreco.FillLength() )
    #alg.AddShowerRecoModule( shrFilter )
    
    alg.PrintModuleList()
    
    return alg

def DefaultShowerReco3D():
    
    # Create analysis unit
    ana_unit = fmwk.ShowerReco3D()
    
    # require PDG == 11 for PFParticles
    ana_unit.SetRequirePDG11(True)
    
    # Attach shower reco alg
    sralg = getShowerRecoAlgModular()
    # sralg.SetDebug()
    # sralg.Verbose(False)
    # sralg.Verbose(True)
    # sralg.SetUseArea(True)

    # Attach calo alg
    # from ROOT import calo
    # calg = calo.CalorimetryAlg()
    # sralg.CaloAlgo(calg)
    #sralg.SetUseModBox(True)
    ana_unit.AddShowerAlgo(sralg)

    return ana_unit


# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Suffix for output
suffix = str(sys.argv[1])

# Set input root file
my_proc.add_input_file(sys.argv[2])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

shower_ana_unit=DefaultShowerReco3D()
print "Load DefaultShowerReco3D @ ",shower_ana_unit

# Specify data output root file name
my_proc.set_output_file("out/out_out_%s.root"%suffix)
my_proc.set_ana_output_file("ana/ana_out_%s.root" % suffix)

# set ProtoShower Algo to go from data-products to a ProtoShower object
protoshoweralg = protoshower.ProtoShowerAlgDL()
protoshoweralg.SetVertexProducer("dl")
shower_ana_unit.GetProtoShowerHelper().setProtoShowerAlg( protoshoweralg )

shower_ana_unit.SetInputProducer("dl")
shower_ana_unit.SetOutputProducer("showerreco")

my_proc.add_process(shower_ana_unit)

my_proc.set_data_to_write(fmwk.data.kShower,       "showerreco")
my_proc.set_data_to_write(fmwk.data.kAssociation,  "showerreco")

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run()

sys.exit()

