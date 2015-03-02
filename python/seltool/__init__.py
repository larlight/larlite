from colored_msg import info
ertool=None
ERViewer=None
try:
    from ROOT import gSystem
    gSystem.Load("libSelectionTool_ERBase.so")
    gSystem.Load("libSelectionTool_ERAlgo.so")
    gSystem.Load("libSelectionTool_ERFilter.so")
    gSystem.Load("libSelectionTool_ERApp.so")
    import erviewer
    ERViewer = erviewer.ERViewer
    ertool   = erviewer.ertool
    ertool.Manager
    from ROOT import ertool
except ImportError,NameError:
    info('Skipped ERTool (unavailable)')

