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
# Package_Name
rootlibmap libRepositoryName_Package_Name.rootmap libRepositoryName_Package_Name.so $LARLITE_USERDEVDIR/RepositoryName/Package_Name/LinkDef.h \
    libLArLite_Analysis.so \
    libLArLiteApp_ERToolBackend.so \
    libBasicTool_GeoAlgo.so \
    libSelectionTool_ERBase.so \
    libSelectionTool_ERAlgo.so \
    libSelectionTool_ERFilter.so \
    libSelectionTool_ERApp.so














