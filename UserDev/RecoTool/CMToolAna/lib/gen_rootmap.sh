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
# CMToolAna
rootlibmap libRecoTool_CMToolAna.rootmap libRecoTool_CMToolAna.so \
    $LARLITE_USERDEVDIR/RecoTool/CMToolAna/LinkDef.h \
    libRecoTool_ClusterRecoUtil.so \
    libRecoTool_FANN.so \
    libRecoTool_CMToolApp.so \
    libRecoTool_AnalysisAlg.so \
    libRecoTool_MCShowerBT.so \
    libLArLite_Analysis.so














