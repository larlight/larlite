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
    fname = sys.argv[x+1]
    if fname.find('gamma')>=0:
        my_proc.add_input_file(fname)

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("gamma_pdf.root")

my_algo = sptool.SPAlgoEMPart()
my_algo.SetMode(True)
#my_algo.LoadParams()

my_ana = fmwk.ExampleSPSelection()
my_ana._mgr.SetSPAlgo(my_algo)
my_ana._mgr._training_mode =True
my_proc.add_process(my_ana)
my_proc.run()

# Re-set
my_proc.reset()
my_algo.SetMode(False)

# Set input root file
for x in xrange(len(sys.argv)-1):
    fname = sys.argv[x+1]
    if fname.find('electron')>=0:
        my_proc.add_input_file(fname)

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("electron_pdf.root")

my_ana = fmwk.ExampleSPSelection()
my_ana._mgr.SetSPAlgo(my_algo)
my_ana._mgr._training_mode =True

# Attach a template process
my_proc.add_process(my_ana)

print
print  "Finished configuring ana_processor. Start event loop!"
print
#sys.exit(1)
# Let's run it.
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

my_algo.StoreParams()
sys.exit(0)

