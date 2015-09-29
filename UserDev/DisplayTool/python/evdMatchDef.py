
from ROOT import cmtool, larutil

def getMatchManager():


  nplanes = larutil.Geometry.GetME().Nplanes()
  manager = cmtool.CMatchManager(nplanes)


  priority_array = cmtool.CPAlgoArray()
  palgo1 = cmtool.CPAlgoNHits()
  palgo1.SetMinHits(20)
  priority_array.AddAlgo(palgo1)

  match_array = cmtool.CFAlgoArray()
  timeAlg     = cmtool.CFAlgoTimeOverlap()
  timeAlg.RequireThreePlanes(False)
  match_array.AddAlgo(timeAlg)

  manager.AddPriorityAlgo(priority_array)
  manager.AddMatchAlgo(match_array)

  return manager

