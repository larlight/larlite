#This is a list of plots that the user can choose from (in gen_ShowerQuality_plots)
#Each plot has a function that takes as input a dataframe, and returns a matplotlib figure.

try:
	import numpy as np
	import pandas as pd
	import matplotlib.pyplot as plt
	from sq_fitutils import *
except ImportError:
	raise ImportError('Something didn\'t import correctly! Do you have numpy, pandas, matplotlib, and root_numpy? Get it!')


def energy_res_plot(df):
	#Plot of energy asymmetry
	fig = plt.figure(figsize=(10,6))
	plt.title("(MC Energy - Reco Energy)/MC Energy")
	bins = np.linspace(-0.9,0.9,100)
	curvebins = np.linspace(-0.9,0.9,300)
	eresU = (df['mc_energy']-df['reco_energy_U'])/df['mc_energy']
	eresV = (df['mc_energy']-df['reco_energy_V'])/df['mc_energy']
	eresY = (df['mc_energy']-df['reco_energy_Y'])/df['mc_energy']
	entriesU, bin_edgesU, patchesU = plt.hist(eresU,bins=bins,alpha=0.5,color='b',histtype='stepfilled')
	params, errors = getGaussFit(entriesU,bin_edgesU)
	plt.plot(curvebins,gauss(curvebins,*params),'b-',lw=3,label='U plane: mu = %.02f sigma = %.02f'%(params[1],params[2]))
	entriesV, bin_edgesV, patchesV = plt.hist(eresV,bins=bins,alpha=0.5,color='g',histtype='stepfilled')
	params, errors = getGaussFit(entriesV,bin_edgesV)
	plt.plot(curvebins,gauss(curvebins,*params),'g-',lw=3,label='V plane: mu = %.02f sigma = %.02f'%(params[1],params[2]))
	entriesY, bin_edgesY, patchesY = plt.hist(eresY,bins=bins,alpha=0.5,color='r',histtype='stepfilled')
	params, errors = getGaussFit(entriesY,bin_edgesY)
	plt.plot(curvebins,gauss(curvebins,*params),'r-',lw=3,label='Y plane: mu = %.02f sigma = %.02f'%(params[1],params[2]))
	plt.grid(True)
	plt.legend(loc=2,fontsize=12)
	return fig

def energy_asym_plot(df):
	#Plot of energy asymmetry
	fig = plt.figure(figsize=(10,6))
	plt.title("2*(MC Energy - Reco Energy)/(MC Energy + Reco Energy)")
	bins = np.linspace(-1,1,100)
	easymU = 2*(df['mc_energy']-df['reco_energy_U'])/(df['mc_energy']+df['reco_energy_U'])
	easymV = 2*(df['mc_energy']-df['reco_energy_V'])/(df['mc_energy']+df['reco_energy_V'])
	easymY = 2*(df['mc_energy']-df['reco_energy_Y'])/(df['mc_energy']+df['reco_energy_Y'])
	entriesU, bin_edgesU, patchesU = plt.hist(easymU,bins=bins,label='U',alpha=0.5,color='b',histtype='stepfilled')
	entriesV, bin_edgesV, patchesV = plt.hist(easymV,bins=bins,label='V',alpha=0.5,color='g',histtype='stepfilled')
	entriesY, bin_edgesY, patchesY = plt.hist(easymY,bins=bins,label='Y',alpha=0.5,color='r',histtype='stepfilled')
	plt.grid(True)
	plt.legend(loc=2)
	return fig

