import os, sys

if len(sys.argv) != 5:
    print 
    print "TRKFILE  = str(sys.argv[1])"
    print "HITFILE  = str(sys.argv[2])"
    print "INFOFILE = str(sys.argv[3])"
    print "OUTDIR   = str(sys.argv[4])"
    print 
    sys.exit(1)

from larlite import larlite as fmwk

TRKFILE  = str(sys.argv[1])
HITFILE  = str(sys.argv[2])
INFOFILE = str(sys.argv[3])
OUTDIR   = str(sys.argv[4])

print "TRKFILE=",TRKFILE
print "HITFILE=",HITFILE
print "INFOFILE=",INFOFILE
print "OUTDIR=",OUTDIR

num = int(TRKFILE.split(".")[0].split("_")[-1])
OUTFILE = os.path.join(OUTDIR,"trackqualsingle_%d.root" % num)

tq_module = fmwk.TrackRecoAna()
tq_module.SetTrackVertexProducer("trackReco")
tq_module.SetTrunkLength(10)

my_proc = fmwk.ana_processor()
my_proc.add_input_file(TRKFILE)

if INFOFILE != "INVALID":
    tq_module.SetMCProducer("mcreco")
    my_proc.add_input_file(INFOFILE)

my_proc.set_io_mode(fmwk.storage_manager.kREAD)
my_proc.set_ana_output_file(OUTFILE)
my_proc.set_output_file("")

my_proc.add_process(tq_module)

my_proc.run()

sys.exit(0)
