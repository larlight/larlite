#!/bin/bash

# clean up previously set env
me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)"
`python $me/python/clean_env.py LD_LIBRARY_PATH`
`python $me/python/clean_env.py DYLD_LIBRARY_PATH`
`python $me/python/clean_env.py PYTHONPATH`
`python $me/python/clean_env.py PATH`
if [[ -z $FORCE_LARLITE_BASEDIR ]]; then
    # If LARLITE_BASEDIR not set, try to guess
    # Find the location of this script:
    me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    # Find the directory one above.
    export LARLITE_BASEDIR="$( cd "$( dirname "$me" )" && pwd )"
else
    export LARLITE_BASEDIR=$FORCE_LARLITE_BASEDIR
fi
unset me;

if [[ -z $ENABLE_LARLITE_ALIAS ]]; then
    export ENABLE_LARLITE_ALIAS=1
fi

echo "LARLITE_BASEDIR = $LARLITE_BASEDIR"
if [[ -z $LARLITE_BASEDIR ]]; then
    echo \$LARLITE_BASEDIR not set! 
    echo You have to set this first.
    return;
fi
# Abort if ROOT not installed. Let's check rootcint for this.
if [ `command -v rootcling` ]; then
    export LARLITE_ROOT6=1
else 
    if [[ -z `command -v rootcint` ]]; then
	echo
	echo Looks like you do not have ROOT installed.
	echo You cannot use LArLite w/o ROOT!
	echo Aborting.
	echo
	return;
    fi
fi
LARLITE_OS=`uname -s`

# Set path to sub directories
export LARLITE_LIBDIR=$LARLITE_BASEDIR/lib
export LARLITE_COREDIR=$LARLITE_BASEDIR/core
export LARLITE_USERDEVDIR=$LARLITE_BASEDIR/UserDev

if [[ -z $USER_MODULE ]]; then
    export USER_MODULE=""
fi

# Check compiler availability for clang++ and g++
LARLITE_CXX=clang++
if [ `command -v $LARLITE_CXX` ]; then
    export LARLITE_CXX="clang++ -std=c++11";
else
    LARLITE_CXX=g++
    if [[ -z `command -v $LARLITE_CXX` ]]; then
	echo
	echo Looks like you do not have neither clang or g++!
	echo You need one of those to compile LArLite... Abort config...
	echo
	return;
    fi
    export LARLITE_CXX;
    if [ $LARLITE_OS = 'Darwin' ]; then
	echo $LARLITE_OS
	echo
	echo "***************** COMPILER WARNING *******************"
	echo "*                                                    *"
	echo "* You are using g++ on Darwin to compile LArLite.    *"
	echo "* Currently LArLite assumes you do not have C++11    *"
	echo "* in this combination. Contact the author if this is *"
	echo "* not the case. At this rate you have no ability to  *"
	echo "* compile packages using C++11 in LArLite.           *"
	echo "*                                                    *"
	echo "* Help to install clang? See manual/contact author!  *"
	echo "*                                                    *"
	echo "******************************************************"
	echo 
    fi
fi
if [[ -z $ROOTSYS ]]; then
    case `uname -n` in
	(houston.nevis.columbia.edu)
	    if [[ -z ${ROOTSYS} ]]; then
		source /usr/nevis/adm/nevis-init.sh
		setup root
		export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH;
	    fi
	    ;;
	(*)
	    echo
	    echo "****************** PyROOT WARNING ********************"
	    echo "*                                                    *"
	    echo "* Did not find your \$ROOTSYS. To use PyROOT feature, *"
	    echo "* Make sure ROOT.py is installed (comes with ROOT).  *"
	    echo "* You need to export \$PYTHONPATH to include the dir  *"
	    echo "* where ROOT.py exists.                              *"
	    echo "*                                                    *"
	    echo "* Help to install PyROOT? See manual/contact author! *"
	    echo "*                                                    *"
	    echo "******************************************************"
	    echo
	    ;;
	esac
else
    export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH;
fi

python $LARLITE_BASEDIR/config/python/gen_usermakefile.py
python $LARLITE_BASEDIR/config/python/gen_topmakefile.py
export LD_LIBRARY_PATH=$LARLITE_LIBDIR:$LD_LIBRARY_PATH
export PYTHONPATH=$LARLITE_BASEDIR/python:$PYTHONPATH
if [ $LARLITE_OS = 'Darwin' ]; then
    export DYLD_LIBRARY_PATH=$LARLITE_LIBDIR:$DYLD_LIBRARY_PATH
fi
export PATH=$LARLITE_BASEDIR/bin:$PATH
if [ $ENABLE_LARLITE_ALIAS ]; then
    alias maketop="make --directory=$LARLITE_BASEDIR"
    alias cdtop="cd $LARLITE_BASEDIR"
    alias llgen_package="python $LARLITE_BASEDIR/bin/gen_package"
    alias llgen_repository="python $LARLITE_BASEDIR/bin/gen_repository"
    alias llgen_class_empty="python $LARLITE_BASEDIR/bin/gen_class_empty"
    alias llgen_class_anaunit="python $LARLITE_BASEDIR/bin/gen_class_anaunit"
    alias llgen_class_erana="python $LARLITE_BASEDIR/bin/gen_class_erana"
    alias llgen_class_eralgo="python $LARLITE_BASEDIR/bin/gen_class_eralgo"
    alias llgen_class_erfilter="python $LARLITE_BASEDIR/bin/gen_class_erfilter"
fi
echo
echo "Finish configuration. To build, type:"
echo "> cd \$LARLITE_BASEDIR"
echo "> make"
echo
