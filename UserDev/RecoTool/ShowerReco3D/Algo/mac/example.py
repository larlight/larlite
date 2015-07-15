import sys
from ROOT import gSystem
gSystem.Load("libRecoTool_Algo")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Algo..."

sys.exit(0)

