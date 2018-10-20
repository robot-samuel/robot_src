#ifndef MYZMQCLIENT_H
#define MYZMQCLIENT_H

#pragma once

#define ZMQ_NAME_CENTER "Rb_MsgCenter"
#define ZMQ_NAME_IMGRECO "Rb_ImageRecognition"
#define ZMQ_NAME_CAMSERV "Rb_CameraService"
#define ZMQ_NAME_AUDISERV "Rb_AudioService"
#define ZMQ_NAME_ROUTPLANMING "Rb_RoutePlanNavi"
#define ZMQ_NAME_PANTILT "Rb_PanTiltCtrl"
#define ZMQ_NAME_MANGSYS "Rb_ManagementSystem"
#define ZMQ_NAME_DRIVERCTRL "Rb_DriverCtrl"
#define ZMQ_NAME_SERVER "Rb_Server"

#define ZMQ_NAME_ALLNODE "Rb_ALLNode"

#define ZMQ_STAND_TOPIC_DRIVERUPDATE "Rb_Collector"

#define MYGEARTRpcTopic "HeartBeat"

#define UPDATE_FUN_NAME "Dcupdataall"

#define DCOM1 "/dev/ttyS0" 
#define DCOM2 "/dev/ttyS1"
#define DCOM3 "/dev/ttyS2" 
#define DCOM4 "/dev/ttyS3"


#define CONFIGLOGPATH "/home/idc-robot/workspace/controller/DriverController_V1/log/zlog.conf"
//#define CONFIGLOGPATH "/home/pc-robot/workspace/log/zlog.conf"
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

