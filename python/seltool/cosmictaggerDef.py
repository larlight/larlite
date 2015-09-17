from ROOT import ertool

def GetCosmicTaggerInstance():

    # Create algorithm
    my_algo = ertool.ERAlgoCosmicTagger()
#    my_algo.setVerbose(False)
    my_algo.setMinShwToTrackDist(5)
    my_algo.setMinTrkStartDist(15)
    my_algo.setMinTrkEndDist(5)
    my_algo.setMinTrkDeltaDist(0.5)

    # Return Algorithm Instance
    return my_algo
