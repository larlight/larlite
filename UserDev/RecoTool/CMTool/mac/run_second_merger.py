import ROOT,sys
from ROOT import *
from ROOT import larlite as fmwk

mgr = fmwk.ana_processor()

#arg1 should be input file name
mgr.add_input_file(sys.argv[1])

#arg2 should be output file name
mgr.set_output_file(sys.argv[2])

mgr.set_io_mode(fmwk.storage_manager.kBOTH)

mgr.set_ana_output_file("")

secondMerger = GetSecondMergerInstance()
# secondMerger = GetSecondMergerInstance(producer="mergedfuzzycluster",saveOutput=True)

mgr.add_process(secondMerger)

mgr.run()


