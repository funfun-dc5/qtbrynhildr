#!/bin/sh
if [ "${QTB_HOME}" != "" ] ; then
	appname="Qt Brynhildr"

	LD_LIBRARY_PATH=${QTB_HOME}/bin
	export LD_LIBRARY_PATH
	"${QTB_HOME}/bin/${appname}" "$@"
else
	echo "Set QTB_HOME!"
fi
