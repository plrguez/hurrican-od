#!/bin/sh

BASEDIR=$(cd $(dirname "$0") && pwd);

LD_LIBRARY_PATH="$BASEDIR"/data/lib/ "$BASEDIR"/hurrican-od

exit $?