def energy_diff_plot(df):
	#Plot of energy asymmetry
	fig = plt.figure(figsize=(10,6))
	plt.title("Reco Energy - MC Energy")
	bins = np.linspace(-1000,1000,100)
	ediffU = df['reco_energy_U']-df['mc_energy']
	ediffV = df['reco_energy_V']-df['mc_energy']
	ediffY = df['reco_energy_Y']-df['mc_energy']
	entriesU, bin_edgesU, patchesU = plt.hist(ediffU,bins=bins,label='U',alpha=0.5,color='b',histtype='stepfilled')
	entriesV, bin_edgesV, patchesV = plt.hist(ediffV,bins=bins,label='V',alpha=0.5,color='g',histtype='stepfilled')
	entriesY, bin_edgesY, patchesY = plt.hist(ediffY,bins=bins,label='Y',alpha=0.5,color='r',histtype='stepfilled')
	plt.grid(True)
	plt.legend(loc=2)
	return fig

def energy_corr_plot(df,plane='Y'):
	#Plot of reco energy vs mc energy (2d histo)
	if plane not in ['U','V','Y']:
		raise ValueError('energy_corr_plot() function requires plane that is either \'U\', \'V\', or \'Y\'')

	# fig = plt.figure(figsize=(10,6))
        mc_energy = df['mc_energy'].ravel()
        reco_energy = df['reco_energy_%s'%plane].ravel()
        fig = plt.figure(figsize=(10,10))
        plt.hist2d(mc_energy,reco_energy,bins=50)
	plt.xlabel('MCShower Deposited Energy',fontsize=14)
	plt.ylabel('Reconstructed Energy',fontsize=14)
        plt.title('MC Energy vs. Reco Energy')
	return fig

def startpoint_acc_plot(df):
	fig = plt.figure(figsize=(10,6))
	xmin, xmax, nbins = 0., 100., 100
	plt.title('Distance Between (3D) MC Start Point and Reco Start Point')
	plt.xlabel('Distance in 3D [cm]')
	plt.ylabel('Counts')
	# plt.yscale('log')
	n_total = len(df['mc_reco_dist'])
	n_underflow = len(df.query('mc_reco_dist < %f'%xmin))
	n_overflow  = len(df.query('mc_reco_dist > %f'%xmax))
	df['mc_reco_dist'].hist(bins=np.linspace(xmin, xmax, nbins),label='Total entries: %d (%d underflow, %d overflow)\nMean = %0.3f, Std = %0.3f'%(n_total,n_underflow,n_overflow,df['mc_reco_dist'].mean(),df['mc_reco_dist'].std()))
	plt.legend(loc=1)
	return fig

def startpoint_resolution_plot(df):
	fig = plt.figure(figsize=(10,6))
	xmin, xmax, nbins = 0., 100., 100
	plt.title('[MC-Reco] Start Point Distance on each Plane')
	plt.xlabel('Distance in 1D [cm]')
        x_res = df['mc_x']-df['reco_x']
        y_res = df['mc_y']-df['reco_y']
        z_res = df['mc_z']-df['reco_z']
	n_total = len(df['mc_reco_dist'])
	n_underflow = len(df.query('mc_reco_dist < %f'%xmin))
	n_overflow  = len(df.query('mc_reco_dist > %f'%xmax))
        bins = np.linspace(-1.5,1.5,100)
        curvebins = np.linspace(-1.5,1.5,100)
	entriesX, bin_edgesX, patchesX = plt.hist(x_res,bins=bins,alpha=0.5,color='b',histtype='stepfilled')
	params, errors = getGaussFit(entriesX,bin_edgesX)
	plt.plot(curvebins,gauss(curvebins,*params),'b-',lw=2,label='X coord: mu = %.02f sigma = %.02f'%(params[1],params[2]))
	entriesY, bin_edgesY, patchesY = plt.hist(y_res,bins=bins,alpha=0.5,color='r',histtype='stepfilled')
	params, errors = getGaussFit(entriesY,bin_edgesY)
	plt.plot(curvebins,gauss(curvebins,*params),'r-',lw=2,label='Y coord: mu = %.02f sigma = %.02f'%(params[1],params[2]))
	entriesZ, bin_edgesZ, patchesZ = plt.hist(z_res,bins=bins,alpha=0.5,color='g',histtype='stepfilled')
	params, errors = getGaussFit(entriesZ,bin_edgesZ)
	plt.plot(curvebins,gauss(curvebins,*params),'g-',lw=2,label='Z coord: mu = %.02f sigma = %.02f'%(params[1],params[2]))
        plt.grid()
	plt.legend(loc=1,fontsize=12)
	return fig

