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
# ERTool_App
rootlibmap libSelectionTool_ERApp.rootmap libSelectionTool_ERApp.so \
    $LARLITE_USERDEVDIR/SelectionTool/ERTool/App/LinkDef.h \
    libSelectionTool_ERBase.so 
