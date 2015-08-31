#!/usr/bin/env python

# Load libraries
import ROOT, sys, os
from ROOT import *
import argparse
import time

def main(**args):
  

  my_proc = larlite.ana_processor()
  # my_proc.enable_event_alignment(False)

  if args['verbose']:
      print "Verbose mode turned on."
      if args['source'] != None:
          print "\tSource file is " + args['source']
      if args['data_output'] != None:
          print "\tData output file is " + args['data_output']
      if args['ana_output'] != None:
          print "\tAna output file is " + args['ana_output'] 
      # my_proc.set_verbosity(larlite.msg.kDEBUG)
  # else:
  my_proc.set_verbosity(larlite.msg.kERROR)

  if args['source'] == None:
      print "Error: please specificy an input file with -s or --source."
      quit()

  if args['data_output'] == None:
      args['data_output'] = "default_event_output.root"
      if args['verbose']:
        print "No event output file selected.  If necessary, output will go to:"
        print "\t"+args['data_output']

  if args['ana_output'] == None:
      args['ana_output'] = "default_ana_output.root"
      if  args['verbose']:
        print "No ana output file selected.  If necessary, output will go to:"
        print "\t"+args['ana_output']

  if args['num_events'] != None:
    nevents = int(args['num_events'])

  my_proc.set_io_mode(larlite.storage_manager.kBOTH)

  for x in args['source']:
      my_proc.add_input_file(x)

  my_proc.set_output_file(args['data_output'])


  # Merge all:
  merger = larlite.ClusterMerger()
  
  ########################################
  # MERGE ALGORITHMS
  ########################################
  algo_array = cmtool.CBAlgoArray()
  mergeAll = cmtool.CBAlgoMergeAll()
  mergeAll.SetHits(3)
  algo_array.AddAlgo(mergeAll)

  
  merger.GetManager().AddMergeAlgo(algo_array)
  merger.SetInputProducer("cccluster")
  merger.SetOutputProducer("mergeall")
  merger.SaveOutputCluster()
  my_proc.add_process(merger)




  if args['num_events'] != None:
    start = time.clock()
    my_proc.run(0,nevents)
    end = time.clock()
    print "Processed ", nevents, " events in ", end-start, "seconds."
    print "Average per event: ", (end-start)/nevents, "seconds."
  else:
    my_proc.run()



if __name__ == '__main__':
  parser = argparse.ArgumentParser(description="Python script to process and merge showers.")
  group = parser.add_mutually_exclusive_group()
  group.add_argument("-v", "--verbose", help="Turn on verbose output",
                      action="store_true")
  group.add_argument("-q", "--quiet", help="Turn off most output",
                      action="store_true")
  parser.add_argument("-s","--source",nargs='*',help="Name of input file")
  parser.add_argument("-o","--data-output",help="Output data file, if event is changed")
  parser.add_argument("-a","--ana-output",help="Analysis output file")
  parser.add_argument("-n","--num-events",help="Number of events to run over")

  args = parser.parse_args()

  if len(sys.argv) == 1:
      print "\n-------You forgot to include a source file!-------\n"
      parser.print_help()

  main(**vars(args))
