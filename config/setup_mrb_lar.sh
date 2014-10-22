#!/bin/bash

# Editor
export EDITOR=emacs

# The following script sets up lots of things. Including
#
# (*) Common UPS products
#     > source /grid/fermiapp/products/setups.sh
#
# (*) LArSoft
#     > source /grid/fermiapp/products/larsoft/setup
#
# (*) Add uboone ups products
#     > echo $PRODUCTS | grep -q /grid/fermiapp/products/uboone \
#           && PRODUCTS=`dropit -e -p $PRODUCTS /grid/fermiapp/products/uboone`
#     > PRODUCTS=/grid/fermiapp/products/uboone:${PRODUCTS}
#
# (*) UBooNE beta ups products area
#     > PRODUCTS=/grid/fermiapp/uboone/software/products:${PRODUCTS}
#
# (*) Add uboone data path to FW_SEARCH_PATH
#     > [[ -n $FW_SEARCH_PATH ]] && FW_SEARCH_PATH=`dropit -e -p $FW_SEARCH_PATH /uboone/data`
#     > export FW_SEARCH_PATH=/uboone/data:${FW_SEARCH_PATH}
#
# (*) Seti[ git & mrb
#     > setup git
#     > setup gitflow
#     > setup mrb
#
# (*) Define MRB_PROJECT value
#     > export MRB_PROJECT=larsoft
#
source /grid/fermiapp/products/uboone/setup_uboone.sh

# List available versions of UBooNE software
#ups list -aK+ uboonecode

# Set nightly build LArSoft
#setup uboonecode nightly -q e4:prof
#setup larsoft nightly -q e5:prof
setup larsoft v03_03_00 -q e6:prof

# Set jobsub available (generic, not specific to uboone)
#setup jobsub_tools

# Set ubtools
#setup ubtools

