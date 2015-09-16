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

df.hist('mc_reco_dist')
plt.show()
