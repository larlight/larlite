from ROOT import ertool

def GetCCSingleEInstance():

    # Create algorithm
    my_algo = ertool.AlgoSingleE()
    my_algo.useRadLength(True)
#    my_algo.setVerbose(False)
    my_algo.setVtxToTrkStartDist(1)
    my_algo.setVtxToTrkDist(1)
    my_algo.setVtxToShrStartDist(50)
    my_algo.setMaxIP(1)
    my_algo.setVtxProximityCut(5)
    my_algo.setEThreshold(0.)

    # Return Algorithm Instance
    return my_algo
