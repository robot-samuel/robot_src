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
#define DCOM3 "/dev/ttyS4" 
#define DCOM4 "/dev/ttyS5"
#define DCOM5 "/dev/ttyS6"
#define DCOM6 "/dev/ttyS7"

#define DCOM7 "/dev/ttyXR0"
#define DCOM8 "/dev/ttyXR1"
#define DCOM9 "/dev/ttyXR2" 
#define DCOM10 "/dev/ttyXR3"

#define CONFIGLOGPATH "/home/pc-robot/workspace/controller/06-controller_232_for_sython_400W_v4/log/zlog.conf"
//#define CONFIGLOGPATH "/home/pc-robot/workspace/log/zlog.conf"
#include <robot2015_common/cpp/PackageMessage.pb.h>
#include <robot2015_common/cpp/ExternSensor.pb.h>
#include <robot2015_common/cpp/ControllerMessage.pb.h>
#include <robot2015_common/cpp/ParamConfig.pb.h>
#include <robot2015_common/cpp/BMSMessage.pb.h>
#include <robot2015_common/cpp/MotecDriverStat.pb.h>
#include <robot2015_common/cpp/AlarmMessage.pb.h>
#include <robot2015_common/cpp/ErrorMessage.pb.h>

#include <robot2015_common/cpp/ExternSensorConfig.pb.h>
#include <robot2015_common/cpp/BmsModeConfig.pb.h>

#include <robot2015_common/cpp/RobotStatusMessage.pb.h>
#include <robot2015_common/cpp/ph3airmsg.pb.h>

#endif // MYZMQCLIENT_H

