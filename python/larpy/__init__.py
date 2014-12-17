from colored_msg import info

try:
    from geoviewer import GeoViewer, geoalgo
except ImportError,NameError:
    info('Skipped GeoViewer (unavailable)')

