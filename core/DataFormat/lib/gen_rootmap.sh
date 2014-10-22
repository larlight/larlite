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
# DataFormat
rootlibmap libLArLite_DataFormat.rootmap libLArLite_DataFormat.so $LARLITE_COREDIR/DataFormat/LinkDef.h \
    libPhysics.so libTree.so libLArLite_Base.so
