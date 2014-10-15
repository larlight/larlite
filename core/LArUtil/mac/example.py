import sys
from ROOT import *

try:

    print "PyROOT recognized your class %s" % str(LArUtil)

except NameError:

    print "Failed importing LArUtil..."

sys.exit(0)

