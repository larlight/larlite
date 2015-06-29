import ROOT
from ROOT import ertool

#
# Make my fake data input
#

# My toy data maker instance
data_maker=ertool.ToyDataMaker()
# Make 2 fake tracks
my_shower1=data_maker.MakeShower(1,0,0, 5, 5, 5,  22)
my_shower2=data_maker.MakeShower(0,0,0, 20,20,20, 22)
# Fake ID
myid=ROOT.std.pair("size_t","string")()

#
# Configure ERTool
#

# My ERTool manager
mgr=ertool.Manager()
# Add algorithm
mgr.AddAlgo(ertool.ERAlgoToy())
# Add my algorithm's config file
mgr.AddCfgFile('dummy.cfg')

#
# Run it
#
mgr.Initialize()

# Fake "event" 1
mgr.ClearData()
mgr.Add(my_shower1,myid)
mgr.Add(my_shower2,myid)
mgr.Process()

# Fake "event" 2
mgr.ClearData()
mgr.Add(my_shower1,myid)
mgr.Add(my_shower2,myid)
mgr.Process()

mgr.Finalize()
