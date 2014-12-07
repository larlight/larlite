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
my_proc.set_ana_output_file("from_test_ana_you_can_remove_me.root")

ana_unit = fmwk.ExampleSPSelection()
ana_unit._mgr.SetSPAlgo(sptool.SPAlgoPi0())
ana_unit._mgr.SetFilter(sptool.SPFilterToy())

# Attach a template process
my_proc.add_process(ana_unit)

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run();

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

