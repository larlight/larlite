export USER_MODULE="BasicTool RecoTool HitRemoval SelectionTool LArLiteApp"
# Find the location of this script:
me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Find the directory one above.
export LARLITE_BASEDIR="$( cd "$( dirname "$me" )" && pwd )"

source $LARLITE_BASEDIR/config/setup.sh



