import ROOT 
from ROOT import ertool, TFile

#
# Make my fake data input
#

# My toy data maker instance
data_maker=ertool.ToyDataMaker()
# Make 2 fake tracks
my_shower1=data_maker.MakeShower(1,0,0, 5, 5, 5,  22)
my_shower2=data_maker.MakeShower(0,0,0, 20,20,20, 22)
# Fake ID
myid=ertool.RecoInputID_t()

#
# Configure ERTool
#

# My ERTool manager
mgr=ertool.Manager()
# Get IO handler (to add shower/track toy data)
io=mgr.GetIOHandle()
in_handle=io.InputStream()
# Add algorithm
mgr.AddAlgo(ertool.ERAlgoToy())
# Add my algorithm's config file
mgr.AddCfgFile('dummy.cfg')

#
# Run it
#
mgr.Initialize()
io.Open()

# Fake "event" 1
mgr.ClearData()
in_handle.Add(my_shower1,myid)
in_handle.Add(my_shower2,myid)
mgr.Process()

# Fake "event" 2
mgr.ClearData()
in_handle.Add(my_shower1,myid)
in_handle.Add(my_shower2,myid)
mgr.Process()

# Give an analysis output file ptr & Finalize
fout = TFile("dummy.root","RECREATE")
mgr.Finalize(fout)
