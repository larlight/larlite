from ROOT import ertool

def GetPrimaryFinderInstance():

    # Create algorithm
    my_algo = ertool.AlgoPrimaryFinder()
    my_algo.setVtxToTrkStartDist(1)
    my_algo.setVtxToTrkDist(1)
    my_algo.setVtxToShrStartDist(50)
    my_algo.setMaxIP(1)

    # Return Algorithm Instance
    return my_algo