def anglediff_3d_plot(df,maxangle=180):
	fig = plt.figure(figsize=(10,6))
	xmin, xmax, nbins =0., maxangle, 100
	angular_res = df['mc_reco_anglediff']
	bins = np.linspace(xmin, xmax, nbins)
	plt.grid(True)
	plt.title('3D Direction MC - Reco Angular Difference')
	plt.xlabel('3D Angle Difference [degrees]')
	plt.ylabel('Counts')
	n_total = len(df['mc_reco_anglediff'])
	n_underflow = len(df.query('mc_reco_anglediff < %f'%xmin))
	n_overflow  = len(df.query('mc_reco_anglediff > %f'%xmax))
	plt.hist(angular_res,bins,histtype='stepfilled',label='Total entries: %d (%d underflow, %d overflow)\nMean = %0.3f, Std = %0.3f'%(n_total,n_underflow,n_overflow,angular_res.mean(),angular_res.std()))
	plt.legend(loc=1)
	# plt.yscale('log')
	return fig

def dEdx_plot(df):
	fig = plt.figure(figsize=(10,6))
	dedx_u = df['reco_dedx_U']
	dedx_v = df['reco_dedx_V']
	dedx_y = df['reco_dedx_Y']
	bins = np.linspace(0,10,100)
	curvebins = np.linspace(1.5,3.5,300)
	# entriesU, bin_edgesU, patchesU = plt.hist(eresU,bins=bins,label='U',alpha=0.5,color='b',histtype='stepfilled')
	# params, errors = getGaussFit(entriesU,bin_edgesU)
	entriesU, bin_edgesU, patchesU = plt.hist(dedx_u,bins=bins,histtype='stepfilled',alpha=0.5,color='b',label='U Plane')
	entriesV, bin_edgesV, patchesV = plt.hist(dedx_v,bins=bins,histtype='stepfilled',alpha=0.5,color='g',label='V Plane')
	entriesY, bin_edgesY, patchesY = plt.hist(dedx_y,bins=bins,histtype='stepfilled',alpha=0.5,color='r',label='Y Plane')
	#params, errors = getGaussFit(entriesY,bin_edgesY)
	#plt.plot(curvebins,gauss(curvebins,*params),'r-',lw=3,label='Y plane: mu = %.02f sigma = %.02f'%(params[1],params[2]))
	plt.grid(True)
	plt.legend(loc=1)
	plt.title('dE/dx [ MeV/cm ] in each Plane [Electrons Only]')
	plt.xlabel('dE/dx [ MeV / cm ]')
	plt.ylabel('Count')
	return fig

def dQdx_plot(df):
	fig = plt.figure(figsize=(10,6))
	dqdx_u = df['reco_dqdx_U']
	dqdx_v = df['reco_dqdx_V']
	dqdx_y = df['reco_dqdx_Y']
	bins = np.linspace(0,200000,100)
	# curvebins = np.linspace(44000,76000,100)
	entriesU, bin_edgesU, patchesU = plt.hist(dqdx_u,bins=bins,histtype='stepfilled',alpha=0.5,color='b',label='U Plane')
	entriesV, bin_edgesV, patchesV = plt.hist(dqdx_v,bins=bins,histtype='stepfilled',alpha=0.5,color='g',label='V Plane')
	entriesY, bin_edgesY, patchesY = plt.hist(dqdx_y,bins=bins,histtype='stepfilled',alpha=0.5,color='r',label='Y Plane')
	# params, errors = getGaussFit(entriesY,bin_edgesY)
	# plt.plot(curvebins,gauss(curvebins,*params),'r-',lw=3,label='Y plane: mu = %.02f sigma = %.02f'%(params[1],params[2]))
	plt.grid(True)
	plt.legend(loc=1)
	plt.title('dQ/dx [ fC/cm ] in each Plane [Electrons Only]')
	plt.xlabel('dQ/dx [ fC / cm ]')
	plt.ylabel('Count')
	return fig


