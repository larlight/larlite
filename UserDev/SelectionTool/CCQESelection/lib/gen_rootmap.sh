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
# CCQESelection
rootlibmap libSelectionTool_CCQESelection.rootmap libSelectionTool_CCQESelection.so $LARLITE_USERDEVDIR/SelectionTool/CCQESelection/LinkDef.h \
    libLArLite_Analysis.so













