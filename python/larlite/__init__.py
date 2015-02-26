from colored_msg import warning, error
import ROOT
orig_level = ROOT.gErrorIgnoreLevel
ROOT.gErrorIgnoreLevel = ROOT.kFatal

libs = [ 'libLArLite_Analysis',
         'libLArLite_DataFormat',
         'libLArLite_LArUtil',
         'libLArLite_Base' ]

for l in libs:
    if ROOT.gSystem.Load(l) < 0:
        warning('Skipping %s' % l)

ROOT.gErrorIgnoreLevel = orig_level

try:
    ROOT.larlite
except AttributeError:
    error('No LArLite core library loaded!')
    raise

from ROOT import larlite


