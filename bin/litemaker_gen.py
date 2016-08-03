#!/usr/bin/env python
import sys
import argparse
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
from ROOT import TFile, gROOT, TTree


def get_file_base(experiment="microboone",path="all"):
 return \
 """#include \"services_{exp}.fcl\"

  litemaker_base:
  {{
    # module name
    module_type:       \"LiteScanner\"
    # boolean to decide whether or not to store association
    store_association: true
    # output file name
    out_filename:      \"larlite_{path}.root\"
    stream: "empty"
    unique_filename: false
  }}

  """.format(exp=experiment,path=path)

def get_fcl_base(experiment="microboone"):
  return \
"""process_name: LiteMaker
services:
{{
  # Load the service that manages root files for histograms.
  TFileService: {{ fileName: \"ana_hist.root\" }}
  Timing:       {{}}
  RandomNumberGenerator: {{}} #ART native random number generator
  message:      @local::standard_warning
  user:         @local::{exp}_services        
}}

source:
{{
  module_type: RootInput
  maxEvents:  -1        # Number of events to create
}}

physics:
{{
  analyzers:
  {{
    scanner: @local::litemaker_base
  }}
  analyzeIt: [ scanner ]
  end_paths: [ analyzeIt ]
}}

""".format(exp=experiment)

lookUpTable={
  "": "unknown",
  "": "event",
  "": "gtruth",
  "simb::MCTruth": "mctruth",
  "simb::MCParticle": "mcpart",
  "": "mcflux",
  "": "mctrajectory",
  "": "mcnu",
  "sim::SimChannel": "simch",
  "sim::MCShower": "mcshower",
  "raw::RawDigit": "rawdigit",
  "recob::Wire": "wire",
  "recob::Hit": "hit",
  "anab::CosmicTag": "cosmictag",
  "recob::OpHit": "ophit",
  "recob::OpFlash": "opflash",
  "recob::Cluster": "cluster",
  "recob::Seed": "seed",
  "recob::SpacePoint": "sps",
  "recob::Track": "track",
  "recob::Shower": "shower",
  "recob::Vertex": "vertex",
  "": "endpoint2d",
  "anab::Calorimetry": "calo",
  "anab::ParticleID": "partid",
  "recob::PFParticle": "pfpart",
  "": "user",
  "raw::Trigger": "trigger",
  "sim::MCTrack": "mctrack",
  "": "mctree",
  "t962::MINOS": "minos",
  "optdata::FIFOChannel": "fifo",
  "": "ass",
  "recob::PCAxis": "pcaxis",
  "": "fmatch",
  "": "trackmom",
  "raw::OpDetWaveform": "opdigit",
  "sim::SimPhotons": "simphotons"
}


class product(object):


  def __init__(self, name, typeName):
      self._name=name
      self._typeName=typeName
      self._isAssociation=False
      self._associatedProduct=None
      self._larliteLookupName=None
      self._producer=None
      self._stage=None

      self.parse()


  def producer(self):
      return self._producer

  def name(self):
      return self._name

  def typeName(self):
      return self._typeName

  def larliteLookupName(self):
      return self._larliteLookupName

  def isAssociation(self):
      return self._isAssociation

  # def associationProduct(self):
      # pass

  # def reverseAssociationProduct(self):
      # pass

  def parse(self):
      tokens=self._name.split('_')
      # Name goes as object_producer_stage
      self._producer=tokens[1]
      self._stage=tokens[-1]

      # The first token is the object, which we can use the lookup table to parse
      #
      # Figure out if this is an association:
      if "Assns" in tokens[0]:
          self._isAssociation=True
          # print self._typeName, ", ", self.producer()
          assTokens=self._typeName.split(',')
          product1=assTokens[0].split('<')[-1]
          product2=assTokens[1]
          print "Association between ", product1, " and ", product2, " by ", self._producer
          try:
              self._larliteLookupName=lookUpTable[product1]
          except:
              # print product1
              self._larliteLookupName=None
      else:
          try:
              self._larliteLookupName=lookUpTable[tokens[0].rstrip('s')]
          except:
              # print tokens[0]
              self._larliteLookupName=None

      return

