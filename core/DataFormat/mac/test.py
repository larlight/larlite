import sys
from ROOT import *
from ROOT import larlite as fmwk

man=fmwk.storage_manager()
man.set_io_mode(man.kWRITE)
man.set_out_filename("aho.root")
man.set_verbosity(fmwk.msg.kDEBUG)
man.open()

for x in xrange(10):

    for y in xrange(fmwk.data.kDATA_TYPE_MAX):


        if y in [fmwk.data.kSeed,
                 fmwk.data.kEvent,
                 fmwk.data.kMCTrajectory,
                 fmwk.data.kMCNeutrino,
                 fmwk.data.kDATA_TYPE_MAX]:
            continue
            
        
        print y
        man.get_data(y,"test")

    man.next_event()

man.close()





