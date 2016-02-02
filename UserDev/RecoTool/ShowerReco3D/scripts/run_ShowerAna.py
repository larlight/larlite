import sys,os
from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify analysis output root file name
my_proc.set_ana_output_file("ShowerAna_ana.root")

ana_unit=fmwk.ShowerAna()
ana_unit.setShowerProducer("showerreco")

my_proc.add_process(ana_unit)

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run()
# my_proc.process_event(2)



sys.exit()

