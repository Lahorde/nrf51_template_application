#!/bin/bash
SCRIPT_DIR=`dirname $0`/jlink
mkdir -p tmp
TEMP_DIR=tmp

DEVICE=nrf51822

FILE=${1:? "$0 requires \"file\" as first argument"}

JLINK=${2:? "$0 requires \"a JLink programmer\" as second argument"}

APPLICATION_START_ADDRESS=${3:? "$0 requires \"a flash address\" as third argument"}

if [ ! -e ${FILE} ]; then
	echo "Error: ${FILE} does not exist..."
	exit
fi

if [ ! -e ${JLINK} ]; then
	echo "Error: ${JLINK} does not exist..."
	exit
fi

#check_mime=$(mimetype $FILE)
#if [ $check_mime != "application/octet-stream" ]; then
#	echo "Error: ${FILE} has mime-type $check_mime instead of application/octet-stream"
#	echo "Are you perhaps trying to upload the .elf file?"
#	exit
#fi

if [[ ! $FILE == *.hex ]]; then
	echo "Error: ${FILE} has not the extension .hex"
	echo "Are you perhaps trying to upload the .elf file?"
	exit
fi

sed "s|@BIN@|$FILE|" $SCRIPT_DIR/upload.script > $TEMP_DIR/upload.script

echo "Program application starting from $APPLICATION_START_ADDRESS"

sed -i "s|@START_ADDRESS@|$APPLICATION_START_ADDRESS|" $TEMP_DIR/upload.script

$JLINK -Device $DEVICE -If SWD $TEMP_DIR/upload.script 
rm -rf $TEMP_DIR