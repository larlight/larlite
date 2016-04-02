import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root files (need simch, mcshower, reco cluster, reco shower)
#for x in xrange(len(sys.argv)):
#    if x == 0:
#        continue
#    my_proc.add_input_file(sys.argv[x])


for x in xrange(len(sys.argv)-3):
    my_proc.add_input_file(sys.argv[x+1])

#last arg should be the producer name
sProducerName = sys.argv[-1]
#second to last arg should be the identifier name
sIdentifier   = sys.argv[-2]


#last arg should be output file name
#sProducerName = sys.argv[-1]

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)


# Specify output root file name
sName = "MMQuality_ana_out_"
#sProducerName = "cccluster"
sExtention = ".root"
sOutputName = sName+sProducerName+"_"+sIdentifier+sExtention
my_proc.set_ana_output_file(sOutputName)


# Attach MMQuality process
mmq = fmwk.MMQuality()
#mmq.SetClusterProducer("mergedmergedfuzzycluster")
mmq.SetClusterProducer(sProducerName) #I need to change this!
#mmq.SetShowerProducer(sProducerName)
#mmq.SetClusterProducer("Step1ShortestDist") #I need to change this!

my_proc.add_process(mmq)

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)
