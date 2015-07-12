import sys
from seltool import ertool
from larlite import larlite as fmwk

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

def EMPartAlgo():
    my_algo = ertool.AlgoEMPart()
    my_algo.Reset()
    my_algo.SetMode(False) # True = Gamma. False = Electron.
    my_algo.setVerbose(False)
    my_algo.setPlot(True)
    if ask_binary('  Load previously extracted fit parameters? [y/n]:'):
        my_algo.setLoadParams(True)
    else:
        my_algo.setLoadParams(False)
    return my_algo

def EMPartAna():
    my_ana = ertool.ERAnaEMPart()
    my_ana.SetDebug(True)
    return my_ana

def AnaUnit():
    my_anaunit = fmwk.ExampleERSelection()
    # Set Producers
    # First Argument: True = MC, False = Reco
    my_anaunit.SetShowerProducer(True,"mcreco");
    my_anaunit.SetTrackProducer(True,"mcreco");
    #my_anaunit.SetShowerProducer(False,"showerreco");
    #my_anaunit.SetTrackProducer(False,"");
    my_anaunit._mode =True # True = Select. False = Fill mode
    my_anaunit._mgr._mc_for_ana = True


    return my_anaunit
