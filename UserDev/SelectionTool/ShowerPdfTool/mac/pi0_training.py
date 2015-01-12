import sys
from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import sptool

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

    # Create ana_processor instance
    my_proc = fmwk.ana_processor()
    # Create algorithm
    my_algo = sptool.SPAlgoPi0()
    # Create analysis unit
    my_ana = fmwk.ExampleSPSelection()

    # Set Producers
    # First Argument: True = MC, False = Reco
    #my_ana.SetShowerProducer(True,"mcreco");
    #my_ana.SetTrackProducer(True,"mcreco");
    #my_ana.SetVtxProducer(True,"generator");
    
    my_ana.SetShowerProducer(False,"showerreco");
    my_ana.SetTrackProducer(False,"");
    my_ana.SetVtxProducer(False,"");

    my_ana._mgr.SetSPAlgo(my_algo)
    my_ana._mgr._training_mode =True

    # Obtain input root files
    files = []
    for x in xrange(len(sys.argv)-1):
        fname = sys.argv[x+1]
        if fname.endswith('.root'):
            files.append(fname)

    print
    print '  Running SPAlgoPi0 training script...'
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
    my_proc.set_ana_output_file("pi0_training.root")
    print '    Start running pi0 training...'
    my_proc.run()
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

