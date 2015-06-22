import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from seltool import ertool
from larlite import larlite as fmwk
from seltool.algoviewer import viewAll

def ask_binary(msg='Proceed? [y/n]:'):
    
    user_input=''
    while not user_input:
        sys.stdout.write(msg)
        sys.stdout.flush()
        user_input = sys.stdin.readline().rstrip('\n').lower()
        if user_input in ['y','n']: break

        print 'Invalid input:',user_input
        user_input=''

    return user_input == 'y'

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("EMPartSelection.root")

my_algo = ertool.AlgoEMPart()
my_algo.Reset()
my_algo.SetMode(False) # True = Gamma. False = Electron.
my_algo.setVerbose(False)
my_algo.setPlot(True)

if ask_binary('  Load previously extracted fit parameters? [y/n]:'):
    my_algo.setLoadParams(True)
else:
    my_algo.setLoadParams(False)

my_ana = ertool.ERAnaEMPart()
my_ana.SetDebug(True)

my_anaunit = fmwk.ExampleERSelection()

# Set Producers
# First Argument: True = MC, False = Reco
#my_ana.SetShowerProducer(True,"mcreco");
#my_ana.SetTrackProducer(True,"mcreco");
my_anaunit.SetShowerProducer(False,"showerreco");
my_anaunit.SetTrackProducer(False,"");

my_anaunit._mgr.AddAlgo(my_algo)
my_anaunit._mgr.AddAna(my_ana)
my_anaunit._mode =True # True = Select. False = Fill mode
my_anaunit._mgr._mc_for_ana = True
my_proc.add_process(my_anaunit)

# start event-by-event loop
counter = 0
while (my_proc.process_event(counter)):
    try:
        counter = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
    except SyntaxError:
        counter = counter + 1
    print "Event number: ", counter
    my_proc.process_event(counter)
    print "Processing event {0}".format(counter) 
    data_reco = my_anaunit.GetData()
    part_reco = my_anaunit.GetParticles()
    data_mc   = my_anaunit.GetData(True)
    part_mc   = my_anaunit.GetParticles(True)
    viewAll(data_mc,part_mc,data_reco,part_reco)

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

