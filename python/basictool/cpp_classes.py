from colored_msg import error, warning, info
import ROOT

orig_level = ROOT.gErrorIgnoreLevel

libs = [ 'libBasicTool_GeoAlgo',
         'libBasicTool_EMShowerTools',
         'libBasicTool_LArPyBackend']

ROOT.gErrorIgnoreLevel = ROOT.kFatal
for l in libs:
    if ROOT.gSystem.Load(l) < 0:
        warning('Skipping %s' % l)
ROOT.gErrorIgnoreLevel = orig_level

names=['geoalgo','larpy']
for n in names:
    try:
        exec('ROOT.%s' % n)
        exec('from ROOT import %s' % n)
    except AttributeError:
        error('Namespace %s unavailable!' % n)
        raise
