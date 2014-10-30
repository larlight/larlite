import sys
from ROOT import *
gSystem.Load("libClusterStudy")
from ROOT import cmtool

bk=cmtool.MatchBookKeeper()


myset = std.vector("unsigned int")()

myset.clear()
myset.push_back(1)
myset.push_back(3)
myset.push_back(5)
bk.Match(myset,1)

myset.clear()
myset.push_back(0)
myset.push_back(2)
myset.push_back(4)
bk.Match(myset,1)

myset.clear()
myset.push_back(6)
myset.push_back(7)
myset.push_back(8)
bk.Match(myset,1)

myset.clear()
myset.push_back(1)
myset.push_back(2)
myset.push_back(3)
bk.Match(myset,2)

res=bk.GetResult()

for x in xrange(res.size()):

    for y in xrange(res.at(x).size()):

        print res.at(x).at(y),

    print
