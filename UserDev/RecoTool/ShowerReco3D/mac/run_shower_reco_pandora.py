import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)


from ROOT import gSystem,TMath
from ROOT import larlite as fmwk, showerreco, larutil


# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify analysis output root file name
my_proc.set_ana_output_file("");

# Specify data output root file name
my_proc.set_output_file("showerreco_batch_out_pandora.root")

#
# Kazu disables to read-in shower data product from input (only speeds up)
#

# Create analysis unit
ana_unit = fmwk.ShowerReco3D()

# Attach shower reco alg
sralg = showerreco.ShowerRecoAlg()
sralg.Verbose(False)
#sralg.Verbose(True)
sralg.SetUseArea(True)
#sralg.SetUseModBox(True)
ana_unit.SetShowerAlgo(sralg)

# Specify cluster type
ana_unit.SetInputProducer("pandoraNu",True)

my_proc.add_process(ana_unit)

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run()

sys.exit()

