from ROOT import ertool

def GetPrimaryCosmicFinderInstance():

    # Create algorithm
    my_algo = ertool.ERAlgoCRPrimary()

    # Return Algorithm Instance
    return my_algo
