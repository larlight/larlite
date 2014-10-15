#!/bin/bash

command -v mrb >/dev/null 2>&1 || { echo >&2 "MRB seems not set up (required!). Aborting."; return; }

# Set my larsoft path
if [ -z $1 ]; then
    echo 1st argument must be the installation location. No argument provided!
    return;
fi

MY_LARSOFT=$1
if [ ! -d $MY_LARSOFT ]; then
    echo Directory \"$MY_LARSOFT\" not found! Aborting...
    return;
fi

export MY_LARSOFT_SETUP=`ls $MY_LARSOFT | grep localProducts`
if [ -z $MY_LARSOFT_SETUP ]; then

    echo The directory $MY_LARSOFT does not contain a setup script ... not properly checked out?
    return
fi
 
# Next steps involve cd to different directries. Remember this directory before doing that & come back later
export TMP_PWD=$PWD

# Configure my larsoft
cd $MY_LARSOFT
source $MY_LARSOFT_SETUP/setup
cd build
#source mrb s
mrbsetenv

# Set up local installation
cd $MY_LARSOFT
#source mrb slp
mrbslp
    
cd $TMP_PWD
unset TMP_PWD

#done
