import sys
from ROOT import gSystem
gSystem.Load("libMCTrack_Check_MCTrack_dEdx")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing MCTrack_dEdx..."

sys.exit(0)

