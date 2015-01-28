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
# MCComp
rootlibmap libRecoTool_MCComp.rootmap libRecoTool_MCComp.so $LARLITE_USERDEVDIR/RecoTool/MCComp/LinkDef.h \
    libLArLite_Base.so \
    libLArLite_LArUtil.so 
    
