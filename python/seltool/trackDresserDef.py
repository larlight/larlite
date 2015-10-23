from ROOT import ertool

def GetTrackDresserInstance():

    # Create algorithm
    my_algo = ertool.ERAlgoTrackDresser()
    my_algo.setMinShwToTrackDist(15)
    my_algo.setMinTrkStartDist(10)
    my_algo.setMinTrkEndDist(10)
    my_algo.setMinTrkDeltaDist(5)

    # Return Algorithm Instance
    return my_algo
