# Simple script to clean fann
import os,subprocess,sys

# Check shell env. var
if not "LARLITE_USERDEVDIR" in list(os.environ.keys()):
    print()
    print("  ERROR: Source LArLight config script first!")
    print()
    sys.exit(1)

# Clean library copies
PACKAGE_DIR = "%s/RecoTool/FANN" % os.environ['LARLITE_USERDEVDIR']
FANN_DIR = "%s/FANN-2.2.0-Source" % PACKAGE_DIR
libs = [x for x in os.listdir("%s/src" % FANN_DIR) if x.endswith(".so") or x.endswith(".dylib")]

for l in libs:    
    source = "%s/src/%s" % (FANN_DIR,l)
    target = "%s/%s" % (os.environ['LARLITE_LIBDIR'],l.replace(".dylib",".so"))
    if os.path.isfile(target):
        os.system("rm %s" % target)
    if os.path.isfile(source):
        os.system("rm %s" % source)
        