def reco_efficiency_vsenergy_singleshower_plot(df):
	fig = plt.figure(figsize=(10,6))
	plt.grid(True)
	binmin, binmax, binwidth = 0, 1000, 100
	bins = np.arange(binmin, binmax, binwidth)
	effs_x = np.arange(binmin + (binwidth/2), binmax-binwidth, binwidth)
	effs_y = np.zeros(len(effs_x))
	errs_y = np.zeros(len(effs_x))
	for i in xrange(len(bins)-1):
		this_df = df.query('mcs_E > %d and mcs_E < %d' % (bins[i], bins[i+1]))
                oneshower_df = this_df.query('n_recoshowers == 1')
                effs_y[i] = float(oneshower_df.shape[0])/this_df.shape[0]
		#effs_y[i] = float(np.sum(this_df['n_recoshowers']))/np.sum(this_df['n_mcshowers'])
                errs_y[i] = np.sqrt(effs_y[i]*(1-effs_y[i])/this_df.shape[0])#np.sum(this_df['n_mcshowers']))
        plt.errorbar(effs_x,effs_y, yerr=errs_y, color='blue', linestyle = ':', marker='*', mfc='blue', mec='blue', label='Shower Reco Efficiency')
	plt.legend(loc=4)
	plt.title('Shower Reconstruction Efficiency vs True Shower Deposited Energy')
	plt.xlabel('True Shower Deposited Energy [MeV]')
	plt.ylabel('Frac. of Events w/ One Reconstructed Shower')
	return fig

def reco_efficiency_vsenergy_goodangle_plot(df):
	fig = plt.figure(figsize=(10,6))
	plt.grid(True)
	binmin, binmax, binwidth = 0, 1000, 100
	bins = np.arange(binmin, binmax, binwidth)
	effs_x = np.arange(binmin + (binwidth/2), binmax-binwidth, binwidth)
	effs_y = np.zeros(len(effs_x))
	errs_y = np.zeros(len(effs_x))
	for i in xrange(len(bins)-1):
		this_df = df.query('mc_energy > %d and mc_energy < %d' % (bins[i], bins[i+1]))
                gooddir_df = this_df.query('mc_reco_anglediff < 10')
                effs_y[i] = float(gooddir_df.shape[0])/this_df.shape[0]
		#effs_y[i] = float(np.sum(this_df['n_recoshowers']))/np.sum(this_df['n_mcshowers'])
                errs_y[i] = np.sqrt(effs_y[i]*(1-effs_y[i])/this_df.shape[0])#np.sum(this_df['n_mcshowers']))
        plt.errorbar(effs_x,effs_y, yerr=errs_y, color='blue', linestyle = ':', marker='*', mfc='blue', mec='blue', label='Shower Reco Efficiency')
	plt.legend(loc=4)
	plt.title('Shower Reconstruction Efficiency vs True Shower Deposited Energy')
	plt.xlabel('True Shower Deposited Energy [MeV]')
	plt.ylabel('Frac. of Events w/ < 10 degree Angle Res.')
	return fig


