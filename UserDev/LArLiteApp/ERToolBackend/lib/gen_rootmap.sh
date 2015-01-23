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
# ShowerPdfTool
rootlibmap libLArLite_ERToolBackend.rootmap libLArLite_ERToolBackend.so \
    $LARLITE_USERDEVDIR/LArLiteApp/ERToolBackend/LinkDef.h \
    libLArLite_Analysis.so \
    libSelectionTool_ERFilter.so \
    libSelectionTool_ERAlgo.so \
    libSelectionTool_ERBase.so

