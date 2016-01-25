import ROOT
import matplotlib.pyplot as plt
from root_numpy import root2array, root2rec, tree2rec, array2root
import pandas as pd
import numpy as np
import sys,os

cut = 0.8

txtfile = open(sys.argv[1])

fig, axarr = plt.subplots(2,3,figsize=(15,10))

producers = []
efficiencies = [ [], [], [] ]
purities     = [ [], [], [] ]

for line in txtfile:

    fname = line.split()[0]
    f = ROOT.TFile(fname)
    t = f.Get("fMMQualityTree")

    cluster_producer = fname.split("_")[3]
    producers.append(cluster_producer)
    print 'examining producer %s'%cluster_producer
    
    t_py = tree2rec(t)

    eff = [ [], [], [] ]
    pur = [ [], [], [] ]

    high_eff = [ [], [], [] ]
    high_pur = [ [], [], [] ]

    for n in xrange(len(t_py)):
        
        for pl in xrange(3):

            # efficiency
            for i in xrange(len(t_py['BestEff_%i'%pl][n])):
                eff_val = t_py['BestEff_%i'%pl][n][i]
                eff[pl].append(eff_val)
                if (eff_val > cut) : high_eff[pl].append(eff_val)
            # purity
            for i in xrange(len(t_py['BestPur_%i'%pl][n])):
                pur_val = t_py['BestPur_%i'%pl][n][i]
                pur[pl].append(pur_val)
                if (pur_val > cut) : high_pur[pl].append(pur_val)

    eff_pl0 = float(len(high_eff[0])) / len(eff[0])
    eff_pl1 = float(len(high_eff[1])) / len(eff[1])
    eff_pl2 = float(len(high_eff[2])) / len(eff[2])
    pur_pl0 = float(len(high_pur[0])) / len(pur[0])
    pur_pl1 = float(len(high_pur[1])) / len(pur[1])
    pur_pl2 = float(len(high_pur[2])) / len(pur[2])

    efficiencies[0].append(eff_pl0)
    print 'eff[0] = %.02f'%eff_pl0
    efficiencies[1].append(eff_pl1)
    print 'eff[1] = %.02f'%eff_pl1
    efficiencies[2].append(eff_pl2)
    print 'eff[2] = %.02f'%eff_pl2

    purities[0].append(pur_pl0)
    print 'pur[0] = %.02f'%pur_pl0
    purities[1].append(pur_pl1)
    print 'pur[1] = %.02f'%pur_pl1
    purities[2].append(pur_pl2)
    print 'pur[2] = %.02f'%pur_pl2

num_producers = len(producers)

axarr[0][0].set_ylabel('Merging Efficiency [FOM]')
axarr[1][0].set_ylabel('Merging Purity [FOM]')

major_ticks = np.linspace(0,num_producers-1,num_producers)
print major_ticks

for x in xrange(2):
    for y in xrange(3):
        
        if (x == 0):
            axarr[x][y].set_title('Efficiency Pl %i'%y)
            axarr[x][y].plot(efficiencies[y],'bo-')
        if (x == 1):
            axarr[x][y].set_title('Purity Pl %i'%y)
            axarr[x][y].plot(purities[y],'bo-')
        axarr[x][y].set_xticks(major_ticks)
        axarr[x][y].set_ylim([0,1])
        axarr[x][y].set_xlim(-1,num_producers)
        axarr[x][y].grid()
        labels = [item.get_text() for item in axarr[x][y].get_xticklabels()]
        for i in xrange(len(labels)):
            labels[i] = producers[i]
        axarr[x][y].set_xticklabels(labels,rotation=45)

plt.tight_layout()
plt.show()
    
    

    
