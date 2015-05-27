# Load libraries
import ROOT, sys, os
from ROOT import *


# Define all of the steps to merging for ClusterCrawlerShower
# each function returns an instance of ClusterMerger()


def getSmallClustMerger(maxHitsProhib=5, maxHitsSmall=1, maxDist=0.5,maxDistAv=2.4, minHits = 1):
  merger = larlite.ClusterMerger()
  ########################################
  # PROHIBIT ALGORITHMS
  ########################################
  prohib_array = cmtool.CBAlgoArray()
  big_prohibit = cmtool.CBAlgoProhibitBigToBig()
  big_prohibit.SetMaxHits(maxHitsProhib)
  prohib_array.AddAlgo(big_prohibit,False)
  
  ########################################
  # MERGE ALGORITHMS
  ########################################
  algo_array = cmtool.CBAlgoArray()
  singleToBig = cmtool.CBAlgoMergeSingleToBig()
  singleToBig.SetMaxDistance(maxDist)
  singleToBig.SetMaxAverageDistance(maxDistAv)
  singleToBig.SetMaxSmallClustHits(maxHitsSmall)

  algo_array.AddAlgo(singleToBig)

  
  merger.GetManager().AddMergeAlgo(algo_array)
  merger.GetManager().AddSeparateAlgo(prohib_array)
  merger.GetManager().MergeTillConverge(False)
  merger.GetManager().SetMinNHits(minHits)
  return merger


def getSmallToTrackMerger(dist):
  merger = larlite.ClusterMerger()
  ########################################
  # PROHIBIT ALGORITHMS
  ########################################
  prohib_array = cmtool.CBAlgoArray()
  big_prohibit = cmtool.CBAlgoProhibitBigToBig()
  big_prohibit.SetMaxHits(15)
  prohib_array.AddAlgo(big_prohibit,False)

  # tracksep_prohibit = cmtool.CBAlgoTrackSeparate()
  # tracksep_prohibit.SetDebug(False)
  # tracksep_prohibit.SetVerbose(False)
  # tracksep_prohibit.SetMinAngleDiff(5)
  # tracksep_prohibit.SetUseEP(False)
  # prohib_array.AddAlgo(tracksep_prohibit,False)


  ########################################
  # MERGE ALGORITHMS
  ########################################
  algo_array = cmtool.CBAlgoArray()
  smallToTrack = cmtool.CBAlgoMergeSmallToTrack()
  smallToTrack.SetDebug(False)
  # Setters for track-like parameters
  # smallToTrack.SetMinHits()
  smallToTrack.SetMinModHitDens(0.8)
  # smallToTrack.SetMinMHitWires()
  # smallToTrack.SetMinPrincipal(10)
  # smallToTrack.SetMinCharge()
  # smallToTrack.SetMinLength()

  # Setter for small like parameters
  smallToTrack.SetMaxHit(8)
  # smallToTrack.SetMaxCharge()
  # smallToTrack.SetMaxLength()
  smallToTrack.SetMaxWidth(5)

  # Setter for merging parameters
  smallToTrack.SetMaxClosestDist(dist)
  # smallToTrack.SetMinDistToStart()
  # smallToTrack.SetMinDistToEnd()
  algo_array.AddAlgo(smallToTrack)

  merger.GetManager().AddMergeAlgo(algo_array)
  merger.GetManager().AddSeparateAlgo(prohib_array)
  return merger

def getInlineMerger(maxInlineDist=0.6,useAllHits=True,hitFraction=0.35):
  merger = larlite.ClusterMerger()
  ########################################
  # PROHIBIT ALGORITHMS
  ########################################
  # prohib_array = cmtool.CBAlgoArray()
  # big_prohibit = cmtool.CBAlgoProhibitBigToBig()
  # big_prohibit.SetMaxHits(maxHits)
  # prohib_array.AddAlgo(big_prohibit,False)
  # Want to add a prohibit function that stops if 
  # start to start point distance is too close

  # No prohibitions - this is meant to be very restrictive anyways


  ########################################
  # MERGE ALGORITHMS
  ########################################
  algo_array = cmtool.CBAlgoArray()
  inline = cmtool.CBAlgoMergeInline()
  inline.SetMaxAverageMinDistance(maxInlineDist)
  inline.SetUseAllHits(useAllHits)
  inline.SetHitFraction(hitFraction)
  algo_array.AddAlgo(inline)

  merger.GetManager().AddMergeAlgo(algo_array)
  # merger.GetManager().AddSeparateAlgo(prohib_array)
  merger.GetManager().MergeTillConverge(False)
  merger.GetManager().SetMinNHits(5)
  return merger

