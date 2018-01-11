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

merger_instance = ll.ClusterMerger()
merger_instance.SaveOutputCluster(True)

polar = cmtool.CBAlgoPolar()
polar.SetBufferAngle(0.0)
polar.SetVerbose(False)
polar.SetMergeTillConverge(True)

vtxalign = cmtool.CBAlgoVtxAlign()
vtxalign.SetUsePairWise(False)
vtxalign.SetVerbose(False)
vtxalign.SetMergeTillConverge(True)
vtxalign.SetMaxAngleDiff(12)
vtxalign.SetMinGammaOAngle(15)
vtxalign.SetMaxMergeDist(3)
vtxalign.SetMinNHits(10)

merger_instance.GetManager().AddMergeAlgo(polar)
merger_instance.GetManager().AddMergeAlgo(vtxalign)
merger_instance.GetManager().MergeTillConverge(False)

merger_instance.SetClusterProducer("sc2")
merger_instance.SetVertexProducer("mcvertex")
merger_instance.SetOutputProducer("polar")
mgr.add_process(merger_instance)

mgr.set_data_to_write(ll.data.kCluster,"polar")
mgr.set_data_to_write(ll.data.kAssociation,"polar")

mgr.run()


