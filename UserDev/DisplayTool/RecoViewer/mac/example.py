import sys
from ROOT import gSystem
gSystem.Load("libEventViewer_RecoViewer")
from ROOT import sample

try:

    print("PyROOT recognized your class %s" % str(sample))

except NameError:

    print("Failed importing RecoViewer...")

sys.exit(0)