def getOverlapMerger(overlapFrac = 0.4, minHits = 10, maxHits = 50):
  merger = larlite.ClusterMerger()
  ########################################
  # PROHIBIT ALGORITHMS
  ########################################
  prohib_array = cmtool.CBAlgoArray()

  # Prohibit Merging Track to track:
  t2t_prohibit = cmtool.CBAlgoProhibitTrackToTrack()
  t2t_prohibit.SetMinHits(minHits)
  t2t_prohibit.SetMinMHitWiresFraction(0.05)
  t2t_prohibit.SetMinPrincipal(0.995)
  t2t_prohibit.SetMinLengthWidthRatio(10)
  t2t_prohibit.SetMode(cmtool.CBAlgoProhibitTrackToTrack.kEITHER)
  prohib_array.AddAlgo(t2t_prohibit, False)


  # big_prohibit = cmtool.CBAlgoProhibitBigToBig()
  # big_prohibit.SetMaxHits(maxHits)
  # prohib_array.AddAlgo(big_prohibit, False)

  # Want to add a prohibit function that stops if 
  # start to start point distance is too close
  s2s_prohibit = cmtool.CBAlgoProhibitStartToStart()
  s2s_prohibit.SetMinSeparation(1.0)
  prohib_array.AddAlgo(s2s_prohibit, False)


  ########################################
  # MERGE ALGORITHMS
  ########################################
  algo_array = cmtool.CBAlgoArray()
  polyOverlap = cmtool.CBAlgoPolyOverlap()
  polyOverlap.SetOverlapFraction(overlapFrac)
  polyOverlap.SetMinNumHits(minHits)
  algo_array.AddAlgo(polyOverlap)

  merger.GetManager().AddMergeAlgo(algo_array)
  merger.GetManager().AddSeparateAlgo(prohib_array)
  merger.GetManager().MergeTillConverge(False)
  merger.GetManager().SetMinNHits(minHits)
  return merger

def getShortestDistMerger(shortestDist,prohibitBig=False):
  merger = larlite.ClusterMerger()
  ########################################
  # PROHIBIT ALGORITHMS
  ########################################
  prohib_array = cmtool.CBAlgoArray()

  # Prohibit Merging Track to track:
  t2t_prohibit = cmtool.CBAlgoProhibitTrackToTrack()
  t2t_prohibit.SetMinHits(10)
  t2t_prohibit.SetMinMHitWiresFraction(0.5)
  t2t_prohibit.SetMinPrincipal(0.99)
  t2t_prohibit.SetMinLengthWidthRatio(10)
  t2t_prohibit.SetMode(cmtool.CBAlgoProhibitTrackToTrack.kEITHER)
  # t2t_prohibit.SetDebug(True)
  prohib_array.AddAlgo(t2t_prohibit, False)


  # if (prohibitBig):
  #   big_prohibit = cmtool.CBAlgoProhibitBigToBig()
  #   big_prohibit.SetMaxHits(15)
  #   prohib_array.AddAlgo(big_prohibit, False)

  # Want to add a prohibit function that stops if 
  # start to start point distance is too close
  s2s_prohibit = cmtool.CBAlgoProhibitStartToStart()
  s2s_prohibit.SetMinSeparation(1.2)
  # s2s_prohibit.SetDebug(True)
  prohib_array.AddAlgo(s2s_prohibit, False)


  ########################################
  # MERGE ALGORITHMS
  ########################################
  algo_array = cmtool.CBAlgoArray()

  SDAlg = cmtool.CBAlgoMergeShortestDistance()
  # SDAlg.SetDebug(True)
  SDAlg.SetMaxDistance(shortestDist)
  algo_array.AddAlgo(SDAlg)



  merger.GetManager().AddMergeAlgo(algo_array)
  merger.GetManager().AddSeparateAlgo(prohib_array)
  merger.GetManager().MergeTillConverge(False)
  merger.GetManager().SetMinNHits(5)
  return merger

