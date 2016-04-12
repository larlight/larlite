import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from larlite import larlite as fmwk


# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root files
for x in xrange(len(sys.argv)):
    if x < 1:
        continue
    my_proc.add_input_file(sys.argv[x])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify analysis output root file name
my_proc.set_ana_output_file("results/ShowerQuality_singleshowers_pandoraNuMerged3D_v9_01_ana_out.root");

# Specify data output root file name
my_proc.set_output_file('')

# Create analysis unit
sq_module = fmwk.ShowerQuality_singleshowers()
#sq_module.setMCShowerQuality(True)
# sq_module.SetShowerProducer("showerrecofuzzy")
# sq_module.SetShowerProducer("showermergeall")
sq_module.SetShowerProducer("showerreco")

sq_module.SetMaxEnergyCut(99999999.)
sq_module.SetMinEnergyCut(0.)
sq_module.SetSingleParticleQuality(True)
my_proc.add_process(sq_module)

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run()

sys.exit(0)

