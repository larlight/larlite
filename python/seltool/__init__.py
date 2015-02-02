from colored_msg import info
ertool=None
ERViewer=None
try:
    from ROOT import gSystem
    gSystem.Load("libSelectionTool_ERBase.so")
    import erviewer
    ERViewer = erviewer.ERViewer
    ertool   = erviewer.ertool
    ertool.Manager
except ImportError,NameError:
    info('Skipped ERTool (unavailable)')

