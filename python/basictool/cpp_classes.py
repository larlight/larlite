from colored_msg import error, warning, info
import ROOT

orig_level = ROOT.gErrorIgnoreLevel

libs = [ 'libBasicTool_LArPyBackend',
         'libBasicTool_GeoAlgo',
         'libBasicTool_EMShowerTools',
         'libBasicTool_FhiclLite']
         

ROOT.gErrorIgnoreLevel = ROOT.kFatal
for l in libs:
    val = ROOT.gSystem.Load(l)
    if val < 0:
        warning('Skipping %s' % l)
    else:
        info('%s %s' % (val,l))
ROOT.gErrorIgnoreLevel = orig_level

names=['geoalgo','larpy','fcllite']
for n in names:
    try:
        exec('ROOT.%s' % n)
        exec('from ROOT import %s' % n)
    except AttributeError:
        error('Namespace %s unavailable!' % n)
        raise
