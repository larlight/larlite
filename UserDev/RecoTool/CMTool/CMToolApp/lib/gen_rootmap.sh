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
# CMTool
rootlibmap libRecoTool_CMToolApp.rootmap libRecoTool_CMToolApp.so \
    $LARLITE_USERDEVDIR/RecoTool/CMTool/CMToolApp/LinkDef.h \
    libRecoTool_CMTAlgPriority.so \
    libRecoTool_CMTAlgMerge.so \
    libRecoTool_CMTAlgMatch.so \
    libLArLite_Base.so \
    libLArLite_DataFormat.so \
    libLArLite_Analysis.so \
    libGpad.so
