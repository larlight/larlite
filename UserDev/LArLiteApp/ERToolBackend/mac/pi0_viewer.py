import sys, os
from ROOT import gSystem
from ROOT import ertool
ertool.Manager()
from ROOT import larlite as fmwk
fmwk.geo.PlaneID

#from basictool import GeoViewer, geoalgo
from seltool import ERViewer
import matplotlib.pyplot as plt

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

def main():

    if len(sys.argv) < 2:
        msg  = '\n'
        msg += "Usage: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
        msg += '\n'
        sys.stderr.write(msg)
        sys.exit(1)

    ertool.Manager()

    #viewer
    plt.ion()
    #display = GeoViewer()
    display = ERViewer()
    display.show()

    # Create ana_processor instance
    my_proc = fmwk.ana_processor()
    # Create algorithm
    my_algo = ertool.AlgoPi0()
    my_algo.setVerbose(True)
    my_algo.setMinShrEnergy(10)
    my_algo.setMaxShrEnergy(1000)
    my_algo.setIPMax(10)
    my_algo.setMinFitMass(50)
    my_algo.setMaxFitMass(200)
    my_algo.setAngleMax(3.14)

    # Create analysis unit
    my_ana = fmwk.ExampleERSelection()

    # ************Set Producers**************
    # First Argument: True = MC, False = Reco
    #my_ana.SetShowerProducer(True,"mcreco");
    #my_ana.SetTrackProducer(True,"mcreco");
    #my_ana.SetVtxProducer(True,"generator");
    my_ana.SetShowerProducer(False,"showerreco");
    my_ana.SetTrackProducer(False,"stitchkalmanhit");
    my_ana.SetVtxProducer(False,"");
    # ************Set Producers**************

    #help(my_ana._mgr)
    my_ana._mgr.SetAlgo(my_algo)
    my_ana._mgr._training_mode =True

    # Obtain input root files
    files = []
    for x in xrange(len(sys.argv)-1):
        fname = sys.argv[x+1]
        if fname.endswith('.root'):
            files.append(fname)

    print
    print '  Running AlgoPi0 training script...'
    print
    print '  Identified %2d input files' % len(files)
    if not ask_binary('  Proceed? [y/n]:'): return False
    print
    if ask_binary('  Load previously extracted fit parameters? [y/n]:'):
        my_algo.LoadParams()    

    my_proc.set_io_mode(fmwk.storage_manager.kREAD)
    my_proc.add_process(my_ana)
    for f in files:
        my_proc.add_input_file(f)

    my_ana._mgr.Reset()
    my_proc.set_ana_output_file("pi0_viewing.root")
    print '    Start running pi0 training...'

    # Start event-by-event loop
    counter = 0
    while (counter < 1000):
        try:
            counter = input('Hit Enter to continue to next evt, or type in an event number to jump to that event:')
        except SyntaxError:
            counter = counter + 1
        my_proc.process_event(counter)
        # get objets and display
        display.clear()
        
        display.add(my_ana.GetParticles(),
                    my_ana.GetData())

        display.show()

    #my_proc.run()

    print
    print '    Finished running pi0 training...'
    print

    #
    # Store trained parameters
    #
    if ask_binary('  Store train result parameters? [y/n]:'):
        my_algo.StoreParams()
        print '  Parameter stored...'
        print

    return True

if __name__ == '__main__':
    if not main(): 
        print
        print 'Process terminated.'
    else:
        print
        print 'Process finished.'
    print

