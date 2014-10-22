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
# Analysis
rootlibmap libLArLite_Analysis.rootmap libLArLite_Analysis.so $LARLITE_COREDIR/Analysis/LinkDef.h \
    libLArLite_Base.so libLArLite_DataFormat.so libHist.so  libTree.so 















