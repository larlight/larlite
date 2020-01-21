import ROOT
from ROOT import ertool
from ROOT import geoalgo
from basictool.geoviewer import GeoViewer
import matplotlib.pyplot as plt
import random as rnd
import time
import datetime

plt.ion()

tottime = 0

# track:
viewer = GeoViewer()
viewer._use_box = False
geoAlgo = geoalgo.GeoAlgo()
line1 = geoalgo.LineSegment(105.727, -66.7871, 814.841, 105.438, -67.4666, 815.464)
line2 = geoalgo.LineSegment(105.229, -67.5519, 815.806, 108.149, -62.1392, 810.246)
pt1 = geoalgo.Vector()
pt2 = geoalgo.Vector()
d = geoAlgo.SqDist(line1,line2,pt1,pt2)
line3 = geoalgo.LineSegment(pt1,pt2)
viewer.add(line1,'ln1','b')
viewer.add(line2,'ln2','r')
viewer.add(line3,'ln3','k')
print('distance is %.02f'%d)
viewer.show()
try:
    counter = eval(input('Hit ENTER when you are ready to close the viewer'))
except:
    print("done with this example...")
