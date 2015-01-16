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
rootlibmap libSelectionTool_ERBase.rootmap libSelectionTool_ERBase.so \
    $LARLITE_USERDEVDIR/SelectionTool/ERTool/Base/LinkDef.h \
    libBasicTool_GeoAlgo.so \
    libRooFit.so \
    libEG.so












