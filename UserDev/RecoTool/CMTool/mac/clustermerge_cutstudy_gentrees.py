from ROOT import *
gSystem.Load("libAnalysis")
import sys

#filename = "/Users/davidkaleko/Data/ShowerStudy/PDGID_11/shower_larlight_11.root"
filename = sys.argv[1]

#for studying distance cuts
#cutvals = [ 0, 2, 4, 7, 10, 30, 75, 150, 250, 300, 450, 500, 600, 700, 800, 900, 1000 ]

#for studying angle cuts
#cutvals = [0, 2, 4, 7, 10, 15, 20, 25, 30, 50, 70, 90, 110, 120, 140, 160, 180]
#cutvals = [ 0 ]
cutvals = [ 2 ]

# Create processor 
ana_proc = larlight.ana_processor()

# Create analyzer
my_ana = larlight.ClusterMergeAlg()

# Run processor in a loop. Each time ana_processor::run() is called, 
# analyzers that were run are detached from ana_processor instance.
# So you have to attach it @ the beginning of the loop. Also I/O config
# should be explicitly reset.
for x in cutvals:

    # Config analyzer
    my_ana.SetAngleCut( x )
    my_ana.SetSquaredDistanceCut( 1000000000 )
    #my_ana.VerboseMode(True)

    # Attach analyzer
    ana_proc.add_process(my_ana)

    # Config IO
    ana_proc.set_io_mode(larlight.storage_manager.READ)
    ana_proc.add_input_file(filename)
    ana_proc.set_input_rootdir("scanner")
    ana_proc.set_ana_output_file("outputs/merge_Tree_cut%d.root" % x )

    # Run
    ana_proc.run(123,1)
    #ana_proc.run(405,1)




