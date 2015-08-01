import ROOT,sys,os
from mergeDefAhack import *
from ROOT import larlite as fmwk

mgr = fmwk.ana_processor()

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

mgr.set_io_mode(fmwk.storage_manager.kBOTH)

mgr.set_ana_output_file("")

producer = "cccluster"

print "At this stage, producer is: ",producer 
shortestDist = getShortestDist(producer)
shortestDist.SetInputProducer(producer)
shortestDist.SetOutputProducer('Step1ShortestDist')
shortestDist.SaveOutputCluster()
prevProducer = 'Step1ShortestDist'
mgr.add_process(shortestDist)
print "At this stage, producer is: ", prevProducer

startTrack = getStartTrack()
startTrack.SetInputProducer(prevProducer)
startTrack.SetOutputProducer('Step2StartTrack')
startTrack.SaveOutputCluster()
prevProducer = 'Step2StartTrack'
mgr.add_process(startTrack)
print "At this stage, producer is: ", prevProducer

polyContain = getPolyContain()
polyContain.SetInputProducer(prevProducer)
polyContain.SetOutputProducer('Step3PolyContain')
polyContain.SaveOutputCluster()
prevProducer = 'Step3PolyContain'
mgr.add_process(polyContain)
print "At this stage, producer is: ", prevProducer

com = getCOM()
com.SetInputProducer(prevProducer)
com.SetOutputProducer('Step4CenterOfMass')
com.SaveOutputCluster()
prevProducer = 'Step4CenterOfMass'
mgr.add_process(com)
print "At this stage, producer is: ", prevProducer

polyOverlap = getPolyOverlap()
polyOverlap.SetInputProducer(prevProducer)
polyOverlap.SetOutputProducer('Step5PolyOverlap')
polyOverlap.SaveOutputCluster()
prevProducer = 'Step5PolyOverlap'
mgr.add_process(polyOverlap)
print "At this stage, producer is: ", prevProducer

polyShortestDist = getPolyShortestDist()
polyShortestDist.SetInputProducer(prevProducer)
polyShortestDist.SetOutputProducer('Step6PolyShortDist')
polyShortestDist.SaveOutputCluster()
prevProducer = 'Step6PolyShortDist'
mgr.add_process(polyShortestDist)
print "At this stage, producer is: ", prevProducer




#prelim = GetPrelimMergerInstance()
#prelim.SetInputProducer(producer)
#prelim.SetOutputProducer("mergedCC1")
#second = GetSecondMergerInstance()
#second.SetInputProducer("mergedCC1")
#second.SetOutputProducer("mergedCC2")
#mgr.add_process(prelim)
#mgr.add_process(second)

#merge = GetDefaultMergerInstance()
#merge.SetInputProducer(producer)
#merge.SetOutputProducer("mergedCC2")
#mgr.add_process(merge)

#merge_all = GetMergeAllInstance()
#merge_all.SetOutputProducer("mergedfuzzy")
#mgr.set_data_to_write(fmwk.data.kCluster,"mergedCC1")
#mgr.set_data_to_write(fmwk.data.kAssociation,"mergedCC1")
#mgr.set_data_to_write(fmwk.data.kCluster,"mergedCC2")
#mgr.set_data_to_write(fmwk.data.kAssociation,"mergedCC2")
##mgr.add_process(merge_all)

mgr.run()


