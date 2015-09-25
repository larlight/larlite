try:
	import numpy as np
	from scipy.optimize import curve_fit

except ImportError:
	raise ImportError('Something didn\'t import correctly! Do you have numpy, pandas, matplotlib, and root_numpy? Get it!')

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

