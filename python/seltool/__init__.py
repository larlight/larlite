from colored_msg import info

try:
    from erviewer import ERViewer, ertool
except ImportError,NameError:
    info('Skipped ERTool (unavailable)')

