import os, sys
from larlite import larlite as fmwk

if len(sys.argv) != 4:
    print ""
    print "INFILE = str(sys.argv[1])"
    print "NUM    = str(sys.argv[2])"
    print "OUTDIR = str(sys.argv[3])"
    print ""
    sys.exit(1)

INFILE = str(sys.argv[1])
NUM    = str(sys.argv[2])
OUTDIR = str(sys.argv[3])

my_proc = fmwk.ana_processor()

if INFILE=="INVALID":
    sys.exit(0)

my_proc.add_input_file(INFILE)
my_proc.set_io_mode(fmwk.storage_manager.kREAD)
my_proc.set_ana_output_file(os.path.join(OUTDIR,"fluxrw_%s.root" % NUM))

my_mod = fmwk.LArLite_FluxRW()
my_proc.add_process(my_mod)
my_proc.run();

sys.exit(0)
