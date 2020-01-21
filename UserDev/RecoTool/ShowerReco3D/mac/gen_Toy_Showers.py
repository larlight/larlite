import sys

# if len(sys.argv) < 2:
#     msg  = '\n'
#     msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
#     msg += '\n'
#     sys.stderr.write(msg)
#     sys.exit(1)

from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
# for x in xrange(len(sys.argv)-1):
#     my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kWRITE)

my_proc.set_output_file("toyShowers.root")

# Specify output root file name
my_proc.set_ana_output_file("from_test_ana_you_can_remove_me.root");

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
proc = fmwk.ToyShowerGenerator()
proc.setOutProducerName("toy")

proc.printParams()

my_proc.add_process(proc)

print()
print("Finished configuring ana_processor. Start event loop!")
print()
my_proc.process_event(0)
my_proc.process_event(1)
my_proc.process_event(2)
# while my_proc.process_event():
  # pass

# Let's run it.
# my_proc.run(0,20)

# done!
print()
print("Finished running ana_processor event loop!")
print()

sys.exit(0)
