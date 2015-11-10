#!/usr/bin/env bash

# If PUB_TOP_DIR not set, try to guess
if [[ -z $PUB_TOP_DIR ]]; then
    # Find the location of this script:
    me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    # Find the directory one above.
    export PUB_TOP_DIR="$( cd "$( dirname "$me" )" && pwd )"
fi

case `whoami` in
    (ubooneshift)
        echo Setting up PUBS for ubooneshift account...
	;;
    (*)
        echo This should only be used for ubooneshift account!!!
	echo Exiting with exterme prejudice.
	return 1
	;;
esac


#
# PSQL configuration
#
export PGOPTIONS="-c client_min_messages=WARNING";

#
# Python configuration
#
# Set PATH
export PATH=$PUB_TOP_DIR/bin:$PATH
# Set PYTHONPATH
export PYTHONPATH=$PUB_TOP_DIR:$PYTHONPATH
# BIN executable directory
export PUB_BIN_DIR=$PUB_TOP_DIR
export PATH=$PUB_BIN_DIR:$PATH

#
# Project configuration
# 
# Default logger level
export PUB_LOGGER_LEVEL=kLOGGER_DEBUG
#shifters don't need the cout spam
if [ `whoami` == 'uboonedaq' ]; then
    export PUB_LOGGER_LEVEL=kLOGGER_ERROR
fi
# Default message drain
export PUB_LOGGER_DRAIN=kLOGGER_COUT
#export PUB_LOGGER_DRAIN=kLOGGER_FILE
export PUB_LOGGER_FILE_LOCATION=$PUB_TOP_DIR/log

#
# Daemon configuration
#
export PUB_DAEMON_LOG_MODULE=""
export PUB_DAEMON_HANDLER_MODULE=""

#
# Server-specific configuration
#
case `uname -n` in
    (uboone-cr-*.fnal.gov)
	echo Setting up for uboone-cr-XX...
	if [ -z $HOME/.sqlaccess/prod_access.sh ]; then
	    echo 'Configuration @ gpvm requires \$HOME/.sqlaccess/prod_access.sh!'
	    echo 'Exiting...'
	    echo 
	    return;
	fi
	source $HOME/.sqlaccess/prod_access.sh
	source /grid/fermiapp/products/uboone/setup_uboone.sh
	#setup uboonecode v04_26_02 -q e7:prof
	setup postgresql v9_3_6 -q p279
	#setup larbatch v01_18_00
	setup -t sam_web_client

	export PUB_LOGGER_FILE_LOCATION=$PUB_TOP_DIR/log/`uname -n`
	mkdir -p $PUB_LOGGER_FILE_LOCATION;
	#export PUB_DAEMON_LOG_MODULE=dstream_prod.gpvm_logger
	;;
    (*)
        echo This should only be used for uboonegpvm account!!!
	echo Exiting with extreme prejudice.
	return 1
	;;
esac



