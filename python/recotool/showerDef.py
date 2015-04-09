import sys
from ROOT import gSystem,TMath
from larlite import larlite as fmwk
from larlite import larutil
from cpp_classes import cmtool, showerreco, calo
from recotool.matchDef import *

def PandoraShowerReco3D():
    # Create analysis unit
    ana_unit = fmwk.ShowerReco3D()
    # Attach shower reco alg
    sralg = showerreco.ShowerRecoAlg()
    sralg.Verbose(False)
    #sralg.Verbose(True)
    sralg.SetUseArea(True)
    # Attach calo alg
    calg = calo.CalorimetryAlg()
    sralg.CaloAlgo(calg)
    #sralg.SetUseModBox(True)
    ana_unit.SetShowerAlgo(sralg)

    return ana_unit

def DefaultShowerReco3D():
    # Create analysis unit
    ana_unit = fmwk.ShowerReco3D()
    
    # Attach shower reco alg
    sralg = showerreco.ShowerRecoAlg()
    sralg.Verbose(False)
    # sralg.Verbose(True)
    sralg.SetUseArea(True)
    # Attach calo alg
    calg = calo.CalorimetryAlg()
    sralg.CaloAlgo(calg)
    #sralg.SetUseModBox(True)
    ana_unit.SetShowerAlgo(sralg)

    # 
    # Attach Matching algorithm
    #
    palgo_array, algo_array = DefaultMatch()
    ana_unit.GetManager().AddPriorityAlgo(palgo_array)
    ana_unit.GetManager().AddMatchAlgo(algo_array)

    return ana_unit

