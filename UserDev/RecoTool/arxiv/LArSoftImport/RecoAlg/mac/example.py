import sys
from ROOT import gSystem
gSystem.Load("libLArSoftImport_RecoAlg")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing RecoAlg..."

sys.exit(0)

