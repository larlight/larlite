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
my_proc.add_input_file(sys.argv[1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("gamma_pdf.root")

my_algo = sptool.SPAlgoGammaSelection()
my_algo.SetMode(True)
my_algo.Reset()
#my_algo.LoadParams()

my_ana = fmwk.ExampleSPSelection()
my_ana._mgr.SetSPAlgo(my_algo)

# Attach a template process
my_proc.add_process(my_ana)

print
print  "Finished configuring ana_processor. Start event loop!"
print
#sys.exit(1)
# Let's run it.
my_proc.run();

# done!
print
print "Finished running ana_processor event loop!"
print
my_algo.StoreParams()
sys.exit(0)

