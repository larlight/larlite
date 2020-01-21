#!/usr/bin/python

import sys
import numpy as np
import ctypes
import matplotlib.pyplot as plt
import time
import pyqtgraph as pg

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)




from larlite import larlite as fmwk
from ROOT import example
from ROOT import *

gSystem.Load("libLArLite_LArUtil")
gSystem.Load("libargoneut_electrons_EventViewer.so")
gSystem.Load("libExample_PyPackage.so")
larutil.LArUtilManager.Reconfigure(fmwk.geo.kArgoNeuT)
aspectRatio = larutil.GeometryUtilities.GetME().TimeToCm() / larutil.GeometryUtilities.GetME().WireToCm()
# aspectRatio = 1.0 / aspectRatio
# Need to know the aspect ratio for these

c2p = example.PyExample()

# Create ana_processor instance
my_proc = fmwk.ana_processor()


# Set input root file
for x in range(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("from_test_ana_you_can_remove_me.root");

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_proc.add_process(fmwk.DrawRaw())

print()
print("Finished configuring ana_processor. Start event loop!")
print()

pg.plot()



# Let's run it.
# while my_proc.process_event():

my_proc.process_event()
time1 = time.clock()
d5 = c2p.Convert(my_proc.get_process(0).getDataByPlane(0))
d6 = c2p.Convert(my_proc.get_process(0).getDataByPlane(1))
# print "Done c++ step"
# d0 = np.array(d5)
d1 = np.array(d6)

print("Done converting, show!")
time2 = time.clock()
# fig, axes = plt.subplots(2,1,figsize=(10,20))

print(type(d1))
# print d0.shape
# print d0
# for ax in axes.flat:
    # ax.imshow(d.T,aspect=aspectRatio)
# axes[0].imshow(d0.T,aspect=aspectRatio,cmap="jet",clim=(0.0,15))
# axes[1].imshow(d1.T,aspect=aspectRatio,cmap="jet")

time3 = time.clock()
print(("Time to prepare: ", (time2-time1)))
print(("Time to draw   : ", (time3-time2)))
# 
# plt.show()

imv = pg.ImageView()
imv.show()
imv.setImage(d1)


# pg.image(d0)
# pg.show(d0)

sys.stdin.readline()



# done!
print()
print("Finished running ana_processor event loop!")
print()

sys.exit(0)
