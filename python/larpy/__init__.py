from colored_msg import info

try:
    from geoviewer import GeoViewer, geoalgo, larpy
except ImportError,NameError:
    info('Skipped GeoViewer (unavailable)')

