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
rootlibmap libSelectionTool_ERAlgo.rootmap libSelectionTool_ERAlgo.so \
    $LARLITE_USERDEVDIR/SelectionTool/ERTool/Algo/LinkDef.h \
    libBasicTool_EMShowerTools.so \
    libSelectionTool_ERBase.so \
    libGpad.so \
    libHist.so
