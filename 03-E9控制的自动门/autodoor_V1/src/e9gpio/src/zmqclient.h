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
#define ZMQ_NAME_AUTTODOOR "Rb_CtrlAutodoor"
#define ZMQ_NAME_WEATHERSERVICE "Rb_WeatherService"
#define ZMQ_NAME_SERVER "Rb_Server"


#define MYGEARTRpcTopic "HeartBeat"

#define DCOM1 "/dev/ttymxc0" 
#define DCOM2 "/dev/ttymxc1"
#define DCOM3 "/dev/ttymxc2" 
#define DCOM4 "/dev/ttymxc3"

#define DCOM11 "/dev/ttyUSB0"

#include <robot2015_common/cpp/PackageMessage.pb.h>
#include <robot2015_common/cpp/WeatherMessage.pb.h>





#endif // MYZMQCLIENT_H

