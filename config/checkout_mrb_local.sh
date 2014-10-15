#!/bin/bash

command -v mrb >/dev/null 2>&1 || { echo >&2 "MRB seems not set up (required!). Aborting."; return; }

# Set my larsoft path
if [ -z $1 ]; then
    echo 1st argument must be the installation location. No argument provided!
    return;
fi

MY_LARSOFT=$1
if [ ! -d $MY_LARSOFT ]; then
    mkdir -p $MY_LARSOFT
fi
if [ ! -d $MY_LARSOFT ]; then
    echo Creation of a directory $MY_LARSOFT failed... Aborting.
    return;
fi

if [ "$(ls -A $MY_LARSOFT)" ]; then
    echo $MY_LARSOFT is not an empty directory! Aborting.
    return;
else
 
    # Next steps involve cd to different directries. Remember this directory before doing that & come back later
    export TMP_PWD=$PWD

    # Checkout my larsoft
    cd $MY_LARSOFT
    mrb newDev
    
    cd $TMP_PWD
    unset TMP_PWD
fi

#done
