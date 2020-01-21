import sys
from ROOT import gSystem
gSystem.Load("libPhotonLibraryImport_PhotonLibrary")
from ROOT import sample

try:

    print("PyROOT recognized your class %s" % str(sample))

except NameError:

    print("Failed importing PhotonLibrary...")

sys.exit(0)

