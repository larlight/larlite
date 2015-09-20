from ROOT import ertool

def GetTrackPidInstance():

    # Create algorithm
    my_algo = ertool.ERAlgoTrackPid() 

    # Return Algorithm Instance
    return my_algo
