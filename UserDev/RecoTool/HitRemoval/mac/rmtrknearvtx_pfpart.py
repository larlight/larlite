#
# Example PyROOT script to run analysis module, ana_base.
# The usage is same for inherited analysis class instance.
#

# Load libraries
import os, ROOT, sys
from ROOT import *
from ROOT import gSystem
from larlite import larlite as fmwk
import time

# Create ana_processor instance
my_proc=fmwk.ana_processor()

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

for x in range(len(sys.argv)-2):
    my_proc.add_input_file(sys.argv[x+1])

my_proc.set_ana_output_file("ana.root")
my_proc.set_output_file(sys.argv[-1])

hitremoval = fmwk.RmTrksNearVtx()
#hitremoval.setPFPartProducer("pandoraCosmic")
hitremoval.setPFPartProducer("pandoraNu")
hitremoval.setVtxProducer("mcroi")
my_proc.add_process(hitremoval)

my_proc.set_data_to_write(fmwk.data.kHit,"shrlike")
my_proc.set_data_to_write(fmwk.data.kHit,"gaushit")
my_proc.set_data_to_write(fmwk.data.kCluster,"pandoraNu")
my_proc.set_data_to_write(fmwk.data.kAssociation,"pandoraNu") 
my_proc.set_data_to_write(fmwk.data.kCluster,"pandoraCosmic")
my_proc.set_data_to_write(fmwk.data.kAssociation,"pandoraCosmic")
my_proc.set_data_to_write(fmwk.data.kCluster,"shrlike")
my_proc.set_data_to_write(fmwk.data.kAssociation,"shrlike")

my_proc.run(0,10)
sys.exit(0);