def getWithinMerger():
  merger = larlite.ClusterMerger()
  ########################################
  # PROHIBIT ALGORITHMS
  ########################################
  prohib_array = cmtool.CBAlgoArray()

  t2t_prohibit = cmtool.CBAlgoProhibitTrackToTrack()
  t2t_prohibit.SetMinHits(10)
  t2t_prohibit.SetMinMHitWiresFraction(0.1)
  t2t_prohibit.SetMinPrincipal(0.99)
  t2t_prohibit.SetMinLengthWidthRatio(5)
  prohib_array.AddAlgo(t2t_prohibit, False)
  # prohib_array = cmtool.CBAlgoArray()
  # big_prohibit = cmtool.CBAlgoProhibitBigToBig()
  # big_prohibit.SetMaxHits(maxHits)
  # prohib_array.AddAlgo(big_prohibit,False)
  # Want to add a prohibit function that stops if 
  # start to start point distance is too close

  # No prohibitions - this is meant to be very restrictive anyways


  ########################################
  # MERGE ALGORITHMS
  ########################################
  algo_array = cmtool.CBAlgoArray()
  within = cmtool.CBAlgoMergeWithinBoundary()
  algo_array.AddAlgo(within)

  merger.GetManager().AddMergeAlgo(algo_array)
  # merger.GetManager().AddSeparateAlgo(prohib_array)
  merger.GetManager().MergeTillConverge(False)
  merger.GetManager().SetMinNHits(3)
  return merger

def getDiffuseMerger(maxInlineDist=0.6,hitFraction=0.35):
  merger = larlite.ClusterMerger()
  ########################################
  # PROHIBIT ALGORITHMS
  ########################################
  # prohib_array = cmtool.CBAlgoArray()
  # big_prohibit = cmtool.CBAlgoProhibitBigToBig()
  # big_prohibit.SetMaxHits(maxHits)
  # prohib_array.AddAlgo(big_prohibit,False)
  # Want to add a prohibit function that stops if 
  # start to start point distance is too close

  # No prohibitions - this is meant to be very restrictive anyways


  ########################################
  # MERGE ALGORITHMS
  ########################################
  algo_array = cmtool.CBAlgoArray()
  inline = cmtool.CBAlgoMergeDiffuse()
  inline.SetMaxAverageMinDistance(maxInlineDist)
  inline.SetHitFraction(hitFraction)
  algo_array.AddAlgo(inline)

  merger.GetManager().AddMergeAlgo(algo_array)
  # merger.GetManager().AddSeparateAlgo(prohib_array)
  merger.GetManager().MergeTillConverge(False)
  merger.GetManager().SetMinNHits(15)
  return merger



def getMergeToTrunk(shortestDist,prohibitBig=False):
  merger = larlite.ClusterMerger()
  ########################################
  # PROHIBIT ALGORITHMS
  ########################################
  prohib_array = cmtool.CBAlgoArray()

  # Prohibit Merging Track to track:
  t2t_prohibit = cmtool.CBAlgoProhibitTrackToTrack()
  t2t_prohibit.SetMinHits(10)
  t2t_prohibit.SetMinMHitWiresFraction(0.5)
  t2t_prohibit.SetMinPrincipal(0.99)
  t2t_prohibit.SetMinLengthWidthRatio(10)
  t2t_prohibit.SetMode(cmtool.CBAlgoProhibitTrackToTrack.kBOTH)
  # t2t_prohibit.SetDebug(True)
  prohib_array.AddAlgo(t2t_prohibit, False)


  if (prohibitBig):
    big_prohibit = cmtool.CBAlgoProhibitBigToBig()
    big_prohibit.SetMaxHits(10)
    prohib_array.AddAlgo(big_prohibit, False)

  # Want to add a prohibit function that stops if 
  # start to start point distance is too close
  s2s_prohibit = cmtool.CBAlgoProhibitStartToStart()
  s2s_prohibit.SetMinSeparation(1.0)
  # s2s_prohibit.SetDebug(True)
  prohib_array.AddAlgo(s2s_prohibit, False)


  ########################################
  # MERGE ALGORITHMS
  ########################################
  algo_array = cmtool.CBAlgoArray()

  SDAlg = cmtool.CBAlgoMergeShortestDistance()
  # SDAlg.SetDebug(True)
  SDAlg.SetMaxDistance(shortestDist)
  algo_array.AddAlgo(SDAlg)



  merger.GetManager().AddMergeAlgo(algo_array)
  merger.GetManager().AddSeparateAlgo(prohib_array)
  merger.GetManager().MergeTillConverge(False)
  merger.GetManager().SetMinNHits(5)
  return merger