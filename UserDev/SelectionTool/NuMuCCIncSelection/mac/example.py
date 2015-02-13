import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing NuMuCCIncSelection..."

sys.exit(0)

