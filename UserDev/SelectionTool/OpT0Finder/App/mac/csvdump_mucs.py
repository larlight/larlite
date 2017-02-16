import sys,ROOT,os

if len(sys.argv) < 3:
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
cfg_name = ''
for x in xrange(len(sys.argv)):
    if sys.argv[x].endswith('.fcl') or sys.argv[x].endswith('.cfg'):
        cfg_name = sys.argv[x]
    if not sys.argv[x].endswith('.root'):
        continue
    print sys.argv[x]
    my_proc.add_input_file(sys.argv[x])
if not cfg_name:
    print 'No config file given in input arguments'
    sys.exit(1)

# Specify IO mode
#my_proc.set_io_mode(fmwk.storage_manager.kREAD)
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)
my_proc.set_output_file("aho.root")

# Specify output root file name
my_proc.set_ana_output_file("ana.root")

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_unit = fmwk.MuCST0Finder()
my_proc.add_process(my_unit)

# TPC Filter Algo
#my_unit.Manager().SetAlgo(flashana.NPtFilter())
# PMT Filter Algo
#my_unit.Manager().SetAlgo(flashana.MaxNPEWindow())
# Match Prohibit Algo
#my_unit.Manager().SetAlgo(flashana.TimeCompatMatch())
# Hypothesis Algo
#my_unit.Manager().SetAlgo(flashana.PhotonLibHypothesis())
# Match Algo
#algo = flashana.QLLMatch.GetME()
#algo = flashana.QWeightPoint()
#algo = flashana.CommonAmps()
#my_unit.Manager().SetAlgo( algo )

# Custom Algo
#my_unit.Manager().SetAlgo( flashana.LightPath()  )

my_unit.SetConfigFile(cfg_name)

print
print  "Finished configuring ana_processor. Start event loop!"
print
fout_pe=open('pespec.txt','w')
line_pe=''
for x in xrange(32):
    line_pe += 'hypo%02d,' % x
for x in xrange(32):
    line_pe += 'flash%02d,' % x
line_pe=line_pe.rstrip(',')
line_pe+='\n'
fout_pe.write(line_pe)

fout=open('data.txt','w')
fout.write('entry,tpcid,xmin,xmax,time,x,t,q,oppe,hypope,mult\n')
ctr=0
while ctr < 500:
    my_proc.process_event(ctr)

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
        qc=None
        try:
            qc = qcluster_v[match.tpc_id]
        except IndexError:
            print
            print match.tpc_id,'/',qcluster_v.size()
            print
            raise IndexError
        for pt_idx in xrange(qc.size()):
            pt = qc[pt_idx]
            if pt.x > xmax: xmax = pt.x
            if pt.x < xmin: xmin = pt.x

        #print 'xmin:',xmin,'xmax:',xmax
        flash = flash_v[match_v[index].flash_id]
        line_pe=''
        
        hyposum=0
        for pe in match.hypothesis:
            hyposum += pe
            line_pe += '%g,' % pe

        pesum = 0
        for pe in flash.pe_v:
            pesum += pe
            line_pe += '%g,' % pe

        line_pe  = line_pe.rstrip(',')
        line_pe += '\n'

        fout_pe.write(line_pe)

        mult = 0
        for pe in flash.pe_v:
            if pe > 0: mult+=1
        fout.write('%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n' % (ctr,match.tpc_id,xmin,xmax,qc.time,match.tpc_point.x,flash.time,match.score,pesum,hyposum,mult))

    proc_status = my_proc.get_process_status()
    if proc_status == my_proc.kFINISHED:
        break

    ctr+=1
    sys.stdout.write('%d\r' % ctr)
    sys.stdout.flush()

    if 'debug' in sys.argv:
        cmd=sys.stdin.readline().rstrip('\n')
        if cmd.lower() in ['n','no']: break
# done!
print
print "Finished running ana_processor event loop!"
print
fout.close()
fout_pe.close()

my_proc.finalize()

sys.exit(0)
