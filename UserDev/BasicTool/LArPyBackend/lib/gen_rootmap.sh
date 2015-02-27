#!/bin/bash

temp=make_rootmap.$$.temp

rootlibmap() {
 ROOTMAP=$1
 SOFILE=$2
 LINKDEF=$3
 shift 3
 DEPS=$*
 if [[ -e $SOFILE && -e $LINKDEF ]]; then
     rlibmap -f -o $ROOTMAP -l $SOFILE -d $DEPS -c $LINKDEF 2>> $temp
     rm -f $temp
 fi
}

######################################################
# LArPyBackend
rootlibmap libBasicTool_LArPyBackend.rootmap libBasicTool_LArPyBackend.so $LARLITE_USERDEVDIR/BasicTool/LArPyBackend/LinkDef.h \
    libGeoAlgo.so
