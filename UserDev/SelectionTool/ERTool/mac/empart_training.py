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
        msg += "$INPUT_ROOT_FILE containing 'gamma' ('electron') is used for training SPAlgoEMPart gamma (electron) mode.\n"
        msg += '\n'
        sys.stderr.write(msg)
        sys.exit(1)

    # Create ana_processor instance
    my_proc = fmwk.ana_processor()
    # Create algorithm
    my_algo = sptool.SPAlgoEMPart()
    my_algo.set_e_dEdxFitRange(1.2,2); # To set range for fitting for dEdx
    my_algo.set_g_dEdxFitRange(3,4.5);
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

    # Check if gamma/electron files are provided:
    gamma_files    = []
    electron_files = []
    for x in xrange(len(sys.argv)-1):
        fname = sys.argv[x+1]
        if fname.find('gamma')>=0:
            gamma_files.append(fname)
        else:
            electron_files.append(fname)

    print
    print '  Running SPAlgoEMPart training script...'
    print
    print '  Identified %2d input files for gamma' % len(gamma_files)
    print '  Identified %2d input files for electron' % len(electron_files)
    if not ask_binary('  Proceed? [y/n]:'): return False
    print
    if ask_binary('  Load previously extracted fit parameters? [y/n]:'):
        my_algo.LoadParams()    
    #
    # Training for gamma mode
    #
    gamma_trained = False
    if len(gamma_files) and ask_binary('  Run training for gamma? [y/n]:'):

        my_proc.set_io_mode(fmwk.storage_manager.kREAD)
        my_proc.add_process(my_ana)
        for f in gamma_files:
            my_proc.add_input_file(f)

        my_algo.SetMode(True)
        my_ana._mgr.Reset()
        my_proc.set_ana_output_file("gamma_training.root")
        print '    Start running gamma training...'
        my_proc.run()
        print
        print '    Finished running gamma training...'
        print
        # Re-set
        my_proc.reset()
        gamma_trained = True

    #
    # Training for electron mode
    #
    electron_trained = False
    if len(electron_files) and ask_binary('  Run training for electron? [y/n]:'):

        my_proc.set_io_mode(fmwk.storage_manager.kREAD)
        my_proc.add_process(my_ana)
        for f in electron_files:
            my_proc.add_input_file(f)

        my_algo.SetMode(False)
        my_ana._mgr.Reset()
        my_proc.set_ana_output_file("electron_training.root")
        print '    Start running electron training...'
        my_proc.run()
        print
        print '    Finished running electron training...'
        print
        # Re-set
        my_proc.reset()
        electron_trained = True

    #
    # Store trained parameters
    #
    if (gamma_trained or electron_trained) and ask_binary('  Store train result parameters? [y/n]:'):
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

