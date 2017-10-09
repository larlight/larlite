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

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-2):
    fname = sys.argv[x+1]
    my_proc.add_input_file(fname)
    
# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify analysis output root file name
my_proc.set_ana_output_file("");

# if output file exits, quit
if (os.path.isfile( sys.argv[-1] ) ):
    print 'OUT file %s already exits...quit'%sys.argv[-1]
    sys.exit()

# Specify data output root file name
my_proc.set_output_file(sys.argv[-1])


mac = fmwk.MergeAlignedClusters()

mac.setVtxProducer("numuCC_vertex")
mac.setPFPProducer("iou")

mac.setOutClusterProducer("align")
mac.setOutPFPProducer("align")

mac.setAngleDiff(15.)

mac.setDebug(False)

my_proc.add_process(mac)

#my_proc.set_data_to_write(fmwk.data.kPFParticle,  "coneshower")
#my_proc.set_data_to_write(fmwk.data.kCluster,     "coneshower")
#my_proc.set_data_to_write(fmwk.data.kAssociation, "coneshower")

print
print  "Finished configuring ana_processor. Start event loop!"
print

my_proc.run()

sys.exit()

