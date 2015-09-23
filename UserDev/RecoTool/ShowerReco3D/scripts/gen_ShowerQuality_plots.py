#Import various things to draw plots. No histograms or plots in root!
try:   
        import sys
        import os.path
        import numpy as np
        import pandas as pd
        import matplotlib.pyplot as plt
        import root_numpy
        from scipy.optimize import curve_fit
        from root_numpy import root2array
        from ShowerQualityUtils.sq_fitutils import *
        from ShowerQualityUtils.sq_plotdefs import *
except ImportError:
	raise ImportError('Something didn\'t import correctly! Do you have numpy, pandas, matplotlib, and root_numpy? Get it!')


#File that was output from ShowerQuality code including a ttree
if len(sys.argv) != 2:
        print "Usage: python %s $ROOT_FILE_OUTPUT_FROM_SHOWER_QUALITY" % sys.argv[0]
        quit()

#File that was output from ShowerQuality code including a ttree
fname = sys.argv[1] #'ShowerQuality_ana_out.root'
#Name of folder this script will create (or use, if exists) to put output image files in
output_directory = 'generated_plots'
#Name of the once-per-shower ttree from the ShowerQuality output root file
tshowername = 'fShowerTree'
#Name of the once-per-event ttree from the ShowerQuality output root file
teventname  = 'fEventTree'

if not os.path.isfile(fname):
	raise IOError('ShowerQuality output file does not exist: %s'%fname)

#Output plots will be saved into a "generated_plots" folder
if not os.path.isdir(output_directory):
        os.makedirs(output_directory)

#Turn the ShowerQuality output into a pandas dataframe like a BAMF
shower_df = pd.DataFrame(root2array(fname,tshowername))
event_df  = pd.DataFrame(root2array(fname,teventname) )

#Plot of energy resolution
fig = energy_res_plot(shower_df)
plt.savefig('%s/energy_res.png'%output_directory)
plt.show()

#Plot of energy asymmetry
fig = energy_res_plot(shower_df)
plt.savefig('%s/energy_res.png'%output_directory)
plt.show()

#2D histogram of reco vs true energy
for myplane in ['U','V','Y']:
        fig = energy_corr_plot(shower_df,plane='%s'%myplane)
        plt.savefig('%s/energy_correlation_%splane.png'%(output_directory,myplane))
        plt.show()

#Plot of start point accuracy
fig = startpoint_acc_plot(shower_df)
plt.savefig('%s/startpoint_accuracy.png'%output_directory)
plt.show()

# 3D direction angular resolution
fig = anglediff_3d_plot(shower_df)
plt.savefig('%s/3Ddir_accuracy.png'%output_directory)
plt.show()

# dEdx plots
fig = dEdx_plot(shower_df)
plt.savefig('%s/dEdx_resolution.png'%output_directory)
plt.show()

# reco efficiency
fig = reco_efficiency_vsenergy_plot(event_df)
plt.savefig('%s/reco_efficiency_vsenergy.png'%output_directory)
plt.show()

# Matched cluster efficiency
fig = cluster_efficiency_perplane_plot(shower_df)
plt.savefig('%s/cluster_efficiency_perplane.png'%output_directory)
plt.show()