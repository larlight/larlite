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
#Name of the once-per-shower ttree from the ShowerQuality output root file
tname = 'fShowerTree'

if not os.path.isfile(fname):
	raise IOError('ShowerQuality output file does not exist: %s'%fname)

#Turn the ShowerQuality output into a pandas dataframe like a BAMF
df = pd.DataFrame(root2array(fname,tname))

#Plot of energy asymmetro
fig = energy_asym_plot(df)
plt.savefig('energy_asym.pdf')
plt.show()

#Plot of start point accuracy
fig = startpoint_acc_plot(df)
plt.savefig('startpoint_accuracy.pdf')
plt.show()

# 3D direction angular resolution
fig = anglediff_3d_plot(df)
plt.savefig('3Ddir_accuracy.pdf')
plt.show()

# dEdx plots
fig = dEdx_plot(df)
plt.savefig('dEdx_resolution.pdf')
plt.show()
