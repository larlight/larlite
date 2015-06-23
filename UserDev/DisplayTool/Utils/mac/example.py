import sys
from ROOT import gSystem
gSystem.Load("libEventViewer_Utils")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Utils..."

sys.exit(0)

