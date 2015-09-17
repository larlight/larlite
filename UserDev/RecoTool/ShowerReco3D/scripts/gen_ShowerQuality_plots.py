#Import various things to draw plots. No histograms or plots in root!
try:
	import numpy as np
	import pandas as pd
	import matplotlib.pyplot as plt
	import root_numpy
        from scipy.optimize import curve_fit
	from root_numpy import root2array
except ImportError:
	raise ImportError('Something didn\'t import correctly! Do you have numpy, pandas, matplotlib, and root_numpy? Get it!')

import os.path

def gauss(x,A,mu,sigma):

        return A/(sigma*np.sqrt(2*np.pi)) * np.exp(-((x-mu)/sigma)**2)

def getGaussFit(entries,bin_edges):

        bin_centers = 0.5*(bin_edges[1:] + bin_edges[:-1])

        # guess should be, for the area the number of entris
        # and for the mean the bin with the highest num. of entries
        ntot = np.sum(entries)
        median = bin_centers[np.argmax(entries)]
        parameters, cov_matrix = curve_fit(gauss, bin_centers, entries, p0=[ntot,median,0.2]) 

        return parameters,cov_matrix

#File that was output from ShowerQuality code including a ttree
fname = 'ShowerQuality_ana_out.root'
#Name of the ttree from the ShowerQuality output root file
tname = 'fShowerQualityTree'

if not os.path.isfile(fname):
	raise IOError('ShowerQuality output file does not exist: %s'%fname)

#Turn the ShowerQuality output into a pandas dataframe like a BAMF
df = pd.DataFrame(root2array(fname,tname))

#Plot of energy asymmetry
fig = plt.figure(figsize=(10,6))
plt.title("(MC Energy - Reco Energy)/MC Energy")
bins = np.linspace(-1,1,100)
curvebins = np.linspace(-1,1,300)
eresU = (df['mc_energy']-df['reco_energy_U'])/df['mc_energy']
eresV = (df['mc_energy']-df['reco_energy_V'])/df['mc_energy']
eresY = (df['mc_energy']-df['reco_energy_Y'])/df['mc_energy']
entriesU, bin_edgesU, patchesU = plt.hist(eresU,bins=bins,label='U',alpha=0.5,color='b',edgecolor=None)
params, errors = getGaussFit(entriesU,bin_edgesU)
plt.plot(curvebins,gauss(curvebins,*params),'b-',lw=3,label='U plane: mu = %.02f sigma = %.02f'%(params[1],params[2]))
entriesV, bin_edgesV, patchesV = plt.hist(eresV,bins=bins,label='V',alpha=0.5,color='g',edgecolor=None)
params, errors = getGaussFit(entriesV,bin_edgesV)
plt.plot(curvebins,gauss(curvebins,*params),'g-',lw=3,label='V plane: mu = %.02f sigma = %.02f'%(params[1],params[2]))
entriesY, bin_edgesY, patchesY = plt.hist(eresY,bins=bins,label='Y',alpha=0.5,color='r',edgecolor=None)
params, errors = getGaussFit(entriesY,bin_edgesY)
plt.plot(curvebins,gauss(curvebins,*params),'r-',lw=3,label='Y plane: mu = %.02f sigma = %.02f'%(params[1],params[2]))
plt.grid()
plt.legend(loc=2)
plt.savefig('energy_asym.pdf')
plt.show()

#Plot of start point accuracy
fig = plt.figure(figsize=(10,6))
plt.title('Distance Between (3D) MC Start Point and Reco Start Point')
plt.xlabel('Distance in 3D [cm]')
plt.ylabel('Counts')
df['mc_reco_dist'].hist(bins=np.linspace(0,10,100))
plt.savefig('startpoint_accuracy.pdf')
plt.show()

# 3D direction angular resolution
fig = plt.figure(figsize=(10,6))
angular_res = df['mc_reco_anglediff']
bins = np.linspace(0,180,100)
plt.hist(angular_res,bins,edgecolor=None)
plt.grid()
plt.xlabel('3D Direction MC - Reco angular difference')
plt.ylabel('Counts')
plt.savefig('3Ddir_accuracy.pdf')
plt.show()
