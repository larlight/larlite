#This is a list of plots that the user can choose from (in gen_ShowerQuality_plots)
#Each plot has a function that takes as input a dataframe, and returns a matplotlib figure.

try:
	import numpy as np
	import pandas as pd
	import matplotlib.pyplot as plt
	from sq_fitutils import *
except ImportError:
	raise ImportError('Something didn\'t import correctly! Do you have numpy, pandas, matplotlib, and root_numpy? Get it!')


def energy_asym_plot(df):
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
	return fig

def startpoint_acc_plot(df):
	fig = plt.figure(figsize=(10,6))
	plt.title('Distance Between (3D) MC Start Point and Reco Start Point')
	plt.xlabel('Distance in 3D [cm]')
	plt.ylabel('Counts')
	df['mc_reco_dist'].hist(bins=np.linspace(0,10,100))
	return fig

def anglediff_3d_plot(df):
	fig = plt.figure(figsize=(10,6))
	angular_res = df['mc_reco_anglediff']
	bins = np.linspace(0,180,100)
	plt.hist(angular_res,bins,edgecolor=None)
	plt.grid()
	plt.title('3D Direction MC - Reco angular difference')
	plt.xlabel('3D Angle Difference [degrees]')
	plt.ylabel('Counts')
	return fig

def dEdx_plot(df):
	fig = plt.figure(figsize=(10,6))
	dedx_u = df['reco_dedx_U']
	dedx_v = df['reco_dedx_V']
	dedx_y = df['reco_dedx_Y']
	bins = np.linspace(0,10,100)
	plt.hist(dedx_u,bins=bins,edgecolor=None,alpha=0.5,color='b',label='U Plane')
	plt.hist(dedx_v,bins=bins,edgecolor=None,alpha=0.5,color='g',label='V Plane')
	plt.hist(dedx_y,bins=bins,edgecolor=None,alpha=0.5,color='r',label='Y Plane')
	plt.grid()
	plt.legend(loc=2)
	plt.title('dE/dx [ MeV/cm ] in each Plane')
	plt.xlabel('dE/dx [ MeV / cm ]')
	plt.ylabel('Count')
	return fig