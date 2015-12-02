from ROOT import ertool

def GetERAlgoPi0Instance():

    # Create algorithm
    my_algo = ertool.ERAlgopi0()
    my_algo.setMinMass(40.)
    my_algo.setMaxMass(220.)
    my_algo.setMaxIP(10.)
    my_algo.setMinShrEnergy(10.)

    # Return Algorithm Instance
    return my_algo
