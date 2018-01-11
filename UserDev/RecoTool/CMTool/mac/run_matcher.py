import sys,os
import ROOT
from larlite import larlite as ll
from ROOT import cluster, cmtool

mgr = ll.ana_processor()

#args should be input file name
for x in xrange(len(sys.argv)-2):

    mgr.add_input_file(sys.argv[x+1])

#last arg should be output file name
out_file = sys.argv[-1]


if os.path.isfile(out_file):
    print
    print 'ERROR: output file already exist...'
    print
    sys.exit(0)

mgr.set_output_file(out_file)

mgr.set_io_mode(ll.storage_manager.kBOTH)

mgr.set_ana_output_file("")

palgo = cmtool.CPAlgoNHits()
palgo.SetMinHits(10)

main_algo = cmtool.CFAlgoIoU()
main_algo.SetVerbose(False)
main_algo.SetMinIoU(0.25)

match_maker = ll.ClusterMatcher()

match_maker.GetManager().AddPriorityAlgo(palgo)
match_maker.GetManager().AddMatchAlgo(main_algo)

match_maker.SetClusterProducer("polar")
match_maker.SetVertexProducer("mcvertex")
match_maker.SetOutputProducer("iou")
match_maker.SaveOutputCluster(True)

mgr.set_data_to_write(ll.data.kPFParticle,"iou")
mgr.set_data_to_write(ll.data.kAssociation,"iou")

mgr.add_process(match_maker)

mgr.run()