def pingFile(file):
    """
    this function opens the file and
    determines what is available to draw
    """
    # This function opens the file to see
    # what data products are available


    gROOT.ProcessLine("gErrorIgnoreLevel = kError;")


    products=[]

    # Open the file
    f=TFile(file)

    e=f.Get("Events")

    keyTable=dict()

    # prepare a dictionary of data products

    # Loop over the keys (list of trees)
    for key in e.GetListOfBranches():

        if key.GetTypeName() == 'art::EventAuxiliary':
            continue
        # if key.name()/startswith('art'):
            # pass

        # print key.GetName()
        # products.append(key.GetTypeName())
        prod=product(key.GetName(), key.GetTypeName())
        # print prod.isAssociation()
        products.append(prod)
        # # keys are dataproduct_producer_tree
        # thisKeyList = key.GetName().split('_')
        # # gets three items in thisKeyList, which is a list
        # # [dataProduct, producer, 'tree'] (don't care about 'tree')
        # # check if the data product is in the dict:
        # if thisKeyList[0] in lookUpTable:
        #     # extend the list:
        #     lookUpTable[thisKeyList[0]] += (thisKeyList[1], )
        # else:
        #     lookUpTable.update({thisKeyList[0]: (thisKeyList[1],)})

    # keyTable.update(lookUpTable)

    # print listOfUniqueProducts
    return products


if __name__ == '__main__':



    parser=argparse.ArgumentParser(
        description='Python utility to generate larsoft -> larlite conversion .fcl files.')
    geom=parser.add_mutually_exclusive_group()
    geom.add_argument('-A', '-a', '--argoneut',
                      action='store_true',
                      help="Use argoneut as the experiment")
    geom.add_argument('-U', '-u', '--uboone',
                      action='store_true',
                      help="Use microboone as the experiment (default)")
    parser.add_argument('file', help="Input file to use")
    parser.add_argument(
        "-o", "--output", help="Name of output fcl file", default="larlite_maker_all.fcl")

    # if len(sys.argv) < 2:
    #     print '\nUsage: %s larsoft_file.root [outputfile.fcl]\n'%sys.argv[0]
    #     quit()
    args=parser.parse_args()

    if args.argoneut:
      exp="argoneut"
    else:
      exp="microboone"

    print args

    if args.file is None:
      parser.print_help()
      sys.exit()


    outfile=args.output

    # if len(sys.argv) > 2:
    #   outfile = sys.argv[2]
    # else:
    #   outfile = "larlite_all.fcl"

    filename=args.file

    larlite_data_table=dict()
    larlite_assn_table=dict()

    table=pingFile(filename)
    for product in table:
        if product.larliteLookupName() is None or product.producer() is None:
            continue
        if product.isAssociation():
            if product.larliteLookupName() not in larlite_assn_table:
                larlite_assn_table.update({product.larliteLookupName(): []})
            # Check for repeats:
            if product.producer() not in larlite_assn_table[product.larliteLookupName()]:
                larlite_assn_table[product.larliteLookupName()].append(
                    product.producer())
        else:
            # print product.larliteLookupName(), ":
            # [\"%s\"]"%product.producer()
            if product.larliteLookupName() not in larlite_data_table:
              larlite_data_table.update({product.larliteLookupName(): []})
            # Check for repeats:
            if product.producer() not in larlite_data_table[product.larliteLookupName()]:
                larlite_data_table[product.larliteLookupName()].append(
                    product.producer())
        # print product.typeName()

    print ""
    print ""

    # for item in larlite_data_table:
      # print item, ": ", larlite_data_table[item]

    # for item in larlite_assn_table:
    #   print item, ": ", larlite_assn_table[item]


    # Make the output file:
    with open(outfile, "w") as text_file:

        # Preliminary stuff:

        # text_file.write("#include \"larlite_maker.fcl\"\n")
        text_file.write(get_file_base(exp))
        text_file.write(get_fcl_base(exp))

        text_file.write("physics.analyzers.scanner: @local::litemaker_base\n")
        text_file.write(
            "physics.analyzers.scanner.out_filename: \"larlite_all.root\"\n")
        text_file.write(
            "physics.analyzers.scanner.pot_labels: [\"generator\"]\n")

        # Write the data lookup map:
        text_file.write("physics.analyzers.scanner.DataLookUpMap:\n")
        text_file.write("{\n")

        for item in larlite_data_table:
            text_file.write("  {0} : [ \n".format(item))
            for producer in larlite_data_table[item]:
                if producer != larlite_data_table[item][-1]:
                    text_file.write("    \"{0}\",\n".format(producer))
                else:
                    text_file.write("    \"{0}\"\n".format(producer))
                    text_file.write("  ]\n")


        text_file.write("}\n")


        # Write the associations lookup map:
        text_file.write("physics.analyzers.scanner.AssociationLookUpMap:\n")
        text_file.write("{\n")

        for item in larlite_assn_table:
            text_file.write("  {0} : [ \n".format(item))
            for producer in larlite_assn_table[item]:
                if producer != larlite_assn_table[item][-1]:
                    text_file.write("    \"{0}\",\n".format(producer))
                else:
                    text_file.write("    \"{0}\"\n".format(producer))
                    text_file.write("  ]\n")

        text_file.write("}\n")

        # Change the end path to only run modules created here:
        text_file.write("physics.analyzeIt:  [ scanner ]")
