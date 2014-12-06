from colored_msg import info

try:
    from geoviewer import GeoViewer
except ImportError,NameError:
    info('Skipped GeoViewer (unavailable)')

