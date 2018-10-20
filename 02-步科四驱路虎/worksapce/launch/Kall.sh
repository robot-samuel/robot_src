#!/bin/sh
#
# This script extracts the 0MQ version from include/zmq.h, which is the master
# location for this information.
#
#kill -9 $(pidof ElcCtrl)
#kill -9 $(pidof DriverCtrl)
rosnode kill -a
