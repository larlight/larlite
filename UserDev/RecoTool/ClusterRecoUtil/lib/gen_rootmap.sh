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
# ClusterRecoUtil
rootlibmap libRecoTool_ClusterRecoUtil.rootmap libRecoTool_ClusterRecoUtil.so \
    $LARLITE_USERDEVDIR/RecoTool/ClusterRecoUtil/LinkDef.h \
    libRecoTool_FANN.so libLArLite_Base.so libLArLite_DataFormat.so libLArLite_Analysis.so libLArLite_LArUtil.so 
