#
# Simple unit test routine for the fmwk
# Call showerreco::EmptyAlg::RecoOneShower function w/ empty clusters
#

from ROOT import showerreco

# Instantiate algorithm
alg = showerreco.EmptyAlg()

# Create fake clusters
cluster0 = showerreco.ShowerCluster_t()
cluster1 = showerreco.ShowerCluster_t()
cluster2 = showerreco.ShowerCluster_t()

# Create fake cluster set
cluster_set = showerreco.ShowerClusterSet_t()
cluster_set.push_back( cluster0 )
cluster_set.push_back( cluster1 )
cluster_set.push_back( cluster2 )

# Call reconstruct
res = alg.RecoOneShower( cluster_set )


