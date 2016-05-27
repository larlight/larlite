#!/usr/bin/bash

# This section of code determines where the evd is stored.

if [ -z ${LARLITE_BASEDIR+x} ]; then 
  echo "Must set up larlite to use this!";
  return 
fi

#SOURCE="${BASH_SOURCE[0]}"
#while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
#  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
#  SOURCE="$(readlink "$SOURCE")"
#  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
#done
#DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

# This section extends the path and python path to run from anywhere
export DIR=$LARLITE_USERDEVDIR/DisplayTool/

git submodule init 
git submodule update

# This section verifies that python dependences are setup 

PYTHONPATH_backup=$PYTHONPATH
PATH_backup=$PATH

if [[ ! ":$PATH:" == *":$DIR/python:"* ]]; then
  export PATH=$DIR/python:$PATH
fi

if [[ ! ":$PYTHONPATH:" == *":$DIR/python:"* ]]; then
  export PYTHONPATH=$DIR/python/:$PYTHONPATH
fi

# Test argparse
if ! $(python -c "import argparse" &> /dev/null); then 
  echo "Warning: can not use evd due to missing package argparse"
  export PATH=$PATH_backup
  export PYTHONPATH=$PYTHONPATH_backup
  return
fi

# Test numpy
if ! $(python -c "import numpy" &> /dev/null); then 
  echo "Warning: can not use evd due to missing package numpy"
  export PATH=$PATH_backup
  export PYTHONPATH=$PYTHONPATH_backup 
  return
fi

# Test pyqt4
if ! $(python -c "import pyqtgraph.Qt" &> /dev/null); then 
  echo "Warning: can not use evd due to missing package PyQt"
  export PATH=$PATH_backup
  export PYTHONPATH=$PYTHONPATH_backup
  return
fi




export BUILD_LARLITE_EVD=true
