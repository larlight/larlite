import sys
from ROOT import gSystem
gSystem.Load("libOpFlashAna_Ana")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Ana..."

sys.exit(0)

