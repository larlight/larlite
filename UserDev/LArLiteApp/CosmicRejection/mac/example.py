import sys
from ROOT import gSystem
gSystem.Load("libLArLiteApp_CosmicRejection")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing CosmicRejection..."

sys.exit(0)

