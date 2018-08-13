#!/bin/sh
#
# This script extracts the 0MQ version from include/zmq.h, which is the master
# location for this information.
#
basepath=$(cd `dirname $0`; pwd)
echo "scriptPath1: " $basepath
lauch="xrs2015.launch"

if [ ! -f ${basepath}/${lauch} ]; then
    echo ${lauch}" : error: "${basepath}/${lauch}" does not exist" 1>&2
    exit 1
fi
source /opt/ros/indigo/setup.sh 
roslaunch  ${lauch}

#echo "scriptPath2: "$(pwd)
#echo "scriptPath3: "$(dirname $(readlink -f $0))
#SOURCE_DIR="/root/robot_workspace/02-robot_dirver"
#ELC_DIR="ElcControl05091640/ElcControl05091640/"
#DIV_DIR="${SOURCE_DIR}/01-dirve/DriverControlv3.3/"
#
#if [ ! -f ${DIV_DIR}/DriverCtrl ]; then
#    echo "version.sh: error: ${DIV_DIR}/DriverCtrl does not exist" 1>&2
#    exit 1
#fi
#
#echo "start to run dirvctrl exec....."
#cd ${DIV_DIR}
#./DriverCtrl &
#cd /etc/init.d/
#exit 0
#
#MAJOR=`egrep '^#define +ZMQ_VERSION_MAJOR +[0-9]+$' include/zmq.h`
#MINOR=`egrep '^#define +ZMQ_VERSION_MINOR +[0-9]+$' include/zmq.h`
#PATCH=`egrep '^#define +ZMQ_VERSION_PATCH +[0-9]+$' include/zmq.h`
#if [ -z "$MAJOR" -o -z "$MINOR" -o -z "$PATCH" ]; then
#    echo "version.sh: error: could not extract version from include/zmq.h" 1>&2
#    exit 1
#fi
#MAJOR=`echo $MAJOR | awk '{ print $3 }'`
#MINOR=`echo $MINOR | awk '{ print $3 }'`
#PATCH=`echo $PATCH | awk '{ print $3 }'`
#echo $MAJOR.$MINOR.$PATCH | tr -d '\n'
