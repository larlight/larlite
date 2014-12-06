from test_msg import debug, info, error, warning
import traceback,sys
from random import *
import numpy as np
from ROOT import geoalgo

_epsilon = 1E-7

def test_dAlgo():

    # number of times to test each function
    tests = 10000

    # import Distance Algo
    dAlgo = geoalgo.DistanceAlgo()

    try:

        # test distance from point to segment
        debug('Testing Point & LineSegment Distance')
        success = 1
        totSuccess = 0
        for y in range(tests):
            # generate a random segment
            l = geoalgo.LineSegment(random(),random(),random(),random(),random(),random())
            # get the segment length
            lLen = l.Dir().Length()
            # get the segment direction
            d = l.Dir()
            # generate a segment parallel to it
            # to get a line parallel to the first,
            # translate the Start and End points
            # by some amount in a direction perpendicular
            # to that of the original line
            transX = random()
            transY = random()
            # now ensure perpendicularity by having dot product be == 0
            transZ = (-transX*d[0]-transY*d[1])/d[2]
            vectTranslate = geoalgo.Vector(transX,transY,transZ)
            p1 = l.Start()+vectTranslate
            p2 = l.End()+vectTranslate
            # parallel segment:
            lPar = geoalgo.LineSegment(p1,p2)

            # first, test a point that is "before start point"
            # distance to this point should be distance between lines
            # in quadrature with how much further from start point we go
            dist = random()
            # direction vector outwards from line
            dirOut = lPar.Dir()*(-1*dist/lPar.Dir().Length())
            pTest = p1+dirOut
            answer = dirOut.SqLength()+vectTranslate.SqLength()
            if not ( np.abs(answer-dAlgo.SqDist(pTest,l)) < _epsilon): success = 0
            if not ( np.abs(answer-dAlgo.SqDist(l,pTest)) < _epsilon): success = 0
            point1 = dAlgo.ClosestPt(pTest,l)
            point2 = dAlgo.ClosestPt(l,pTest)
            for x in xrange(3):
                if not ( (l.Start()[x]-point1[x]) < _epsilon ) : success = 0
                if not ( (l.Start()[x]-point2[x]) < _epsilon ) : success = 0

            # now, test a point inside the segment.
            # distance between point & segment should be 
            # pre-determined distance between the two segments
            dist = random()
            dirIn = lPar.Dir()*dist #dist ensures < distance of full segment
            pTest = p1+dirIn
            answer = vectTranslate.SqLength()
            if not (np.abs(answer-dAlgo.SqDist(pTest,l)) < _epsilon): success = 0
            if not (np.abs(answer-dAlgo.SqDist(l,pTest)) < _epsilon): success = 0
            pAns = l.Start()+dirIn
            point1 = dAlgo.ClosestPt(pTest,l)
            point2 = dAlgo.ClosestPt(l,pTest)
            for x in xrange(3):
                if not ( (pAns[x]-point1[x]) < _epsilon ) : success = 0
                if not ( (pAns[x]-point2[x]) < _epsilon ) : success = 0

            # now test a point beyond the segment
            dist = random()
            dirOut = lPar.Dir()*(dist/lPar.Dir().Length())
            pTest = p2+dirOut
            answer = dirOut.SqLength()+vectTranslate.SqLength()
            if not ( np.abs(answer-dAlgo.SqDist(pTest,l)) < _epsilon): success = 0
            if not ( np.abs(answer-dAlgo.SqDist(l,pTest)) < _epsilon): success = 0
            point1 = dAlgo.ClosestPt(pTest,l)
            point2 = dAlgo.ClosestPt(pTest,l)
            for x in xrange(3):
                if not ( (l.End()[x]-point1[x]) < _epsilon ) : success = 0
                if not ( (l.End()[x]-point2[x]) < _epsilon ) : success = 0
                
            if (success == 1) : totSuccess += 1
        print "Success: {0}%".format(100*float(totSuccess)/tests)
            
        # test Point to HalfLine distance
        debug('Testing Point & HalfLine Distance')
        success = 1
        totSuccess = 0
        for x in range(tests):
            # generate a random segment
            l = geoalgo.HalfLine(random(),random(),random(),random(),random(),random())
            # generate a segment parallel to it
            # to get a line parallel to the first,
            # translate the Start and End points
            # by some amount in a direction perpendicular
            # to that of the original line
            transX = random()
            transY = random()
            # now ensure perpendicularity by having dot product be == 0
            transZ = (-transX*l.Dir()[0]-transY*l.Dir()[1])/l.Dir()[2]
            vectTranslate = geoalgo.Vector(transX,transY,transZ)
            # create the new translated & parallel hal-fline
            lPar = geoalgo.HalfLine(l.Start()+vectTranslate, l.Dir())
            # first, test a point that is "before start point"
            # distance to this point should be distance between lines
            # in quadrature with how much further from start point we go
            dist = random()
            # direction vector outwards from line
            dirOut = lPar.Dir()*(-1*dist)
            pTest = lPar.Start()+dirOut
            answer = dirOut.SqLength()+vectTranslate.SqLength()
            if not ( np.abs(answer-dAlgo.SqDist(pTest,l)) < _epsilon): success = 0
            if not ( np.abs(answer-dAlgo.SqDist(l,pTest)) < _epsilon): success = 0
            point1 = dAlgo.ClosestPt(pTest,l)
            point2 = dAlgo.ClosestPt(l,pTest)
            for x in xrange(3):
                if not ( (l.Start()[x]-point1[x]) < _epsilon ) : success = 0
                if not ( (l.Start()[x]-point2[x]) < _epsilon ) : success = 0

            # now, test a point inside the segment.
            # distance between point & segment should be 
            # pre-determined distance between the two segments
            dist = random()
            dirIn = lPar.Dir()*dist #dist ensures < distance of full segment
            pTest = lPar.Start()+dirIn
            answer = vectTranslate.SqLength()
            if not (np.abs(answer-dAlgo.SqDist(pTest,l)) < _epsilon): success = 0
            if not (np.abs(answer-dAlgo.SqDist(l,pTest)) < _epsilon): success = 0
            pAns = l.Start()+dirIn
            point1 = dAlgo.ClosestPt(pTest,l)
            point2 = dAlgo.ClosestPt(l,pTest)
            for x in xrange(3):
                if not ( (pAns[x]-point1[x]) < _epsilon ) : success = 0
                if not ( (pAns[x]-point2[x]) < _epsilon ) : success = 0

            if (success == 1) : totSuccess += 1
        print "Success: {0}%".format(100*float(totSuccess)/tests)

        # test Distance between two Infinite Lines
        debug('Testing Inf Line & Inf Line Distance')
        success = 1
        totSuccess = 0
        for y in range(tests):
            l1 = geoalgo.Line(random(),random(),random(),random(),random(),random())
            # take a point a fixed distance away
            # generate a random direction in the plane
            # perpendicular to the plane connecting
            # the point and the line
            # the distance between the two lines
            # should be the fixed amount selected previously
            # use half-way point to do calculations
            p1 = (l1.Pt2()+l1.Pt1())/2
            # get line direction
            d1 = (l1.Pt2()-l1.Pt1())
            # move in a random direction perpendicular to line
            dirx = random()
            diry = random()
            dirz = (-dirx*d1[0]-diry*d1[1])/d1[2]
            vectTranslate = geoalgo.Vector(dirx,diry,dirz)
            # need to re-orient in some random direction on this plane
            # this direction has to be perpendicular to both
            # the line's direction as well as to the direction
            # of the segment uniting the two lines
            # use cross-product
            vectRotate = vectTranslate.Cross(d1)
            l2 = geoalgo.Line(p1+vectTranslate,p1+vectTranslate+vectRotate)
            # now calculate distance. Should be vectTranslate.Length()
            answer = vectTranslate.SqLength()
            # expect the closest points on both lines to be p1 & l2.Pt1()
            ptL1 = geoalgo.Vector()
            ptL2 = geoalgo.Vector()
            if not (np.abs(answer-dAlgo.SqDist(l1,l2)) < _epsilon): success = 0
            if not (np.abs(answer-dAlgo.SqDist(l1,l2,ptL1,ptL2)) < _epsilon) : success = 0
            for x in xrange(3):
                if not ( np.abs(ptL1[x]-p1[x]) < _epsilon ) : success = 0
                if not ( np.abs(ptL2[x]-l2.Pt1()[x]) < _epsilon ) : success = 0

            if (success == 1) : totSuccess += 1
        print "Success: {0}%".format(100*float(totSuccess)/tests)
            
    except Exception:
        error('geoalgo::DistanceAlgo unit test failed.')
        print traceback.format_exception(*sys.exc_info())[2]
        return 1

    info('geoalgo::DistanceAlgo unit test complete.')
    return 0

if __name__ == '__main__':
    test_dAlgo()

