import sys
from larlite import larlite as fmwk
from ROOT import *

man=fmwk.storage_manager()
man.set_io_mode(man.kWRITE)
man.set_out_filename("aho.root")
man.set_verbosity(fmwk.msg.kDEBUG)
man.open()

run,subrun,event=(0,0,0)
for x in xrange(10):

    if x%5==0: run=x/5
    subrun = (x%5)/3
    event = x - run*5 - subrun*3
    
    for y in xrange(fmwk.data.kDATA_TYPE_MAX):

        if y in [fmwk.data.kUndefined,
                 fmwk.data.kSeed,
                 fmwk.data.kEvent,
                 fmwk.data.kMCTrajectory,
                 fmwk.data.kMCNeutrino,
                 fmwk.data.kDATA_TYPE_MAX]:
            continue
            
        man.get_data(y,"test")

    for y in xrange(fmwk.data.kSUBRUNDATA_TYPE_MAX):

        if y in [fmwk.data.kSUBRUNDATA_Undefined]:
            continue

        man.get_subrundata(y,"test")

    for y in xrange(fmwk.data.kRUNDATA_TYPE_MAX):

        if y in [fmwk.data.kRUNDATA_Undefined]:
            continue
        
        man.get_rundata(y,"test")

    man.set_id(run,subrun,event)
    man.next_event()

man.close()





