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
    msg += "Usage 1: %s PRODUCER $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
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

# Set input root file
cfg_file=''
for f in sys.argv:
    if f.endswith('.root'):
        my_proc.add_input_file(f)
        continue
    if f.endswith('.cfg') or f.endswith('.fcl'):
        cfg_file=f
# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("ana.root")

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_unit = fmwk.MuCST0Finder()
#my_unit.SetEDiff(10.)
my_proc.add_process(my_unit)
#my_unit.ShiftFlashTime(-500)

# TPC Filter Algo
#my_unit.Manager().SetAlgo(flashana.NPtFilter())
# PMT Filter Algo
#my_unit.Manager().SetAlgo(flashana.MaxNPEWindow())
# Match Prohibit Algo
#my_unit.Manager().SetAlgo(flashana.TimeCompatMatch())
# Hypothesis Algo
#my_unit.Manager().SetAlgo(flashana.PhotonLibHypothesis())
# Match Algo
#algo = flashana.QLLMatch.GetME()
#algo = flashana.QWeightPoint()
#algo = flashana.CommonAmps()
#my_unit.Manager().SetAlgo( algo )

# Custom Algo
#my_unit.Manager().SetAlgo( flashana.LightPath()  )

# producer
my_unit.SetConfigFile(cfg_file)

print()
print("Finished configuring ana_processor. Start event loop!")
print()
ctr=0
max_ctr=20
geo=larutil.Geometry.GetME()
while ctr<max_ctr:

    print()
    print('Entry',ctr,'processing...')
    my_proc.process_event(ctr)

    match_v = my_unit.MatchResult()
    
    mgr = my_unit.Manager()

    qcluster_v = mgr.QClusterArray()

    flash_v = mgr.FlashArray()

    ctr+=1
    print(match_v.size(),'match found for',qcluster_v.size(),'tpc objects and',flash_v.size(),'pmt objects')

    # full result
    fullres_tpc_flash_v = mgr.FullResultTPCFlash()
    fullres_flash_tpc_v = mgr.FullResultFlashTPC()

    print(qcluster_v.size(), flash_v.size(), fullres_tpc_flash_v.size())

    if qcluster_v.empty() or flash_v.empty(): continue

    # report the right matching
    if fullres_tpc_flash_v.size():

        for res in fullres_tpc_flash_v[0]:
            if res.flash_id >= flash_v.size(): continue
            flash = flash_v[res.flash_id]
            qc = qcluster_v[0]
            xmin = 1e4
            xmax = 0
            for pt_idx in range(qc.size()):
                pt = qc[pt_idx]
                if pt.x > xmax: xmax = pt.x
                if pt.x < xmin: xmin = pt.x
            if flash.time < -1.5 or -0.5 < flash.time:
                continue

            print('Right match parameters:')
            print('TPC: 0 ... PMT:',res.flash_id)
            print('TPC MinX:',xmin)
            print('PMT MinX:',res.tpc_point.x)
            print('Match Score:',res.score)
            print('Flash Time:',flash.time)
        
            flash = np.array(flash.pe_v)
            hypothesis = np.array(res.hypothesis)
            
            ymax = np.array((flash.max(),hypothesis.max())).max()

            fig, ax = plt.subplots(figsize=(12, 8),facecolor='w')
            
            ax.plot(np.arange(0,len(flash),1),flash,
                    marker='o',color='blue',label='OpFlash PE Spec.')
        
            ax.plot(np.arange(0,len(hypothesis),1), hypothesis,
                    marker='o',color='red',label='Hypothesis PE Spec.')
            
            plt.ylim(0,ymax*1.5)
            plt.tick_params(labelsize=20)
            plt.grid()
            plt.ylabel('PE Count',fontsize=20,fontweight='bold')
            #plt.set_title('PMT (OpDet ID)',fontsize=20,fontweight='bold')
            leg=plt.legend(fontsize=20,loc=1)
            leg_frame=leg.get_frame()
            leg_frame.set_facecolor('white')
            plt.title('Flash PE Distribution',fontsize=24,fontweight='bold')
            plt.xlim(-0.5,31.5)
            plt.show()
            break
            
    for match_idx in range(match_v.size()):

        match = match_v[match_idx]
        if 'onlymucs' in sys.argv and not match.tpc_id == 0:
            continue
            
        print()
        print('Match #',match_idx, end=' ')
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
        print('TPC MinX:',xmin,'TPC MaxX:',xmax)
        print('PMT MinX:',match.tpc_point.x,'qll:',match.score,'t0',flash.time)
        flash = np.array(flash.pe_v)

        print('Max PE @',flash.argmax(),'fraction',flash[flash.argmax()]/flash.sum())
        
        hypothesis = np.array(match.hypothesis)

        ymax = np.array((flash.max(),hypothesis.max())).max()

        fig, ax = plt.subplots(figsize=(12, 8),facecolor='w')

        ax.plot(np.arange(0,len(flash),1),flash,
                marker='o',color='blue',label='OpFlash PE Spec.')
    
        ax.plot(np.arange(0,len(hypothesis),1), hypothesis,
                marker='o',color='red',label='Hypothesis PE Spec.')

        plt.ylim(0,ymax*1.5)
        ax.tick_params(labelsize=20)
        ax.grid()
        plt.ylabel('PE Count',fontsize=20,fontweight='bold')
        plt.xlabel('PMT (OpDet ID)',fontsize=20,fontweight='bold')
        leg=plt.legend(fontsize=20,loc=1)
        leg_frame=leg.get_frame()
        leg_frame.set_facecolor('white')
        plt.title('Flash PE Distribution',fontsize=24,fontweight='bold')
        plt.xlim(-0.5,31.5)
        plt.show()
        
        #sys.stdin.readline()
    print('Type event entry or hit enter')
    entry=sys.stdin.readline().rstrip('\n')
    if entry: ctr=int(entry)
    
# done!
print()
print("Finished running ana_processor event loop!")
print()

sys.exit(0)
