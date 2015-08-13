import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)


from ROOT import *

manager = larlite.storage_manager()

manager.reset()
manager.add_in_filename(sys.argv[1])
manager.set_io_mode(larlite.storage_manager.kREAD)
manager.open()

manager.next_event()

cluster = manager.get_data(larlite.data.kCluster,"mergeall")
print cluster
print cluster.size()

hits = manager.get_data(larlite.data.kHit,"gaushit")
print hits
print hits.size()

ass = manager.get_data(larlite.data.kAssociation,"mergall")
print ass
print ass.size()
