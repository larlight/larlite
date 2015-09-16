#Import various things to draw plots. No histograms or plots in root!
try:
	import numpy as np
	import pandas as pd
	import matplotlib.pyplot as plt
	import root_numpy
	from root_numpy import root2array
except ImportError:
	raise ImportError('Something didn\'t import correctly! Do you have numpy, pandas, matplotlib, and root_numpy? Get it!')

import os.path

#File that was output from ShowerQuality code including a ttree
fname = 'ShowerQuality_ana_out.root'
#Name of the ttree from the ShowerQuality output root file
tname = 'fShowerQualityTree'

if not os.path.isfile(fname):
	raise IOError('ShowerQuality output file does not exist: %s'%fname)

#Turn the ShowerQuality output into a pandas dataframe like a BAMF
df = pd.DataFrame(root2array(fname,tname))

#Plot of energy asymmetry
plt.title("(MC Energy - Reco Energy)/MC Energy")
((df['mc_energy']-df['reco_energy_U'])/df['mc_energy']).hist(bins=np.linspace(-2,2,200),label='U',alpha=0.5)
((df['mc_energy']-df['reco_energy_V'])/df['mc_energy']).hist(bins=np.linspace(-2,2,200),label='V',alpha=0.5)
((df['mc_energy']-df['reco_energy_Y'])/df['mc_energy']).hist(bins=np.linspace(-2,2,200),label='Y',alpha=0.5)
plt.legend()
#plt.savefig('energy_asym.pdf')
plt.show()

#Plot of start point accuracy
# plt.title('Distance Between (3D) MC Start Point and Reco Start Point')
# plt.xlabel('Distance in 3D [cm]')
# plt.ylabel('Counts')
# df['mc_reco_dist'].hist(bins=100)
# plt.savefig('startpoint_accuracy.pdf')

#plt.show()