def startpoint_resolution_vsenergy_plot(df):
	fig = plt.figure(figsize=(10,6))
	binmin, binmax, binwidth = 0, 1000, 100
	bins = np.arange(binmin, binmax, binwidth)
	energy_bins = np.arange(binmin + (binwidth/2), binmax-binwidth, binwidth)
	pos_resolution     = np.zeros(len(energy_bins))
	for i in xrange(len(bins)-1):
                this_df = df.query('mc_energy > %d and mc_energy < %d' % (bins[i], bins[i+1]))
                x_res = this_df['mc_x']-this_df['reco_x']
                y_res = this_df['mc_y']-this_df['reco_y']
                z_res = this_df['mc_z']-this_df['reco_z']
                pos_bins = np.linspace(-1.5,1.5,100)
                entriesX, bin_edgesX = np.histogram(x_res,bins=pos_bins)
                params, errors = getGaussFit(entriesX,bin_edgesX)
                xres = params[2]
                entriesY, bin_edgesY = np.histogram(y_res,bins=pos_bins)
                params, errors = getGaussFit(entriesY,bin_edgesY)
                yres = params[2]
                entriesZ, bin_edgesZ = np.histogram(z_res,bins=pos_bins)
                params, errors = getGaussFit(entriesZ,bin_edgesZ)
                zres = params[2]
                res = np.sqrt( xres*xres + yres*yres + zres*zres )
                pos_resolution[i] = res
        plt.plot(energy_bins,pos_resolution,'bo--')
        plt.grid()
        plt.ylim([0,1])
	plt.legend(loc=1,fontsize=12)
        plt.xlabel('True Shower Deposited Energy [MeV]')
        plt.ylabel('Position Resolution [cm]')
        plt.title('Shower 3D Start Point Resolution vs. Shower Energy')
	return fig


def eres_vsenergy_plot(df):

	fig = plt.figure(figsize=(10,6))
	binmin, binmax, binwidth = 0, 1000, 100
	bins = np.arange(binmin, binmax, binwidth)
	resolution_E = np.arange(binmin + (binwidth/2), binmax-binwidth, binwidth)
        #resolution_E = 0.5*(resolution_E[1:]+resolution_E[:-1])
	resolution_U = np.zeros(len(resolution_E))
	resolution_V = np.zeros(len(resolution_E))
	resolution_Y = np.zeros(len(resolution_E))
        resbins = np.linspace(-0.9,0.9,25)
	for i in xrange(len(bins)-1):
                this_df = df.query('mc_energy > %d and mc_energy < %d' % (bins[i], bins[i+1]))
                eresU = (this_df['mc_energy']-this_df['reco_energy_U'])/this_df['mc_energy']
                eresV = (this_df['mc_energy']-this_df['reco_energy_V'])/this_df['mc_energy']
                eresY = (this_df['mc_energy']-this_df['reco_energy_Y'])/this_df['mc_energy']
                entries, bin_edges = np.histogram(eresU,bins=resbins)
                bin_centers = 0.5*(bin_edges[1:]+bin_edges[:-1])
                params, errors = getGaussFit(entries,bin_edges)
                errors = np.sqrt(np.diag(errors))
                resolution_U[i] = params[2]
                entries, bin_edges = np.histogram(eresV,bins=resbins)
                bin_centers = 0.5*(bin_edges[1:]+bin_edges[:-1])
                params, errors = getGaussFit(entries,bin_edges)
                errors = np.sqrt(np.diag(errors))
                resolution_V[i] = params[2]
                entries, bin_edges = np.histogram(eresY,bins=resbins)
                bin_centers = 0.5*(bin_edges[1:]+bin_edges[:-1])
                params, errors = getGaussFit(entries,bin_edges)
                errors = np.sqrt(np.diag(errors))
                resolution_Y[i] = params[2]
	plt.plot(resolution_E,resolution_U,'o--',color='b',label='U Plane')
	plt.plot(resolution_E,resolution_V,'o--',color='g',label='V Plane')
	plt.plot(resolution_E,resolution_Y,'o--',color='r',label='Y Plane')
	plt.grid(True)
	plt.legend(loc=2,fontsize=12)
        plt.xlabel('Truth Deposited Energy [MeV]')
        plt.ylabel('Energy Resolution Fraction')
        plt.title('Per-Plane Energy Resoluton vs. Energy')
        plt.ylim([0,0.3])
	return fig



