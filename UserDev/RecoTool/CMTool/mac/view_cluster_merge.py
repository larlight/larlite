from ROOT import *
from recotool.mergeDef import *
import sys

import argparse

parser = argparse.ArgumentParser(description="Python script to process and merge showers.")
group = parser.add_mutually_exclusive_group()
group.add_argument("-v", "--verbose", help="Turn on verbose output",
                    action="store_true")
group.add_argument("-q", "--quiet", help="Turn off most output",
                    action="store_true")
parser.add_argument("-s","--source",help="Name of input file")
parser.add_argument("-o","--data-output",help="Output data file, if event is changed")
parser.add_argument("-a","--ana-output",help="Analysis output file")
parser.add_argument("-n", "--num-events",help="Number of events to process")
parser.add_argument("-f", "--first",help="First event to start processing")
parser.add_argument("-d","--display",help="Turn on the display to see each view before and after",
                    action="store_true")
parser.add_argument("-c","--cluster",help="Producer of original cluster")
args = parser.parse_args()

if len(sys.argv) == 1:
    parser.print_help()

if args.verbose:
    print("Verbose mode turned on.")
    if args.source != None:
        print("\tSource file is " + args.source)
    if args.data_output != None:
        print("\tData output file is " + args.data_output)
    if args.ana_output != None:
        print("\tAna output file is " + args.ana_output)

if args.source == None:
    print("Error: please specificy an input file with -s or --source.")
    quit()

if args.data_output == None:
    args.data_output = "default_event_output.root"
    print("No event output file selected.  If necessary, output will go to:")
    print("\t"+args.data_output)

if args.ana_output == None:
    args.ana_output = "default_ana_output.root"
    print("No ana output file selected.  If necessary, output will go to:")
    print("\t"+args.ana_output)

if args.num_events == None:
    args.num_events = 0

if args.first == None:
    args.first = 0

if args.cluster == None:
    args.cluster = "fuzzycluster"

print(args.cluster, " is a ", type(args.cluster))


ana_proc = larlite.ana_processor()

if args.verbose:
    ana_proc.set_verbosity(larlite.msg.DEBUG)

# Not sure what this does
ana_proc.set_io_mode(larlite.storage_manager.kBOTH)

# Add the input file.  Not sure if the above takes multiple input files yet
ana_proc.add_input_file(args.source)

# ?
# larlite.storage_manager.get().set_in_rootdir("scanner")

# set output file
ana_proc.set_output_file(args.data_output)

# Set destination for ana stuff
ana_proc.set_ana_output_file(args.ana_output)

prelimMerger = GetPrelimMergerInstance(str(args.cluster))
# prelimMerger = GetPrelimMergerInstance(producer="fuzzycluster",saveOutput=True)

secondMerger = GetSecondMergerInstance(str("merged"+args.cluster))
# secondMerger = GetSecondMergerInstance(producer="mergedfuzzycluster",saveOutput=True)

ana_proc.add_process(prelimMerger)
ana_proc.add_process(secondMerger)


# if this is not displaying clusters, just run:
if args.display == None:
    ana_proc.run(int(args.first),int(args.num_events))

else:

    raw_viewer = larlite.ClusterViewer()
    raw_viewer.SetInputProducer(str(args.cluster))
    first_viewer = larlite.ClusterViewer()
    first_viewer.SetInputProducer(str("merged"+args.cluster))
    second_viewer = larlite.ClusterViewer()
    second_viewer.SetInputProducer(str("mergedmerged"+args.cluster))

    ana_proc.add_process(raw_viewer)
    ana_proc.add_process(first_viewer)
    ana_proc.add_process(second_viewer)


    while true:

        try:
            args.first = eval(input('Hit Enter to continue to next evt, or type in an event number to jump to that event:'))
        except SyntaxError:
            args.first = int(args.first) + 1

        ana_proc.process_event(int(args.first))

        raw_viewer.DrawAllClusters();
        first_viewer.DrawAllClusters();
        second_viewer.DrawAllClusters();




