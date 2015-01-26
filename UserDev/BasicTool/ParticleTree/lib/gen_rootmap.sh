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
# ParticleTree
rootlibmap libBasicTool_ParticleTree.rootmap libBasicTool_ParticleTree.so $LARLITE_USERDEVDIR/BasicTool/ParticleTree/LinkDef.h 
