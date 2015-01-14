import sys
from ROOT import gSystem,TMath
from ROOT import larlite as fmwk, cmtool, showerreco, larutil

def PandoraShowerReco3D():
    # Create analysis unit
    ana_unit = fmwk.ShowerReco3D()
    # Attach shower reco alg
    sralg = showerreco.ShowerRecoAlg()
    sralg.Verbose(False)
    #sralg.Verbose(True)
    sralg.SetUseArea(True)
    #sralg.SetUseModBox(True)
    ana_unit.SetShowerAlgo(sralg)

def ShowerReco3D():
    # Create analysis unit
    ana_unit = fmwk.ShowerReco3D()
    
    # Attach shower reco alg
    sralg = showerreco.ShowerRecoAlg()
    sralg.Verbose(False)
    # sralg.Verbose(True)
    sralg.SetUseArea(True)
    #sralg.SetUseModBox(True)
    ana_unit.SetShowerAlgo(sralg)

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
    algo_array.AddAlgo(cmtool.CFAlgoTimeOverlap())
    #algo_array.AddAlgo(cmtool.CFAlgoTimeProf())
    #algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
    #algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())

    ana_unit.GetManager().AddMatchAlgo(algo_array)

    return ana_unit



