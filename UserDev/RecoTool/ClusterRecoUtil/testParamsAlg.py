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

clusts = manager.get_data(larlite.data.kCluster,"mergeall")
# print clusts
# print clusts.size()

hits = manager.get_data(larlite.data.kHit,"gaushit")
# print hits.size()

ass = manager.get_data(larlite.data.kAssociation,"mergeall")
# print ass
# print ass.size()

hit_index_v = ass.association(clusts.id(), hits.id())
# print hit_index_v

# Make the hits into PxHits and pass to the clusterParamsAlg



defaultAlg = cluster.DefaultParamsAlg()
defaultAlg.SetDebug(True)


params = cluster.cluster_params()
helper = cluster.CRUHelper()
helper.GenerateParams(hit_index_v[0],hits,params)

# alg.FillParams(params)
defaultAlg.FillParams(params)
