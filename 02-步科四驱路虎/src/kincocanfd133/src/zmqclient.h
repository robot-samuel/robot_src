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

 
#define DCOM2 "/dev/ttymxc1"
#define DCOM3 "/dev/ttymxc2" 
#define DCOM4 "/dev/ttymxc3"

#define CAN1 "can0"
#define CAN2 "can1"

#include <robot2015_common/cpp/PackageMessage.pb.h>
#include <robot2015_common/cpp/ControllerMessage.pb.h>
#include <robot2015_common/cpp/ParamConfig.pb.h>
#include <robot2015_common/cpp/BMSMessage.pb.h>
#include <robot2015_common/cpp/AlarmMessage.pb.h>
#include <robot2015_common/cpp/ErrorMessage.pb.h>

#include <robot2015_common/cpp/ParamConfig.pb.h>
#include <robot2015_common/cpp/BmsModeConfig.pb.h>


#endif // MYZMQCLIENT_H

