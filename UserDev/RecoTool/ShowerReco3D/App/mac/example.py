import sys
from ROOT import gSystem
gSystem.Load("libRecoTool_App")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing App..."

sys.exit(0)

