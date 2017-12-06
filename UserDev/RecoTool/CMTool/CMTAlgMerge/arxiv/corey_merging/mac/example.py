import sys
from ROOT import gSystem
gSystem.Load("libargoneut_electrons_merging")
from ROOT import larlite

try:

    print "PyROOT recognized your class %s" % str(HitToCluster)

except NameError:

    print "Failed importing merging..."

sys.exit(0)

