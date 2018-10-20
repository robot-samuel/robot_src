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

rosnode kill -a
pgrep roslaunch |xargs sudo kill -s 9
pgrep rosmaster |xargs sudo kill -s 9
#pgrep ZmqProxy| xargs kill -s 9
#ps -ef | grep ZmqProxy
pgrep ZmqProxy |xargs sudo kill -9
pgrep rosout| xargs sudo kill -s 9

