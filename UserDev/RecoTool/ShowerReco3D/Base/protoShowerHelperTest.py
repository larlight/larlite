import ROOT
import larlite

from ROOT import larlite, showerreco

filename="/data_linux/pfp_v2_pandoraNu.root"

psh = showerreco.ProtoShowerHelper()

_mgr = larlite.storage_manager()

# Get the pfparticles from the storage manager:
_mgr.reset()
_mgr.add_in_filename(filename)
_mgr.set_io_mode(larlite.storage_manager.kREAD)
_mgr.open()
_mgr.next_event()

producer_name="pandoraNu"

# ev_pfp = _mgr.get_data(larlite.event_pfpart)(larlite.data.kPFParticle,"pandoraNu")

protoShowerVec = ROOT.vector(showerreco.ProtoShower)()

psh.GenerateProtoShowers(_mgr,producer_name,protoShowerVec)


for shower in protoShowerVec:
  print "This shower has {N} params associated.".format(N=shower.params().size())
  print "shower.hasCluster2D(): ", shower.hasCluster2D()
  print "shower.hasCluster3D(): ", shower.hasCluster3D()
  print "shower.hasVertex(): ", shower.hasVertex()
  shower.params3D().Report()
  for vertex in shower.vertexes():
    print "vertex is ({x},{y},{z})".format(x=vertex.X(), y=vertex.Y(), z=vertex.Z())
    # print "\n\n\n"