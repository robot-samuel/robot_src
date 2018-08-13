#ifndef MYZMQCLIENT_H
#define MYZMQCLIENT_H


#define ZMQ_NAME_CENTER "Rb_MsgCenter"
#define ZMQ_NAME_IMGRECO "Rb_ImageRecognition"
#define ZMQ_NAME_CAMSERV "Rb_CameraService"
#define ZMQ_NAME_AUDISERV "Rb_AudioService"
#define ZMQ_NAME_ROUTPLANMING "Rb_RoutePlanNavi"
#define ZMQ_NAME_PANTILT "Rb_PanTiltCtrl"
#define ZMQ_NAME_MANGSYS "Rb_ManagementSystem"
#define ZMQ_NAME_DRIVERCTRL "Rb_DriverCtrl"


#define MYGEARTRpcTopic "HeartBeat"

#define DCOM1 "/dev/ttyS0" 
#define DCOM2 "/dev/ttyS1"
#define DCOM3 "/dev/ttyS2" 
#define DCOM4 "/dev/ttyS3"
#define DCOM5 "/dev/ttyS4"
#define DCOM6 "/dev/ttyS5"

#define DCOM7 "/dev/ttyXR0"
#define DCOM8 "/dev/ttyXR1"
#define DCOM9 "/dev/ttyXR2" 
#define DCOM10 "/dev/ttyXR3"


#include <robot2015_common/cpp/PackageMessage.pb.h>
#include <robot2015_common/cpp/ExternSensor.pb.h>
#include <robot2015_common/cpp/ExternSensorConfig.pb.h>

#include <robot2015_common/cpp/ControllerMessage.pb.h>
#include <robot2015_common/cpp/ParamConfig.pb.h>

#include <robot2015_common/cpp/BMSMessage.pb.h>
#include <robot2015_common/cpp/BmsModeConfig.pb.h>

#include <robot2015_common/cpp/MotecDriverStat.pb.h>
#include <robot2015_common/cpp/AlarmMessage.pb.h>
#include <robot2015_common/cpp/ErrorMessage.pb.h>




#endif // MYZMQCLIENT_H

