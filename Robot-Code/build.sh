#!/bin/bash

for var in $@
do
	if [ "$var" == "--x86" ]; then
		PLATFORM="x86"
	elif [ "$var" == "--arm" ]; then
		PLATFORM="arm"
	elif [ "$var" == "--clean" ]; then
		BUILD="clean"
	fi
done

if [ -z "$PLATFORM" ]; then
	echo "WARNING: Using x86 as default target."
	PLATFORM="x86"
fi

#pushd lcmlite &> /dev/null
#make -f ${PLATFORM}-make-lcmlite.mak $BUILD
#popd &> /dev/null

#pushd soarev3 &> /dev/null
#make -f ${PLATFORM}-make-soarev3.mak $BUILD
#popd &> /dev/null

pushd src &> /dev/null
if [ "$PLATFORM" == "x86" ]; then
  make -f make_soar_client.mak $BUiLD
else
  make -f make_ev3_server.mak $BUILD
fi
popd &> /dev/null
