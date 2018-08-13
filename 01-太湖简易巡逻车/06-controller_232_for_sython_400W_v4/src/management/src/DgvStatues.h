#pragma once

#include "dgvmsg/ctrl_ModeMessage.h"
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include "dgvmsg/remoter.h"
#include "dgvmsg/DriverStatues.h"
#include "dgvmsg/Bms.h"
#include "dgvmsg/ExternSensor.h"

#include <boost/date_time/posix_time/posix_time.hpp>  
#include "zmqclient.h"


#define LEFTF_OBT 7
#define RIGHTF_OBT 6
#define LEFTB_OBT 5
#define RIGHTB_OBT 4

#define LEFTF_DROP 3
#define RIGHTF_DROP 2

class ErrorLogMessage
{
public:
	int time_start;
	int time_end;
	std::string Strerror;
	int errorcode;
};
class OnlineLogMessage
{
public:
       int online;
	   int timeout;
       boost::posix_time::ptime time_recvnow;  
       boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse;
       OnlineLogMessage()
       	{
		online = 0; 
		timeout=2;
		time_recvnow = boost::posix_time::microsec_clock::universal_time();  
       	}
	int check_connect()
	{   

	    if(online == 1)
	        {
	            //
	             boost::posix_time::ptime now;
	            // ����Ϊ΢��Ϊ��λ;������Խ�microsec_clock�滻��second_clock����Ϊ��λ;  
	            now = boost::posix_time::microsec_clock::universal_time();
	            time_elapse = now - time_recvnow;
	            // ����GetTickCount��ֻ����ߵõ�����2��ʱ���ticketֵ�Ĳ��΢��Ϊ��λ;  
	            int ticks = time_elapse.ticks();  
	            // �õ�����ʱ����������;   
	            int sec = time_elapse.total_seconds();
	            if(sec>=timeout){
	                    online = 0;
	                }
	            
	        
		}
        return online;
	        
	} 

	int Set_connect()
	{   
              online=1;
                    // ����Ϊ΢��Ϊ��λ;������Խ�microsec_clock�滻��second_clock����Ϊ��λ;  
               time_recvnow = boost::posix_time::microsec_clock::universal_time();  
	        
	} 
	int Set_timeout(int time)
	{   
		timeout =  time;
	} 	
       
};

class Dgv_statues
{
public:

	enum dev_add{
            MYADD  =100,
            DEV_DIR_ADD_LF =100,
            DEV_DIR_ADD_LB =102,
            DEV_DIR_ADD_RF =101,
            DEV_DIR_ADD_RB =103
		};
	
	enum CtrlID{
                    NONE = 0,    //����
                    NET_USER=1,  //�����û�����
                    RS232_USER=2,  //�����û�����
                    LASER_USER, 
                    REMOTE_JOY,
                    USBREMOTE_JOY   //ң��JOY�ֱ�����                
		};
	enum DEV_FANS{
    			ID_FAN_1 = 1,
    			ID_FAN_2 = 2
		};  

	enum DEV_BCTRL{
    			ID_ENABLE = 1,
    			ID_DISENABLE = 0
		};  
	enum DEV_NUM{
    			DRIVER_AMOUNT = 4
		};  
public:

	Dgv_statues();
	~Dgv_statues();
	void MyMemclear(const void *src,int count);
	
	//driver
	OnlineLogMessage driverNode[DRIVER_AMOUNT];
	//ControllerMessage_Dirver  Dirver[DRIVER_AMOUNT];

	dgvmsg::DriverStatues feedback[DRIVER_AMOUNT];
	
	unsigned char driveradd[DRIVER_AMOUNT];

	//laser
	OnlineLogMessage LaserCrelNode;
	dgvmsg::ctrl_ModeMessage laserCmd;
	int Laser_isconnect;
	int Laser_count;
	void laserCmdClear();
	void print_laserCmdMsg(const dgvmsg::ctrl_ModeMessage::ConstPtr& msg);
	
	//extern
	OnlineLogMessage ExternSnsorNode;	
	//ExternSensor  ExternModeInfo;
	dgvmsg::ExternSensor ExternModeInfo;
	
	//bms
	OnlineLogMessage BmsSysNode;
	//BmsMessage BmsModeInfo;
	dgvmsg::Bms BmsModeInfo;


	
	//������
	//OnlineLogMessage UltrasoundNode;
	//UltrasoundMessage UltrasoundInfo;

