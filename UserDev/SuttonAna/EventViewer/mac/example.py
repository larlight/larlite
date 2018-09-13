import sys
from ROOT import gSystem
gSystem.Load("libSuttonAna_EventViewer")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing EventViewer..."

sys.exit(0)

