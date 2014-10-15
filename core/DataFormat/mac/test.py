import sys
from ROOT import *
#gSystem.Load("libDataFormat")
from ROOT import larlight as fmwk

man=fmwk.storage_manager()
man.set_io_mode(man.WRITE)
man.set_out_filename("aho.root")
man.set_verbosity(fmwk.MSG.DEBUG)
man.open()

for x in xrange(10):

    for y in xrange(fmwk.DATA.DATA_TYPE_MAX):


        if y in [fmwk.DATA.Seed,
                 fmwk.DATA.Event,
                 fmwk.DATA.MCTrajectory,
#                 fmwk.DATA.MCTruth,
                 fmwk.DATA.MCNeutrino,
                 fmwk.DATA.DATA_TYPE_MAX]:
            continue
            
        
        print y
        man.get_data(y)

    man.next_event()

man.close()





