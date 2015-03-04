import sys
from ROOT import gSystem
gSystem.Load("libRepositoryName_Package_Name")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Package_Name..."

sys.exit(0)

