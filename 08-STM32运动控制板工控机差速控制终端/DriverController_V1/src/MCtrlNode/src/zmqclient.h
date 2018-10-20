#ifndef MYZMQCLIENT_H
#define MYZMQCLIENT_H




#define DCOM1 "/dev/ttyS0"
#define DCOM2 "/dev/ttyS1"
#define DCOM3 "/dev/ttyS2"
#define DCOM4 "/dev/ttyS3"

#define CONFIGLOGPATH "/home/idc-robot/workspace/controller/DriverController_V1/log/zlog.conf"


#include <robot2015_common/cpp/PackageMessage.pb.h>


#include "ros/ros.h"
#include "ros/console.h"

#include "std_msgs/String.h"
#include "dgvmsg/ctrl_ModeMessage.h"
#include "dgvmsg/Encounter.h"
#include "dgvmsg/EncounterV.h"
#include <geometry_msgs/Twist.h>
#include "dgvmsg/DriverVelocity.h"

#include "dgvmsg/DriverNode.h"
#include <sensor_msgs/Joy.h>
#include "dgvmsg/DriverStatues.h"


#endif // MYZMQCLIENT_H

