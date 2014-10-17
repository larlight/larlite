from ROOT import *
gSystem.Load("libAnalysis")
import sys

filename = sys.argv[1]

fout = TFile.Open("ana.root","RECREATE")

dataman=larlite.storage_manager()
dataman.set_io_mode(dataman.kREAD)
dataman.add_in_filename(sys.argv[1])
dataman.set_in_rootdir("scanner")
dataman.open()

hNHitsMax=TH1D("hNHitsMax","",1000,0,5000)
hNHitsMin=TH1D("hNHitsMin","",1000,-0.5,999.5)
hQMax=TH1D("hQMax","Max Cluster Charge; Charge; Entries",300,0,1e5);
hQMin=TH1D("hQMin","Min Cluster Charge; Charge; Entries",300,0,5e4);
hNHitsQ=TH2D("hNHitsQ","Cluster NHits vs. Q-Fraction; # Hits; Charge Fraction",
             300,0,1500,300,0,3e6)

while dataman.next_event():

    ev_cluster = dataman.get_data(larlight.DATA.ShowerAngleCluster)
    
    clusters = ev_cluster.GetClusterCollection()

    min_nhits=[1e9,1e9,1e9]

    max_nhits=[0,0,0]

    min_q=[1e10,1e10,1e10]
    
    max_q=[0,0,0]

    for x in xrange(clusters.size()):

        hits = clusters[x].Hits()
        nhits = hits.size()

        cluster_q = 0

        for y in xrange(nhits):
            
            cluster_q+=hits[y].Charge()

        hNHitsQ.Fill(nhits,cluster_q)

        view = clusters[x].View()

        if nhits < min_nhits[view]:

            min_nhits[view] = nhits
            
        if nhits > max_nhits[view]:

            max_nhits[view] = nhits

        if cluster_q < min_q[view]:

            min_q[view] = cluster_q

        if cluster_q > max_q[view]:

            max_q[view] = cluster_q

    for x in xrange(len(min_nhits)):

        hNHitsMin.Fill(min_nhits[x])
        hNHitsMax.Fill(max_nhits[x])
        hQMax.Fill(max_q[x])
        hQMin.Fill(min_q[x])

fout.cd()
hNHitsMax.Write()
hNHitsMin.Write()
hQMax.Write()
hQMin.Write()
hNHitsQ.Write()
fout.Close()

