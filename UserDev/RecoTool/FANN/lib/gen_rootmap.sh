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
# FANN
rootlibmap libRecoTool_FANN.rootmap libRecoTool_FANN.so \
    $LARLITE_USERDEVDIR/RecoTool/FANN/LinkDef.h \
    libfloatfann.so libLArLite_Base.so libLArLite_DataFormat.so libLArLite_LArUtil.so

