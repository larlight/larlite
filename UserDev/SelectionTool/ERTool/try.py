from ROOT import TChain, geoalgo, TVector3

ch=TChain("track_trackkalmanhit_tree")
ch.AddFile("$LARLITE_BASEDIR/data/eminus/larlite_reco3d_000.root")

ch.GetEntries()
ch.GetEntry(0)

trk1 = ch.track_trackkalmanhit_branch[0]
trk2 = geoalgo.Trajectory()
length = 0
pos = trk1.vertex_at(0)
for x in xrange(trk1.n_point()):

    trk2 += geoalgo.Vector(trk1.vertex_at(x))
    if not x: continue

    thisvtx = TVector3(trk1.vertex_at(x))
    print "diff vec:",thisvtx[0]-trk2[x][0],thisvtx[1]-trk2[x][1],thisvtx[2]-trk2[x][2]
    print "prev:    ",pos[0],pos[1],pos[2]
    print "this:    ",thisvtx[0],thisvtx[1],thisvtx[2]
    print "segment: ",thisvtx[0]-pos[0],thisvtx[1]-pos[1],thisvtx[2]-pos[2]
    thisvtx -= pos
    length += thisvtx.Mag()
    #print thisvtx[0],thisvtx[1],thisvtx[2]
    print length-trk2.Length()
    print
    pos = TVector3(trk1.vertex_at(x))
print
for x in xrange(trk1.n_point()):

    for y in xrange(3):
        print trk1.vertex_at(x)[y] - trk2[x][y],
    print


