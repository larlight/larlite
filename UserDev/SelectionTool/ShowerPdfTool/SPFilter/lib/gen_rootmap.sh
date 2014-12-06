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
rootlibmap libSelectionTool_SPFilter.rootmap libSelectionTool_SPFilter.so \
    $LARLITE_USERDEVDIR/SelectionTool/ShowerPdfTool/SPFilter/LinkDef.h \
    libSelectionTool_SPTBase.so













