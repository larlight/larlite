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
if len(sys.argv) > 3:
        print "Usage: python %s $ROOT_FILE_OUTPUT_FROM_SHOWER_QUALITY [OPTIONAL: OUTFILE]" % sys.argv[0]
        quit()

#File that was output from ShowerQuality code including a ttree
fname = sys.argv[1] #'ShowerQuality_ana_out.root'
#Name of folder this script will create (or use, if exists) to put output image files in
output_directory = 'generated_plots'
#Name of the once-per-shower ttree from the ShowerQuality output root file
tshowername = 'fShowerTree_nueshowers'
#Name of the once-per-event ttree from the ShowerQuality output root file
teventname  = 'fEventTree_nueshowers'

# output file (where to store fit values / distributions)
fout = None
if (len(sys.argv) == 3):
        fout = open(sys.argv[2],'w')
        fout.write('ShowerEmuSmearing:{\n')

if not os.path.isfile(fname):
	raise IOError('ShowerQuality output file does not exist: %s'%fname)

#Output plots will be saved into a "generated_plots" folder
if not os.path.isdir(output_directory):
        os.makedirs(output_directory)

#Turn the ShowerQuality output into a pandas dataframe like a BAMF
shower_df = pd.DataFrame(root2array(fname,tshowername))
event_df  = pd.DataFrame(root2array(fname,teventname) )

#Plot of energy resolution
fig = energy_res_plot(shower_df,fout)
plt.savefig('%s/energy_res_single.png'%output_directory)
#plt.show()

#Plot of energy resolution vs energy
fig = eres_vsenergy_plot(shower_df)
plt.savefig('%s/eres_vsenergy_single.png'%output_directory)
#plt.show()

#2D histogram of reco vs true energy
for myplane in ['U','V','Y']:
        fig = energy_corr_plot(shower_df,plane='%s'%myplane)
        plt.savefig('%s/energy_correlation_%splane_single.png'%(output_directory,myplane))
        #plt.show()

#Plot of start point accuracy
fig = startpoint_acc_plot(shower_df)
plt.savefig('%s/startpoint_accuracy_single.png'%output_directory)
#plt.show()

#plot start-point resolution vs. energy
fig = startpoint_resolution_vsenergy_plot(shower_df)
plt.savefig('%s/startpoint_resolution_vsenergy_single.png'%output_directory)
#plt.show()

#Plot start point resolution per plane
fig = startpoint_resolution_plot(shower_df,fout)
plt.savefig('%s/startpoint_resolution_perplane_single.png'%output_directory)
#plt.show()

# 3D direction angular resolution
fig = anglediff_3d_plot(shower_df,180,fout)
plt.savefig('%s/3Ddir_accuracy_180_single.png'%output_directory)
#plt.show()

# 3D direction angular resolution
fig = anglediff_3d_plot(shower_df,30)
plt.savefig('%s/3Ddir_accuracy_30_single.png'%output_directory)
#plt.show()

# 3D angle resolution
fig = angleres_vsenergy_plot(shower_df)
plt.savefig('%s/angleres_vsenergy_plot_single.png'%output_directory)
#plt.show()

# 3D angle resolution vs. energy matrix
fig = angleres_vsenergy_matrix(shower_df)
plt.savefig('%s/angleres_vsenergy_matrix_single.png'%output_directory)
#plt.show()

# dEdx plots
fig = dEdx_plot(shower_df,fout)
plt.savefig('%s/dEdx_resolution_single.png'%output_directory)
#plt.show()

# dQdx plots
fig = dQdx_plot(shower_df)
plt.savefig('%s/dQdx_resolution_single.png'%output_directory)
#plt.show()

# Length plot
fig = length_plot(shower_df)
plt.savefig('%s/reco_shower_length_single.png'%output_directory)
#plt.show()

# Length vs reco energy
fig = length_energy_corr_plot(shower_df,plane='Y')
plt.savefig('%s/length_energy_correlation_Yplane_single.png'%(output_directory))
#plt.show()

# reco efficiency
fig = reco_efficiency_vsenergy_singleshower_plot(event_df)
plt.savefig('%s/reco_efficiency_singleshower_vsenergy_single.png'%output_directory)
#plt.show()

# reco efficiency for good angle showers
fig = reco_efficiency_vsenergy_goodangle_plot(shower_df)
plt.savefig('%s/reco_efficiency_vsenergy_goodangle_single.png'%output_directory)
#plt.show()

# Matched cluster efficiency
fig = cluster_efficiency_perplane_plot(shower_df)
plt.savefig('%s/cluster_efficiency_perplane_single.png'%output_directory)
#plt.show()

if (fout != None):
        fout.write('}\n')
        fout.close()
        print 'closed file '
