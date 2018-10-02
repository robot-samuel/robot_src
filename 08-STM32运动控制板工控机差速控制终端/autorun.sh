#!/bin/sh -e
#
# rc.local
#
# This script is executed at the end of each multiuser runlevel.
# Make sure that the script will "exit 0" on success or any other
# value on error.
#
# In order to enable or disable this script just change the execution
# bits.
#
# By default this script does nothing.
source /opt/ros/indigo/setup.bash
#echo "123" > a1.txt 
#source /home/pc-robot/workspace/ubuntu_dev/02-controller_232/devel/setup.bash 
source /home/pc-robot/workspace/02-project/01-controller_232_for_sython_400W_v1/devel/setup.bash
#echo "123" >a2.txti
#export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/root/workspace/ubuntu_dev/02-controller_232/src
#export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH/root/workspace/ubuntu_dev/02-controller_232/src
#roslaunch /home/pc-robot/workspace/ubuntu_dev/02-controller_232/launch/Controller-test.launch 
roslaunch /home/pc-robot/workspace/02-project/01-controller_232_for_sython_400W_v1/launch/Controller-test.launch
exit 0
