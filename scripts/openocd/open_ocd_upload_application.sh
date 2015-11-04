#!/bin/bash

if [ ! -n "$1" ] || [ ! -f "$1" ] ; then
    echo "Please give a valid binary file as argument"
    exit -1
fi

openocd -f `dirname $0`/nrf51_open-ocd_st-link-v2.cfg \
-c "init" \
-c "halt" \
-c "program $1 reset" \
-c "exit "

if [ $? == 0 ] ; then
    echo "*** UPLOAD SUCCESS *** "
    exit 0
else
    echo "*** UPLOAD FAILED *** "
    exit $?
fi 