#!/bin/sh

BASEDIR=$(cd $(dirname "$0") && pwd);

OPTIONS="--showfps $OPTIONS"

LD_LIBRARY_PATH="$BASEDIR"/data/lib/ "$BASEDIR"/hurrican-od $OPTIONS

exit $?
