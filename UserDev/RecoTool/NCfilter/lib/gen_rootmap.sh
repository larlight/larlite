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
# NCfilter
rootlibmap libRecoTool_NCfilter.rootmap libRecoTool_NCfilter.so $LARLITE_USERDEVDIR/RecoTool/NCfilter/LinkDef.h \
    libRecoTool_ClusterRecoUtil.so \
    libLArLite_Analysis.so













