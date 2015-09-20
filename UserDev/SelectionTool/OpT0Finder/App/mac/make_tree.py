import sys,ROOT

def pmt_pos():
    xv = ROOT.std.vector("double")()
    yv = ROOT.std.vector("double")()
    zv = ROOT.std.vector("double")()
    for line in open('opt_geo.txt','r').read().split('\n'):
        words = line.split()
        if not len(words) == 3: continue
        xv.push_back(float(words[0]))
        yv.push_back(float(words[1]))
        zv.push_back(float(words[2]))
    return (xv,yv,zv)

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

import ROOT
#ROOT.gSystem.Load("libOpFlashAna_OpT0FinderApp")
from larlite import larlite as fmwk
from ROOT import flashana

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-2):
    print sys.argv[x+1]
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("ana.root")

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_unit = fmwk.UBT0Finder()
my_proc.add_process(my_unit)
my_unit.UseMC()
my_unit.Manager().SetAlgo(flashana.NPtFilter())
my_unit.Manager().SetAlgo(flashana.MaxNPEWindow())

#my_unit.Manager().SetVerbosity(0)

xv,yv,zv = pmt_pos()

#match_alg = flashana.QWeightPoint(int(sys.argv[-1]))

#match_alg = flashana.QWeightPoint(xv,yv,zv,int(sys.argv[-1]))
#match_alg.UsePhotonLibrary(True)

match_alg = flashana.QLLMatch.GetME()
match_alg.SetOpDetPositions(xv,yv,zv)
match_alg.UsePhotonLibrary(True)

my_unit.Manager().SetAlgo(match_alg)

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run();

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)
