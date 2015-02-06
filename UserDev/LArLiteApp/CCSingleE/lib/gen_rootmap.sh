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
# CCSingleE
rootlibmap libLArLiteApp_CCSingleE.rootmap libLArLiteApp_CCSingleE.so $LARLITE_USERDEVDIR/LArLiteApp/CCSingleE/LinkDef.h \
    libSelectionTool_ERBase.so \
    libLArLite_Analysis.so \
    libLArLite_ERToolBackend.so














