import sys
from ROOT import gSystem,TMath
from larlite import larlite as fmwk
from larlite import larutil
from cpp_classes import cmtool


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
