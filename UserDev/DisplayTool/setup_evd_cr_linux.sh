#Setup larsoft:

source /grid/fermiapp/products/uboone/setup_uboone.sh
setup larsoft v04_26_00 -q e7:prof

#Setup uboonedaq_datatypes
setup uboonedaq_datatypes v6_15_03c -q e7:prof

#Set up postgresql
setup postgresql v9_1_15

#set up python packages:
source /home/ubooneshift/pystack/setup.sh

export SECRET_CONNSTRING="host=fnalpgsdev.fnal.gov dbname=uboonedaq_dev user=uboonedaq_web port=5436 password=argon\!uBooNE"

#Set up pubs:
source /home/ubooneshift/pubs/config/setup_ubooneshift.sh

#Do this to set up the required area to import functions from pubs to get latest files:
export PYTHONPATH=$PUB_TOP_DIR/bin:$PYTHONPATH

#setup larlite too
source /home/ubooneshift/larlite/config/setup_reco.sh
