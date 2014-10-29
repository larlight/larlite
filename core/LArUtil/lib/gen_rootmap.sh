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
# LArUtil
rootlibmap libLArLite_LArUtil.rootmap libLArLite_LArUtil.so $LARLITE_COREDIR/LArUtil/LinkDef.h \
    libLArLite_Base.so libTree.so













