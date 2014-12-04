from test_msg import debug, info, error, warning
import traceback,sys
from random import *
from printGeoAlgo import *
import numpy as np
from ROOT import geoalgo

def test_dAlgo():

    # import Distance Algo
    dAlgo = geoalgo.DistanceAlgo()

    try:

        # test distance from point to segment
        for y in range(100):
            # generate a random segment
            l = geoalgo.LineSegment(random(),random(),random(),random(),random(),random())
            # get the segment length
            lLen = l.Dir().Length()
            # get the segment direction
            d = l.Dir()
            # generate a segment parallel to it
            # to get a line parallel to the first,
            # translate the Start and End points
            # by some uniform amount in any direction
            vectTranslate = geoalgo.Vector(random(),random(),random())
            p1 = l.Start()+vectTranslate
            p2 = l.End()+vectTranslate
            # parallel segment:
            lPar = geoalgo.LineSegment(p1,p2)
            # make sure the two segments are parallel:
            debug('Testing if two lines are Parallel')
            #printLineSegment(l.Dir())
            #printLineSegment(lPar.Dir())
            for x in xrange(3):
                print l.Dir()[x]
                print lPar.Dir()[x]
                if not  (l.Dir()[x] == lPar.Dir()[x]) : raise Exception
            # first, test a point that is "before start point"
            # distance to this point should be distance between lines
            # in quadrature with how much further from start point we go
            #dist = random()
            #pTest = p1+geoalgo.Vector(d[0]+dist,d[1]+dist,d[2]+dist)
            #answer = np.sqrt(dist+vectTranslate.Length())
            #debug('Testing Point & LineSegment Distance')
            #if not ( answer == dAlgo.SqDist(pTest,l) ): raise Exception
        
    except Exception:
        error('geoalgo::DistanceAlgo unit test failed.')
        print traceback.format_exception(*sys.exc_info())[2]
        return 1

    info('geoalgo::DistanceAlgo unit test complete.')
    return 0

if __name__ == '__main__':
    test_dAlgo()

