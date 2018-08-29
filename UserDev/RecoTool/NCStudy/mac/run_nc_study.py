import sys, os

if len(sys.argv) != 3:
    print
    print "MCINFO = str(sys.argv[1])"
    print "NUM    = str(sys.argv[2])"
    print "OUTDIR = str(sys.argv[3])"
    print
    sys.exit(1)

MCINFO = str(sys.argv[1])
NUM    = str(sys.argv[2])
OUTDIR = str(sys.argv[3])

from larlite import larlite as fmwk

my_proc = fmwk.ana_processor()
my_proc.add_input_file(sys.argv[x+1])
my_proc.set_io_mode(fmwk.storage_manager.kREAD)
my_proc.set_ana_output_file(os.path.join(OUTDIR,"nc_dump_%s.root" % NUM));

ncdump = fmwk.nc_dump()
my_proc.add_process(ncdump)
my_proc.run();
sys.exit(0)
