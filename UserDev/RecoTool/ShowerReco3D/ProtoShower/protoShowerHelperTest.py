import ROOT
import larlite

from ROOT import larlite, protoshower, larutil

filename="/data_linux/dedx_files/electron_sample_bootleg_matched.root"

larutil.LArUtilManager.Reconfigure(larlite.geo.kArgoNeuT)

psh = protoshower.ProtoShowerHelper()
pshalg = protoshower.ProtoShowerAlgArgoNeuT()

psh.setProtoShowerAlg(pshalg)


_mgr = larlite.storage_manager()

# Get the pfparticles from the storage manager:
_mgr.reset()
_mgr.add_in_filename(filename)
_mgr.set_io_mode(larlite.storage_manager.kREAD)
_mgr.open()
_mgr.next_event()
_mgr.go_to(23)

producer_name="bootlegMatched"

# ev_pfp = _mgr.get_data(larlite.event_pfpart)(larlite.data.kPFParticle,"pandoraNu")

protoShowerVec = ROOT.vector(protoshower.ProtoShower)()

psh.GenerateProtoShowers(_mgr,producer_name,protoShowerVec)


for shower in protoShowerVec:
  print "This shower has {N} params associated.".format(N=shower.params().size())
  print "shower.hasCluster2D(): ", shower.hasCluster2D()
  print "shower.hasCluster3D(): ", shower.hasCluster3D()
  print "shower.hasVertex(): ", shower.hasVertex()
  for param in shower.params():
    param.Report()
    print param.start_point.w, ",", param.start_point.t
  for vertex in shower.vertexes():
    print "vertex is ({x},{y},{z})".format(x=vertex.X(), y=vertex.Y(), z=vertex.Z())
    # print "\n\n\n"