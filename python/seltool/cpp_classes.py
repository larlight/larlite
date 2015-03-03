from colored_msg import info,warning
import ROOT
libs=['libSelectionTool_ERApp',
      'libSelectionTool_ERAlgo',
      'libSelectionTool_ERFilter',
      'libSelectionTool_ERBase']
orig_level = ROOT.gErrorIgnoreLevel
ROOT.gErrorIgnoreLevel = ROOT.kFatal
try:
    from ROOT import gSystem
    for l in libs:
        val = gSystem.Load(l)
        if val < 0:
            warning('Skip loading %s' % l)
        #else:
        #    info('%s %s' % (val,l))
    from ROOT import ertool
except ImportError,NameError:
    warning('Skipped ERTool (unavailable)')

ROOT.gErrorIgnoreLevel = orig_level
