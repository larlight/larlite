import sys
from ROOT import gSystem
gSystem.Load("libLArLiteApp_PIDTest")
from ROOT import sample

try:

    print("PyROOT recognized your class %s" % str(sample))

except NameError:

    print("Failed importing PIDTest...")

sys.exit(0)

