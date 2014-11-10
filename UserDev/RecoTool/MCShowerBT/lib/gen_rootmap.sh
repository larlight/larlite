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
# MCShowerBT
rootlibmap libRecoTool_MCShowerBT.rootmap libRecoTool_MCShowerBT.so \
    $LARLITE_USERDEVDIR/RecoTool/MCShowerBT/LinkDef.h \
    libLArLite_DataFormat.so \
    libLArLite_LArUtil.so
