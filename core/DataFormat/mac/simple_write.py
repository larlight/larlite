import sys
from ROOT import larlite
import ROOT

man=larlite.storage_manager()
man.set_io_mode(man.kWRITE)
man.set_out_filename("trialpy.root")
man.set_verbosity(larlite.msg.kDEBUG)
man.open()

run_id = 1;
subrun_id = 1;

my_event_track = man.get_data(larlite.data.kTrack,"test")
my_v_hit1 = man.get_data(larlite.data.kHit1,"test")
my_int = man.get_data(larlite.data.kInt,"test")
my_m_intdouble = man.get_data(larlite.data.kMapIntDouble,"test")
my_larsofthits = man.get_data(larlite.data.kLarSoftHit,"test")
my_larsoftclusters = man.get_data(larlite.data.kLarSoftCluster,"test")

for event_id in xrange(100):
    print event_id
    man.set_id(run_id, subrun_id, event_id)

    # Let's make 2 tracks!
    for j in xrange(2):
        track = larlite.track()
        for k in xrange(20):
            track.set_track_id(j)
            aVector = ROOT.TVector3(k,k,k)
            track.add_vertex(aVector)
            track.add_momentum(1.0)
            track.add_direction(aVector)
        my_event_track.push_back(track)

        h = larlite.hit1()
        # The argument value is arbitrary, just writing something
        # so that I can read it back later and check that I read
        # what I wrote.
        h.set_rms(11.0 + j)
        my_v_hit1.product().push_back(h)

        # All values same as default constructor except the RMS
        # where I put in an arbitrary test value
        larsoftHit = ROOT.recob.Hit(ROOT.raw.InvalidChannelID, 0, 0, 0., -1., 200.0 + event_id + j, 0., -1., 0., 0., -1., 0, -1, 0., -1, ROOT.geo.kUnknown, ROOT.geo.kMysteryType, ROOT.geo.WireID())

        my_larsofthits.product().push_back(larsoftHit)

        larsoftCluster = ROOT.recob.Cluster(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, event_id + j, 0.0, 0.0, 0, 0, ROOT.geo.PlaneID())


        my_larsoftclusters.product().push_back(larsoftCluster)

    my_int.set(21);

    p = ROOT.std.pair('const int','double')(31, 101.0)

    my_m_intdouble.product().insert(p)

    man.next_event()

man.close()