def angleres_vsenergy_plot(df):

	fig = plt.figure(figsize=(10,6))
	binmin, binmax, binwidth = 0, 1000, 100
	bins = np.arange(binmin, binmax, binwidth)
	resolution_E = np.arange(binmin + (binwidth/2), binmax-binwidth, binwidth)
        #resolution_E = 0.5*(resolution_E[1:]+resolution_E[:-1])
	angle_res = np.zeros(len(resolution_E))
        resbins = np.linspace(0,10,100)
	for i in xrange(len(bins)-1):
                this_df = df.query('mc_energy > %d and mc_energy < %d' % (bins[i], bins[i+1]))
                angle_vals = this_df['mc_reco_anglediff'].ravel()
                entries, bin_edges = np.histogram(angle_vals,bins=resbins)
                bin_centers = 0.5*(bin_edges[1:]+bin_edges[:-1])
                max_val = np.argmax(entries)
                angle_resolution = bin_centers[max_val]
                angle_res[i] = angle_resolution
	plt.plot(resolution_E,angle_res,'o--',color='b')
	plt.grid(True)
	plt.legend(loc=2,fontsize=12)
        plt.xlabel('Truth Deposited Energy [MeV]')
        plt.ylabel('Angular Resolution [degrees]')
        plt.title('3D Axis Angular Resoluton vs. Energy')
        plt.ylim([0,10])
	return fig



def cluster_efficiency_perplane_plot(df):
	fig = plt.figure(figsize=(10,6))
	cluseff_u = df['cluster_eff_U']
	cluseff_v = df['cluster_eff_V']
	cluseff_y = df['cluster_eff_Y']
	bins = np.linspace(0,1,100)
	plt.hist(cluseff_u,bins=bins,histtype='stepfilled',alpha=0.5,color='b',label='U Plane')
	plt.hist(cluseff_v,bins=bins,histtype='stepfilled',alpha=0.5,color='g',label='V Plane')
	plt.hist(cluseff_y,bins=bins,histtype='stepfilled',alpha=0.5,color='r',label='Y Plane')
	plt.grid(True)
	plt.legend(loc=1)
	plt.title('Shower\'s Associated Cluster\'s Efficiency in each Plane')
	plt.xlabel('Cluster Summed Hit Charge / Total Summed Hit Charge, per Plane')
	plt.ylabel('Counts')
	return fig

def length_plot(df):
	fig = plt.figure(figsize=(10,6))
	xmin, xmax, nbins = 0., 400., 100
	plt.title('Reconstructed Length of Shower')
	plt.xlabel('Length in 3D [cm]')
	plt.ylabel('Counts')
	n_total = len(df['reco_length'])
	n_underflow = len(df.query('reco_length < %f'%xmin))
	n_overflow  = len(df.query('reco_length > %f'%xmax))
	df['reco_length'].hist(bins=np.linspace(xmin, xmax, nbins),label='Total entries: %d (%d underflow, %d overflow)'%(n_total,n_underflow,n_overflow))
	plt.legend(loc=1)
	return fig

def length_energy_corr_plot(df,plane='Y'):
	#Plot of reco length vs reco energy (2d histo)
	if plane not in ['U','V','Y']:
		raise ValueError('energy_corr_plot() function requires plane that is either \'U\', \'V\', or \'Y\'')

	# fig = plt.figure(figsize=(10,6))
        reco_length = df['reco_length'].ravel()
        reco_energy = df['reco_energy_%s'%plane].ravel()
        fig = plt.figure(figsize=(10,10))
        plt.hist2d(reco_length,reco_energy,bins=50)
	plt.xlabel('Reconstructed Energy')
	plt.ylabel('Reconstructed 3D Shower Length [cm]')
        plt.title('Reco Length vs. Reco Energy')
	return fig
