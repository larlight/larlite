# hey, here's some info on how to use this script:
# it'll run the nominal shower reco, and then reconstruct a mass peak
#
# the bool on line 108 chooses mc information / truth information
# true  - run pi0 reconstruction on mcshowers
# false - run pi0 reconstruction on reco'd showers, w/ producer set on line 87
#
# it also generates some plots with pyplot
# if you want to disable this, just uncomment line 159

import sys, ROOT, os
from matplotlib import pyplot as plt
from matplotlib import patches

if len(sys.argv) < 2:
  msg  = "\n"
  msg += "Usage 1: {} $INPUT_FILE_ROOT(s)\n".format(sys.argv[0])
  msg += "\n"
  sys.stderr.write(msg)
  sys.exit(1)

from larlite import larlite as fmwk
from seltool import ertool
from recotool import showerreco

# function to handle all the shower reco setup
def ShowerReco():
  
  ana_unit = fmwk.ShowerReco3D()

  alg = showerreco.ShowerRecoAlgModular()
  alg.SetDebug(False)

  axis3D = showerreco.Axis3DModule()
  axis3D.setMaxIterations(100)
  axis3D.setNStepsInitial(25)
  axis3D.setTargetError(0.001)
  axis3D.setNormalErrorRange(0.01)
  axis3D.setThetaRangeStart(0.1)
  axis3D.setThetaRangeMin(0.0005)
  axis3D.setNStepsStart(4)
  axis3D.setConvergeRate(0.85)
  axis3D.setVerbosity(False)
  axis3D.setSeedVectorErrorCutoff(0.1)

  angle3D = showerreco.Angle3DFormula()
  angle3D.setMaxAngleError(0.1)
  angle3D.setVerbosity(False)
  
  energy = showerreco.LinearEnergy()
  energy.SetUseModBox(True)
  energy.setVerbosity(False)
  
  dqdx = showerreco.dQdxModule()
  
  dedx = showerreco.dEdxFromdQdx()
  dedx.SetUsePitch(False)
  dedx.setVerbosity(False)
  
  #alg.AddShowerRecoModule(showerreco.StartPoint2DModule()  )
  #alg.AddShowerRecoModule(showerreco.SanityCheckInitial() )
  alg.AddShowerRecoModule(axis3D)
  alg.AddShowerRecoModule(showerreco.StartPoint3DModule() )
  #alg.AddShowerRecoModule(angle3D)
  #alg.AddShowerRecoModule(showerreco.StartPoint3DModule()  )
  alg.AddShowerRecoModule(energy)
  alg.AddShowerRecoModule(dqdx)
  alg.AddShowerRecoModule(dedx)
  #alg.AddShowerRecoModule(showerreco.OtherStartPoint3D()  )
  #alg.AddShowerRecoModule(showerreco.ShowerChargeModule()  )
  #alg.AddShowerRecoModule(showerreco.GeoModule())
  #alg.AddShowerRecoModule(showerreco.SanityCheckFinal() )
  
  alg.PrintModuleList()
  
  ana_unit.AddShowerAlgo(alg)
  return ana_unit

# function to set up all the ertool handles
def ERTool():

  ana_unit = fmwk.ExampleERSelection()

  if use_mc:
    ana_unit.SetShowerProducer(True,"mcreco")
  else:
    ana_unit.SetShowerProducer(False,"showerreco")

  # ana_unit.SetTrackProducer(True,"mcreco")
  # ana_unit.SetFlashProducer("opflashSat")

  # algo_flashmatch = ertool.ERAlgoFlashMatch()
  # ana_unit._mgr.AddAlgo(algo_flashmatch)
  
  # ana_flashefficiency = ertool.ERAnaFlashEfficiency()
  # ana_unit._mgr.AddAna(ana_flashefficiency)

  ana_masspeak = ertool.ERAnaMassPeak()
  ana_masspeak.SetEnergyCut(40.)
  ana_masspeak.SetAngleCut(0.175)
  ana_masspeak.SetEnergySumCut(70.)
  ana_masspeak.SetVtxCut(100.)
  ana_unit._mgr.AddAna(ana_masspeak)

  return ana_unit

# choose MC or reco
use_mc = False

# set up the ana processor
ana_processor = fmwk.ana_processor()

for x in xrange(len(sys.argv)-1):
  ana_processor.add_input_file(sys.argv[x+1])

ana_processor.set_io_mode(fmwk.storage_manager.kBOTH)

if use_mc:
  ana_processor.set_ana_output_file("pi0reco_ana_mc.root")
else:
  ana_processor.set_ana_output_file("pi0reco_ana_reco.root")
ana_processor.set_output_file("pi0reco.root")

# draw a big ol' line
#ana_processor.add_process(fmwk.DrawLine())

# run pfpart filter
# ana_pi0showerfilter = fmwk.Pi0ShowerFilter()
#ana_processor.add_process(ana_pi0showerfilter, True)

# run shower reco
ana_showerreco = ShowerReco()

#ana_showerreco.SetInputProducer("pandoraNu")
ana_showerreco.SetInputProducer("ImageClusterHit")

ana_showerreco.SetOutputProducer("showerreco")
ana_processor.add_process(ana_showerreco)

# add in ertool processes
ana_ertool = ERTool()
ana_processor.add_process(ana_ertool)

# run shower quality
ana_showerquality = fmwk.ShowerQuality_multishowers()
ana_showerquality.SetShowerProducer("showerreco")
ana_showerquality.SetMaxEnergyCut(99999.)
ana_showerquality.SetMinEnergyCut(0.)
ana_showerquality.setMCShowerQuality_multishowers(True)
#ana_processor.add_process(ana_showerquality)

# run!

print
print  "Finished configuring ana_processor. Start event loop!"
print

ana_processor.run()
sys.exit()

# make plots

if use_mc:
  plot_type = "mc"
else:
  plot_type = "reco"

n_showers_1 = []
n_showers_2 = []
n_shower_pairs = []

startpointdist = []
mass_inclusive = []
mass_exclusive = []

if use_mc:
  outfile = ROOT.TFile("pi0reco_ana_mc.root","read")
else:
  outfile = ROOT.TFile("pi0reco_ana_reco.root","read")
tree = outfile.Get("MassPeak")
for event in tree:

  n_showers_1.append(event.n_showers_1)

  for dist in event.start_point_distance:
    startpointdist.append(dist)

  for mass in event.mass_inclusive:
    mass_inclusive.append(mass)
  for mass in event.mass_exclusive:
    mass_exclusive.append(mass)

plt.hist(startpointdist, 10, range=[0,100], facecolor='green', alpha=0.75)

plt.xlabel('Distance between shower start points [cm]')
plt.ylabel('No. events')
plt.savefig("plots/startpointdist_{}.png".format(plot_type))

plt.clf()

hist_all = plt.hist(mass_inclusive, 20, facecolor='red', alpha=0.75, range=[0,200], label='All shower pairs')
hist_cut = plt.hist(mass_exclusive, 20, facecolor='blue', alpha=0.75, range=[0,200], label='Shower pairs after cuts')

if use_mc:
  plt.legend(loc='upper left')
else:
  plt.legend(loc='upper right')

plt.xlabel('two-shower invariant mass [MeV]')
plt.ylabel('No. candidates')
plt.savefig("plots/masspeak_{}.png".format(plot_type))

plt.clf()

plt.hist(n_showers_1, 10, facecolor='red', alpha=0.75, range=[0,10])

plt.xlabel('No. reconstructed showers')
plt.ylabel('No. events')
plt.savefig("plots/number_showers_{}.png".format(plot_type))


sys.exit()
