import sys
from ROOT import gSystem,TMath
from larlite import larlite as fmwk
from larlite import larutil
from .cpp_classes import cmtool


def DefaultMatch():

    palgo_array = cmtool.CPAlgoArray()
    
    palgo1 = cmtool.CPAlgoNHits()
    palgo1.SetMinHits(25)
    
    palgo2 = cmtool.CPAlgoIgnoreTracks()
    
    palgo_array.AddAlgo(palgo1)
    palgo_array.AddAlgo(palgo2)

    algo_array = cmtool.CFAlgoArray()
    #algo_array.SetMode(cmtool.CFAlgoArray.kPositiveAddition)
    TOL = cmtool.CFAlgoTimeOverlap()
    TOL.RequireThreePlanes(False)
    #TOL.RequireThreePlanes(True)
    algo_array.AddAlgo(TOL)
    #algo_array.AddAlgo(cmtool.CFAlgoTimeProf())
    #algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
    #algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())

    return palgo_array, algo_array


def ChargeMatch():

    palgo_array = cmtool.CPAlgoArray()
    
    palgo1 = cmtool.CPAlgoNHits()
    palgo1.SetMinHits(25)
    
    palgo2 = cmtool.CPAlgoIgnoreTracks()
    
    palgo_array.AddAlgo(palgo1)
    palgo_array.AddAlgo(palgo2)

    algo_array = cmtool.CFAlgoArray()
    #algo_array.SetMode(cmtool.CFAlgoArray.kPositiveAddition)
    qratio = cmtool.CFAlgoQRatio()
    algo_array.AddAlgo(qratio)
    #TOL = cmtool.CFAlgoTimeOverlap()
    #TOL.RequireThreePlanes(False)
    #TOL.RequireThreePlanes(True)
    #algo_array.AddAlgo(TOL)
    #algo_array.AddAlgo(cmtool.CFAlgoTimeProf())
    #algo_array.AddAlgo(cmtool.CFAlgo3DAngle())
    #algo_array.AddAlgo(cmtool.CFAlgoStartPointMatch())

    return palgo_array, algo_array
