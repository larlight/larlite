import sys,ROOT,os

from matplotlib.colors import LogNorm
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.colors as mpc
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

import scipy
from scipy import interpolate
import pandas as pd
import itertools

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

import ROOT
#ROOT.gSystem.Load("libOpFlashAna_OpT0FinderApp")
from larlite import larlite as fmwk
from larlite import larutil
from ROOT import flashana,TChain

# Create ana_processor instance
my_proc = fmwk.ana_processor()

mcflash_ch=TChain("opflash_mcflash_tree")
cheater_ch=TChain("opflash_cheatFlashCosmic_tree")
# Set input root file
for x in range(len(sys.argv)):
    fname = sys.argv[x]
    if not fname.endswith('.root'): continue
    my_proc.add_input_file(fname)
    mcflash_ch.AddFile(fname)
    cheater_ch.AddFile(fname)

if not mcflash_ch.GetEntries():
    print('OpFlash w/ name mcflash not found...')
    sys.exit(1)
if not cheater_ch.GetEntries():
    print('OpFlash w/ name cheater not found...')
    sys.exit(1)

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("ana.root")

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_unit = fmwk.UBT0Finder()
my_proc.add_process(my_unit)
my_unit.SetConfigFile("%s/SelectionTool/OpT0Finder/App/mac/flashmatch.fcl" % os.environ['LARLITE_USERDEVDIR'])

print()
print("Finished configuring ana_processor. Start event loop!")
print()
ctr=0
geo=larutil.Geometry.GetME()
while ctr < mcflash_ch.GetEntries():

    my_proc.process_event()

    match_v = my_unit.MatchResult()
    
    mgr = my_unit.Manager()

    qcluster_v = mgr.QClusterArray()

    flash_v = mgr.FlashArray()

    print(match_v.size(),'match found for',qcluster_v.size(),'tpc objects and',flash_v.size(),'pmt objects')

    if not (match_v.size() == 1 and qcluster_v.size() == 1):
        print('Skipping (match size & qcluster size !=1)')
        ctr += 1
        continue

    mcflash_ch.GetEntry(ctr)
    cheater_ch.GetEntry(ctr)
    mcflash_v = mcflash_ch.opflash_mcflash_branch
    cheater_v = cheater_ch.opflash_cheatFlash_branch
    if not (mcflash_v.size() == 1 and cheater_v.size() == 1):
        print('Skipping (mcflash & chetar size !=1)')
        ctr += 1
        continue

    mcflash=[0]*geo.NOpDets()
    for x in range(geo.NOpDets()):
        mcflash[geo.OpDetFromOpChannel(x)] = mcflash_v[0].PE(x)
    mcflash = np.array(mcflash)

    cheater=[0]*geo.NOpDets()
    for x in range(geo.NOpDets()):
        cheater[geo.OpDetFromOpChannel(x)] = cheater_v[0].PE(x)
    cheater=np.array(cheater)

    match = match_v[0]
    print('TPC:',match.tpc_id,'PMT:',match.flash_id,'Score:',match.score,'TPC point:',match.tpc_point.x,match.tpc_point.y,match.tpc_point.z)

    xmin = 1e4
    xmax = 0
    qc = qcluster_v[match.tpc_id]
    for pt_idx in range(qc.size()):
        pt = qc[pt_idx]
        if pt.x > xmax: xmax = pt.x
        if pt.x < xmin: xmin = pt.x

    #print 'xmin:',xmin,'xmax:',xmax
    flash = flash_v[match.flash_id]
    flash = np.array(flash.pe_v)

    print('x:',match.tpc_point.x,'qll:',match.score)
    print('xmin:',xmin,'xmax:',xmax)

    hypothesis = np.array(match.hypothesis)

    ymax = np.array((flash.max(),hypothesis.max(),mcflash.max(),cheater.max())).max()

    fig, ax = plt.subplots(figsize=(12, 8),facecolor='w')

    plt.plot(np.arange(0,len(flash),1),flash,
             marker='o',color='blue',label='OpFlash PE Spec.')
    
    plt.plot(np.arange(0,len(hypothesis),1), hypothesis,
             marker='o',color='red',label='Hypothesis PE Spec.')

    plt.plot(np.arange(0,len(mcflash),1), mcflash,
             marker='o',color='black',label='MCFlash PE Spec.')

    plt.plot(np.arange(0,len(cheater),1), cheater,
             marker='o',color='orange',label='CheatFlash PE Spec.')


    plt.ylim(0,ymax*1.5)
    plt.tick_params(labelsize=20)
    plt.grid()
    plt.ylabel('PE Count',fontsize=20,fontweight='bold')
    plt.xlabel('PMT (OpDet ID)',fontsize=20,fontweight='bold')
    leg=plt.legend(fontsize=20,loc=1)
    leg_frame=leg.get_frame()
    leg_frame.set_facecolor('white')
    plt.title('Flash PE Distribution',fontsize=24,fontweight='bold')
    plt.show()    

    sys.stdin.readline()

    ctr+=1

# done!
print()
print("Finished running ana_processor event loop!")
print()

sys.exit(0)
