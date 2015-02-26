#!/bin/bash

temp=make_rootmap.$$.temp

rootlibmap() {
 ROOTMAP=$1
 SOFILE=$2
 LINKDEF=$3
 shift 3
 DEPS=$*
 echo boke
 if [[ -e $SOFILE && -e $LINKDEF ]]; then
     echo rlibmap -f -o $ROOTMAP -l $SOFILE -d $DEPS -c $LINKDEF 
     rm -f $temp
 fi
}

######################################################
# Base
echo ahoaho
rootlibmap libLArLite_Base.rootmap libLArLite_Base.so $LARLITE_COREDIR/Base/LinkDef.h libCore
