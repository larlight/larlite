
export USER_MODULE="BasicTool RecoTool"
# Find the location of this script:
me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Find the directory one above.
export LARLITE_BASEDIR="$( cd "$( dirname "$me" )" && pwd )"

source $LARLITE_BASEDIR/config/setup.sh

# Set up the event display if possible:
export PYTHONPATH="/Library/Python/2.7/site-packages/PyQt4:/System/Library/Frameworks/Python.framework/Versions/2.7/bin:${PYTHONPATH}"
export DYLD_FRAMEWORK_PATH="/Users/yuntse/Sources/qt-everywhere-opensource-src-5.5.1/qtbase/lib"
source $LARLITE_USERDEVDIR/DisplayTool/setup_evd.sh
if [ $BUILD_LARLITE_EVD ]; then
  USER_MODULE="${USER_MODULE} DisplayTool "
fi


