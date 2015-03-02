from colored_msg import info
from cpp_classes import * 

try:
    from geoviewer import GeoViewer
except ImportError,NameError:
    info('Skipped BasicTool (unavailable)')

