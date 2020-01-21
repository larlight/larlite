import sys
from ROOT import gSystem
gSystem.Load("libGraphForDummies_GeoGraph")
from ROOT import sample

try:

    print("PyROOT recognized your class %s" % str(sample))

except NameError:

    print("Failed importing GeoGraph...")

sys.exit(0)

