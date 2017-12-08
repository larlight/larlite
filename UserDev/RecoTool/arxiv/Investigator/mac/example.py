import sys
from ROOT import gSystem
gSystem.Load("libRecoTool_Investigator")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Investigator..."

sys.exit(0)

