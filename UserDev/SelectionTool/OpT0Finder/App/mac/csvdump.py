import sys,ROOT,os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

import ROOT
#ROOT.gSystem.Load("libOpFlashAna_OpT0FinderApp")
from larlite import larlite as fmwk
from ROOT import flashana

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    print sys.argv[x+1]
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("ana.root")

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_unit = fmwk.UBT0Finder()
#my_unit.SetEDiff(10.)
my_unit.UseMC(True)
my_unit.UseBNBCorrectnessWindow(False)
my_proc.add_process(my_unit)
#my_unit.ShiftFlashTime(-500)

# TPC Filter Algo
my_unit.Manager().SetAlgo(flashana.NPtFilter())
# PMT Filter Algo
my_unit.Manager().SetAlgo(flashana.MaxNPEWindow())
# Match Prohibit Algo
my_unit.Manager().SetAlgo(flashana.TimeCompatMatch())
# Hypothesis Algo
my_unit.Manager().SetAlgo(flashana.PhotonLibHypothesis())
# Match Algo
algo = flashana.QLLMatch.GetME()
#algo = flashana.QWeightPoint()
#algo = flashana.CommonAmps()
my_unit.Manager().SetAlgo( algo )

# Custom Algo
my_unit.Manager().SetAlgo( flashana.LightPath()  )
my_unit.Manager().SetAlgo( flashana.MCQCluster() )

#
# Other algorithms
#
#my_unit.Manager().AddCustomAlgo( flashana.LightPath() )

my_unit.Manager().Configure( "%s/SelectionTool/OpT0Finder/App/mac/flashmatch.fcl" % os.environ['LARLITE_USERDEVDIR'])

print
print  "Finished configuring ana_processor. Start event loop!"
print
fout=open('data.txt','w')
fout.write('xmin,xmax,x,q,oppe,hypope\n')
while 1:
    my_proc.process_event()

    match_v = my_unit.MatchResult()

    mgr = my_unit.Manager()

    qcluster_v = mgr.QClusterArray()

    flash_v = mgr.FlashArray()

    #print match_v.size(),'match found for',qcluster_v.size(),'tpc objects and',flash_v.size(),'pmt objects'

    for index in xrange(match_v.size()):
        #print 'Match id',index
        match = match_v[index]
        #print 'TPC:',match.tpc_id,'PMT:',match.flash_id,'Score:',match.score,'TPC point:',match.tpc_point.x,match.tpc_point.y,match.tpc_point.z

        xmin = 1e4
        xmax = 0
        try:
            qc = qcluster_v[match.tpc_id]
        except IndexError:
            print match.tpc_id,'/',qcluster_v.size()
            raise IndexError
        for pt_idx in xrange(qc.size()):
            pt = qc[pt_idx]
            if pt.x > xmax: xmax = pt.x
            if pt.x < xmin: xmin = pt.x

        #print 'xmin:',xmin,'xmax:',xmax
        flash = flash_v[match_v[0].flash_id]
        pesum = 0
        for pe in flash.pe_v: pesum += pe
        hyposum=0
        for pe in match.hypothesis: hyposum += pe
        fout.write('%g,%g,%g,%g,%g,%g\n' % (xmin,xmax,match.tpc_point.x,match.score,pesum,hyposum))

    proc_status = my_proc.get_process_status()
    if proc_status == my_proc.kFINISHED:
        break


# done!
print
print "Finished running ana_processor event loop!"
print
fout.close()
sys.exit(0)
