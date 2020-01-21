import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import ertool

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

# Set input root file
for x in range(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("michel_selection_ana_out.root")

# Possible filter to select true events
my_proc.add_process(fmwk.MichelElectronFilter())

my_algo = ertool.AlgoMichelE()
my_algo.LoadParams()
my_ana = fmwk.ExampleERSelection()
my_ana._mgr.SetAlgo(my_algo)
# OPTIONAL:
#my_ana.SetTrackProducer(False,"stitchkalmanhit")
#my_ana.SetShowerProducer(False,"newdefaultreco")
my_ana.SetTrackProducer(True,"mcreco")
my_ana.SetShowerProducer(True,"mcreco")

my_proc.add_process(my_ana)
my_proc.run()

my_algo.Finalize()


# done!
print()
print("Finished running ana_processor event loop!")
print()

#my_algo.StoreParams()
sys.exit(0)

