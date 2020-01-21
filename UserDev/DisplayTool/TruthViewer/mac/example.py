import sys
from ROOT import gSystem
gSystem.Load("libEventViewer_TruthViewer")
from ROOT import sample

try:

    print("PyROOT recognized your class %s" % str(sample))

except NameError:

    print("Failed importing TruthViewer...")

sys.exit(0)

