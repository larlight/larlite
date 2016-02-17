import sys

from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

my_unit = fmwk.MakeFakeShowers()
my_proc.add_process(my_unit)

# Specify output root file name
my_proc.set_ana_output_file("ana.root")

my_proc.set_output_file("fakeshowers.root")

my_proc.set_data_to_write(fmwk.data.kMCShower,"fakeshower")

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run(0,100)

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)
