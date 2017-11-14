import sys, os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)


from larlite import larlite as fmwk
from ROOT import twodimtools
a = twodimtools.Poly2D()
print a

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import matplotlib.patches as patches
from matplotlib.collections import PatchCollection
from matplotlib.path import Path

plt.ion()
plt.rcParams.update({'font.size':18})

fig = plt.figure(figsize=(8,8))
ax = fig.add_subplot(111,aspect='equal')

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    fname = sys.argv[x+1]
    my_proc.add_input_file(fname)
    
# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify analysis output root file name
my_proc.set_ana_output_file("");

# Specify data output root file name
my_proc.set_output_file('out.root')

#hitproducer = 'shrhits2'
hitproducer = 'gaushit'

clusterer = fmwk.ConeOverlapTag()
clusterer.setShowerProducer("showerreco")
clusterer.setPhotonProducer("rawcluster")
clusterer.setOutPFPartProducer("coneshower")
clusterer.setOutClusterProducer("coneshower")
clusterer.setDebug(False)

my_proc.add_process(clusterer)

print
print  "Finished configuring ana_processor. Start event loop!"
print

colors = ['r','b','y','g','m','c']

while my_proc.process_event():

    ax.cla()
    fig.gca()

    shower_poly   = clusterer.getShowerPolygon(2)
    photon_poly_v = clusterer.getPhotonPolygon(2)

    shrpts_w = []
    shrpts_t = []
    for n in xrange(shower_poly.Size()):
        shrpts_w.append( shower_poly.Point(n).first  )
        shrpts_t.append( shower_poly.Point(n).second )
    shrpts_w.append( shower_poly.Point(0).first  )
    shrpts_t.append( shower_poly.Point(0).second )

    plt.plot(shrpts_w,shrpts_t,color='k',lw=5)

    for i in xrange(photon_poly_v.size()):

        poly = photon_poly_v.at(i)

        print 'polygon with size : ',poly.Size()

        poly_w = []
        poly_t = []
        for n in xrange(poly.Size()):
            poly_w.append( poly.Point(n).first  )
            poly_t.append( poly.Point(n).second )
        poly_w.append( poly.Point(0).first  )
        poly_t.append( poly.Point(0).second )
        
        plt.plot(poly_w,poly_t,color=colors[i%6],lw=2)

    plt.show()

    usrinput = raw_input("Hit Enter: next evt  || int: go to event number ||  q: exit viewer\n")
    if (usrinput == 'q'):
        sys.exit()
    

sys.exit()