	//ZEROMQ
	OnlineLogMessage ZmqTransNode;

	//REMOTE_JOY
	OnlineLogMessage RemoteNode;
	int Remoteconnect;
	int Remote_count;
	dgvmsg::remoter Remotestdmsg;
	
	
	//USBREMOTE_JOY
	OnlineLogMessage USBJostickNode;
	geometry_msgs::Twist Joycmd_velocity; 
	
	int joyconnect;
	int joystick_count;
	 void print_joymsg(const sensor_msgs::Joy::ConstPtr& joy);

	void JoycmdClear();
	
	//RS232_USER
	OnlineLogMessage User232Node;
	int User232connect;
	int User232_count;
	
	//NET_USER
	OnlineLogMessage CenterNode;	
	int center_isconnect ;
	int center_count ;
	

	//ERROR
	ErrorMessage ErrorMsg;
	//ALARM
	AlarmMessage AlarmMsg;

	ParamConfig config;

	int speedVelocity; //����ʵ���ٶ�
	int angelVelocity; //�������ٶ�

	int cmd_speedVelocity; //�����ٶ�����
	int cmd_angelVelocity; //�������ٶ�����
	
	int IsDriverNodeConnect();
	int IsDriverError();
	int IsDriverComError();
	int IsCarCash();
	int IsCarObstacle();
	int IsCarBObstacle();
	int IsCarEmergencyStop();
	int IsCarDropStop();

	int CheckAlarm_JoyTestNodeOFF();
	int CheckAlarm_EcanExternSnsorNdeOFF();
	int CheckAlarm_CtrolBoxHighTem();
	int CheckAlarm_BMSModComLostAlarm();

	void CheckAlarm();
	//void Init_ExternModeInfo();
	void CheckOnline();
	void OnlineGroupInit();

	void CheckError();
	void CheckError_BmsCom();
	void CheckError_ExternCom();
	void CheckError_CurrentOverload();
	void CheckExternError();

	


	int checkCarCasherror();
	int checkCarObstacleerror();
	int checkCarBObstacleerror();
	int checkCarDroperror();

	void CheckAlarm_LaserComLost();
	void CheckAlarm_LaserNodeOFF();


	void msg_init();
	std::string pack_all_configinfo();
	
	
	
	
//private:
	struct DEVICE_DRIV_CTRL_ERROR_CODE1{
		unsigned char  bit0_FAULT_SYSTEM_ERROR; //ϵͳ���� 
		unsigned char  bit1_WARNING_I2T_ERROR;  //I2T����
		unsigned char  bit2_FAULT_PARAMETER_ERROR;  //��ʼ����������
		unsigned char  bit3_FAULT_UNDER_VOLTAGE;  //Ƿѹ 
		unsigned char  bit4_FAULT_OVER_VOLTAGE;  //��ѹ 
		unsigned char  bit5_FAULT_I2T_ERROR;   //I2T���� 
		unsigned char  bit6_WARNING_PEAKCURRENT_ARRIVED;  //�����ﵽ��ֵ����
		unsigned char  bit7_FAULT_POSITION_FOLLOWING_ERROR; //λ��ƫ�����
		unsigned char  bit8_FAULT_ENCODER; //�������źŹ��� 
		unsigned char  bit9_FAULT_VELOCITY_FOLLOWING_ERROR; //�ٶ�ƫ�����
		unsigned char  bit10_WARNING_IPM_TEMPERATURE;  //IPMģ���¶ȳ���һ����ֵ 
		unsigned char  bit11_FAULT_IPM_TEMPERATURE; //IPMģ���¶ȳ���������ֵ 
		unsigned char  bit12_FAULT_VELOBSERVER_FOLLOWING_ERROR; //�ٶȳ��ޱ��� 
		unsigned char  bit13_FAULT_FLASH_ERROR;  //FLASH ��д���� 
		unsigned char  bit14_FAULT_CURRENT_OFFSET;  //�����������쳣
		unsigned char  bit15_WARNING_LIMIT;  //�����������쳣 

	};
	struct DEVICE_DRIV_Status{
		unsigned char bit0_isDone;
		unsigned char bit1_isNearlyDone;
		unsigned char bit2_isPositiveLimit;
		unsigned char bit3_isNegativeLimit;
		unsigned char bit4_isIndexPulse;
		unsigned char bit5_logging_complete;
		unsigned char bit6_motor_enable;
		unsigned char bit7_error_status;
	};


	
	
	
};
