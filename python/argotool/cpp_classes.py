from colored_msg import warning, error, info
import ROOT
orig_level = ROOT.gErrorIgnoreLevel
ROOT.gErrorIgnoreLevel = ROOT.kFatal

libs = ['/home/cadams/larlite/lib/libargoneut_electrons_analysis.so',
        '/home/cadams/larlite/lib/libargoneut_electrons_filter.so',
        '/home/cadams/larlite/lib/libargoneut_electrons_matching.so',
        '/home/cadams/larlite/lib/libargoneut_electrons_merging.so',
        '/home/cadams/larlite/lib/libargoneut_electrons_minos.so',
        '/home/cadams/larlite/lib/libargoneut_electrons_utils.so']


for l in libs:
    val = ROOT.gSystem.Load(l)
    if val < 0:
        warning('Skipping %s' % l)
    # else:
    #    info('%s %s' % (val,l))
ROOT.gErrorIgnoreLevel = orig_level

try:
    ROOT.argoutils
    ROOT.argomerge
    ROOT.argomatch
    ROOT.argofilter
except AttributeError:
    error('No LArLite core library loaded!')
    raise
from ROOT import argoutils, argomerge, argomatch, argofilter
