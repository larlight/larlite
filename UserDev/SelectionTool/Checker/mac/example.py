import sys
from ROOT import gSystem
gSystem.Load("libSelectionTool_Checker")
from ROOT import sample

try:

    print("PyROOT recognized your class %s" % str(sample))

except NameError:

    print("Failed importing Checker...")

sys.exit(0)

