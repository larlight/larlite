import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import sptool

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("pi0_pdf.root")

my_algo = sptool.SPAlgoPi0()
#my_algo.LoadParams()

my_algo.Reset()
my_ana = fmwk.ExampleSPSelection()
my_ana._mgr.SetSPAlgo(my_algo)
my_ana._mode =False
my_proc.add_process(my_ana)
my_proc.run()

my_algo.StoreParams()
sys.exit(0)

