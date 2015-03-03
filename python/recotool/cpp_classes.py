from colored_msg import warning, error, info
import ROOT
orig_level = ROOT.gErrorIgnoreLevel
ROOT.gErrorIgnoreLevel = ROOT.kFatal

libs = [ 'libRecoTool_ShowerReco3D.so',
         'libRecoTool_CMToolAna.so',
         'libRecoTool_CMToolApp.so',
         'libRecoTool_CMTAlgMatch.so',
         'libRecoTool_CMTAlgMerge.so',
         'libRecoTool_CMTAlgPriority.so',
         'libRecoTool_CMToolBase.so',
         'libRecoTool_NCfilter.so',
         'libRecoTool_ClusterRecoUtil.so',
         'libRecoTool_FANN.so',
         'libRecoTool_MCComp.so',
         'libRecoTool_MCShowerReco.so',
         'libRecoTool_AnalysisAlg.so' ]         
         

for l in libs:
    val = ROOT.gSystem.Load(l)
    if val < 0:
        warning('Skipping %s' % l)
    #else:
    #    info('%s %s' % (val,l))
ROOT.gErrorIgnoreLevel = orig_level

try:
    ROOT.cluster
    ROOT.cmtool
    ROOT.showerreco
except AttributeError:
    error('No LArLite core library loaded!')
    raise
from ROOT import cluster, cmtool, showerreco


