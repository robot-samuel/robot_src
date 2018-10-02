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

rosnode kill /BmsNode
rosnode kill /ZmqTransNode
rosnode kill /LaserCtrlNode
rosnode kill /JoyTestNode
rosnode kill /dgv_manageNode
rosnode kill /sythondriver_LB
rosnode kill /sythondriver_LF
rosnode kill /sythondriver_RB
rosnode kill /sythondriver_RF





