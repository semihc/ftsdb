#
# Necessary environment variables to work QmkBld based projects
#

# If Projects Home directory is not defined
PRJS_HOME=${PRJS_HOME:-$HOME}
export PRJS_HOME

# QmkBld related variables
QMKBLD=${QMKBLD:-$PRJS_HOME/qmkbld}
QMK_DEBUG=${QMK_DEBUG:-0}
QMAKEFEATURES=${QMAKEFEATURES:-$QMKBLD/prf}
export QMKBLD QMK_DEBUG QMAKEFEATURES

# Other projects that this one depends on:
QDECIMAL=${QDECIMAL:-$PRJS_HOME/qdecimal}
QT_CVT=${QT_CVT:-$PRJS_HOME/qt-cvt}
QT_BDB=${QT_BDB:-$PRJS_HOME/qt-bdb}
export QDECIMAL QT_CVT QT_BDB

export LD_LIBRARY_PATH=$QT_BDB/bdb/lib:$LD_LIBRARY_PATH

# The project that uses QmkBld 
export PRJ_ROOT=$PRJS_HOME/ftsdb

# Give feedback about project root
echo "PRJ_ROOT=$PRJ_ROOT"