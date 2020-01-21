import sys
from ROOT import *
from ROOT import larlight as fmwk

processor = fmwk.ana_processor()

#need to use BOTH so output from first module is used as input to second
processor.set_io_mode(fmwk.storage_manager.BOTH)
#processor.set_io_mode(fmwk.storage_manager.READ)

processor.set_rootdir('scanner')

for x in range(len(sys.argv)):

    if x < 1: continue
    processor.add_input_file(sys.argv[x])

#need to save output file from shower module otherwise it won't work
processor.set_output_file("out.root")
processor.set_output_rootdir("scanner")

processor.set_ana_output_file("")



mod = fmwk.MCShowerClusterer()

#whether to group overlapping hits into single hit objects. TRUE by default
#reduces number of hits by a factor of ~10
mod.SetGroupOverlappingHits(False)

processor.add_process(mod)

processor.run()
